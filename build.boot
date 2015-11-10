;; This library is distributed in  the hope that it will be useful but without
;; any  warranty; without  even  the  implied  warranty of  merchantability or
;; fitness for a particular purpose.
;; The use and distribution terms for this software are covered by the Eclipse
;; Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
;; can be found in the file epl-v10.html at the root of this distribution.
;; By using this software in any  fashion, you are agreeing to be bound by the
;; terms of this license. You  must not remove this notice, or any other, from
;; this software.
;; Copyright (c) 2013-2015, Ken Leung. All rights reserved.

(set-env!

  :buildVersion "0.9.0-SNAPSHOT"
  :buildDebug true
  :buildType "cpp"

  :DOMAIN "czlab.fusilli"
  :PID "fusilli"

  :source-paths #{"src/main/cpp" }

  :dependencies '[])

(require '[czlab.tpcl.boot :as b :refer [fp! ge ]]
         '[clojure.data.json :as js]
         '[clojure.java.io :as io]
         '[clojure.string :as cs]
         '[boot.core :as bc]
         '[czlab.tpcl.antlib :as a])

(import '[java.io File]
        '[java.util Stack])

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(b/BootEnvVars)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(b/BootEnvPaths)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; task definitions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- cocos->new ""

  [appid]

  (let [gdir (fp! (ge :basedir) "games")]
    (a/RunTasks*
      (a/AntMkdir {:dir gdir})
      (a/AntExec
        {:executable "cocos"}
        [[:argvalues ["new" "-l" "cpp"
                      "-d" gdir appid]]]))
    (let [des (fp! gdir appid)
          res (io/file des "Resources")
          cls (io/file des "Classes") ]
      (a/CleanDir res)
      (a/CleanDir cls))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- deploy->core ""

  [appid]

  (let [top (io/file (ge :srcDir) "cpp/fusilli")
        base (ge :basedir)]
    (doseq [f (.listFiles top)
            :when (.isDirectory f)
            :let [fname (.getName f)]]
      (a/SymLink (fp! base "games" appid "Classes" fname)
                 (.getCanonicalPath f)))
    (a/SymLink (fp! base "games" appid "Classes" "GSL")
               (fp! (ge :srcDir) "cpp/GSL"))
    (a/SymLink (fp! base "games" appid "Classes" "dbox")
               (fp! (ge :srcDir) "cpp/dbox"))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- deploy->res ""

  [appid]

  (let [top (io/file (ge :srcDir) "cpp" appid "res")
        base (ge :basedir)]
    (doseq [f (.listFiles top)
            :let [fname (.getName f)]]
      (a/SymLink (fp! base "games" appid "Resources" fname)
                 (.getCanonicalPath f)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- deploy->app ""

  [appid]

  (let [top (io/file (ge :srcDir) "cpp" appid)
        base (ge :basedir)]
    (doseq [f (.listFiles top)
            :let [fname (.getName f)]]
      (if (and (.isDirectory f)
               (= "res" fname))
        (deploy->res appid)
        (a/SymLink (fp! base "games" appid "Classes" fname)
                   (.getCanonicalPath f))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- syncMakeFile ""

  [appid]

  (let [top (fp! (ge :basedir) "games" appid)
        f (fp! top "CMakeLists.txt")
        c1 (cs/join "\n"
                    (map #(str "Classes/" %)
                         (b/CollectFilePaths (fp! top "Classes") ".cpp")))
        h1 (cs/join "\n"
                    (map #(str "Classes/" %)
                         (b/CollectFilePaths (fp! top "Classes") ".h"))) ]
    (b/ReplaceFile f #(cs/replace % "@@GAMEHEADERS@@" h1))
    (b/ReplaceFile f #(cs/replace % "@@GAMESRC@@" c1))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- clean-cocos ""

  [appid]

  (a/CleanDir (fp! (ge :basedir) "games" appid "Resources"))
  (a/CleanDir (fp! (ge :basedir) "games" appid "Classes")))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask ccnew

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (cocos->new id)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask cclink

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (clean-cocos id)
    (deploy->core id)
    (deploy->app id)
    (syncMakeFile id)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask ccclean

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (clean-cocos id)
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- XXsyncMakeFile ""

  [appid]

  (let [f (fp! (ge :basedir) "games" appid "CMakeLists.txt")
        c1  (str
              (cs/join "\n"
                       (map #(str "Classes/" %)
                            (b/CollectFilePaths (fp! (ge :srcDir) "cpp" "fusilli") ".cpp")))
              "\n"
              (cs/join "\n"
                       (map #(str "Classes/" %)
                            (b/CollectFilePaths (fp! (ge :srcDir) "cpp" appid) ".cpp"))))
        h1  (str
              (cs/join "\n"
                       (map #(str "Classes/" %)
                            (b/CollectFilePaths (fp! (ge :srcDir) "cpp" "fusilli") ".h")))
              "\n"
              (cs/join "\n"
                       (map #(str "Classes/GSL/" %)
                            (b/CollectFilePaths (fp! (ge :srcDir) "cpp" "GSL") ".h")))
              "\n"
              (cs/join "\n"
                       (map #(str "Classes/" %)
                            (b/CollectFilePaths (fp! (ge :srcDir) "cpp" appid) ".h")))) ]
    (b/ReplaceFile f #(cs/replace % "@@GAMEHEADERS@@" h1))
    (b/ReplaceFile f #(cs/replace % "@@GAMESRC@@" c1))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(defn- deploy->XXXapp ""

  [appid]

  (let [vendors (fp! (ge :basedir) "public/vendors")
        despath (fp! (ge :basedir) "cocos" appid)
        srcpath (fp! (ge :webDir) appid)
        des (io/file despath)
        src (io/file srcpath)
        dd2 (io/file des "res")
        dd1 (io/file des "src") ]

    (when-not (.exists des) (cocos->new appid))
    ;;(print (format "Deploying game: %s\n" appid))
    (a/CleanDir dd2)
    (a/CleanDir dd1)
    ;; resources
    (doseq [s ["hdr" "hds" "sd" "sfx"]]
      (a/RunTasks*
        (a/AntCopy
          {:todir (io/file dd2 s "cocos2d") }
          [[:fileset {:dir (fp! (ge :webDir) "cocos2d/res" s)} ]])
        (a/AntCopy
          {:todir (io/file dd2 s appid)}
          [[:fileset {:dir (io/file (ge :srcpath) "res" s)} ]])))
    ;; js code
    (a/RunTasks*
      (a/AntCopy
        {:todir (io/file dd1 "zotohlab")}
        [[:fileset {:dir (fp! (ge :webDir) "cocos2d/src/zotohlab")} ]])
      (a/AntCopy
        {:todir (io/file dd1 appid)}
        [[:fileset {:dir (fp! srcpath "src")} ]])
      (a/AntCopy
        {:todir (io/file dd1 "helpers")}
        [[:fileset {:dir (fp! vendors "helpers")
                    :includes "dbg.js"}]]))

    (doseq [s ["almond" "js-signals" "ash-js"
               "rxjs" "ramda" "cherimoia"
               "l10njs" "cookies" "mustache"]]
      (a/RunTasks*
        (a/AntCopy
          {:todir (io/file dd1  s)}
          [[:fileset {:dir (io/file vendors s)} ]])))

    ;; boot stuff
    (a/RunTasks*
      (a/AntCopy
        {:todir despath
         :overwrite true}
        [[:fileset {:dir (io/file (ge :srcDir) "resources")
                    :includes (str "manifest.webapp,project.json,"
                                   "main.js,index.html")}]]))

    (let [j1 (-> (slurp (fp! (ge :srcDir)
                             "resources/project.json")
                        :encoding "utf-8")
                 (js/read-str :key-fn keyword))
          jm (-> (slurp (io/file despath "manifest.webapp")
                        :encoding "utf-8"))
          j2 (-> (slurp (fp! srcpath "src/project.json")
                        :encoding "utf-8")
                 (js/read-str :key-fn keyword)) ]
      (->> (-> jm
              (cs/replace "@@APP@@" appid)
              (cs/replace "@@USER@@" (System/getProperty "user.name")))
           (spit (io/file despath "manifest.webapp")))

      (->> (update-in j1
                      [:jsList]
                      #(concat % (:jsList j2)))
           (js/write-str )
           (spit (io/file despath "project.json"))))

    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(deftask deployapp

  ""
  [n id VAL str "game id"]

  (bc/with-pre-wrap fileset
    (let [srcpath (fp! (ge :webDir) id)
          src (io/file srcpath)]
      (if-not (.exists src)
        (print (format "Invalid game: %s\n" id))
        (deploy->app id)))
    fileset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;EOF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;

