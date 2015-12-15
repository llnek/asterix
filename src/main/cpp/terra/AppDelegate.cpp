// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "ash/NodeRego.h"
#include "n/GNodes.h"
#include "p/Config.h"
#include "AppDelegate.h"
NS_USING(terra)
NS_USING(fusii)
NS_USING(ash)

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::AppDelegate() {

    {
        int x=3, y=4;
        auto mm= new int[x][4];
        
    }
    typedef int POO[2][2];
    
    
    std::array<POO,2> qqq {
        1,2,3,4,
         5,6,7,8
    };
    void*  rk= &qqq[0];
    auto www= (int (*)[2][2] )rk;
    int ggg[]= {7,8};
    auto hh = &ggg;
    printf("%d %d %d %d\n",
           qqq[1][0][0],
           qqq[1][0][1],
           qqq[1][1][0],
           qqq[1][1][1]);
    
//    int pp[2][2]= {1, 2, 3, 4};
//    auto kkk= new int[2][2];
//    auto ppp= &pp;
//    printf("%p %p", ppp, kkk);
//        printf("%d %d %d %d\n", p[0][0], p[0][1],
//               p[1][0], p[1][1]
//               );
    
//    int (*p)[2] =new int[2][2];
//    p[0][0]=1; p[0][1]=2;
//    p[1][0]=3; p[1][1]=4;
//    int i=1;
//    printf("%d %d %d %d\n", (*p)[0], (*p)[1],
//           (*(p+i))[0], (*(p+i))[1]
//           );
  // step.1: register all ash::node factories here
  auto r= NodeRegistry::self();

  r->rego( mc_new(ShipNode));

  // step.2: set up app-config
  Config::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
AppDelegate::~AppDelegate() {
  //delete NodeRegistry::Self();
  //delete XConfig::Self();
}


