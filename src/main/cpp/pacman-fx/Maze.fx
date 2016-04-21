/*
 * Maze.fx
 *
 * Created on 2008-12-20, 20:22:15
 */

package pacman;

import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import pacman.MazeData;
import pacman.PacMan;
import pacman.WallBlackLine;
import pacman.WallBlackRectangle;
import pacman.WallRectangle;
import javafx.scene.input.*;
import javafx.scene.text.Text;
import javafx.scene.text.Font;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import java.lang.Math;
import javafx.scene.shape.ArcType;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;

/**
 * @author Henry Zhang
 */

public class Maze extends CustomNode {

  // counter for ghosts eaten
  var ghostEatenCount : Integer;

  // text to be displayed for score of eating a ghost
  var scoreText = [
    ScoreText {
      content: "200"
      visible: false;
    },
    ScoreText {
        content: "400"
        visible: false;
    },
    ScoreText {
        content: "800"
        visible: false;
    },
    ScoreText {
        content: "1600"
        visible: false;
    },
  ];

  // Pac Man Character
  public var pacMan : PacMan = PacMan{ maze:this x:15 y:18 } ;

  public var ghostBlinky = Ghost {
    defaultImage1: Image {
      url: "{__DIR__}images/ghostred1.png"
    }

    defaultImage2: Image {
      url: "{__DIR__}images/ghostred2.png"
    }

     maze: this
     pacMan: pacMan
     x: 17
     y: 15
     xDirection: 0
     yDirection: -1
     trapTime: 1
    };

   public var ghostPinky = Ghost {
     defaultImage1:Image {
        url: "{__DIR__}images/ghostpink1.png"
     }

     defaultImage2:Image {
       url: "{__DIR__}images/ghostpink2.png"
     }
     
     maze: this
     pacMan: pacMan
     x: 12
     y: 14
     xDirection: 0 
     yDirection: 1
     trapTime: 10
   };

   public var ghostInky = Ghost {
     defaultImage1:Image {
       url: "{__DIR__}images/ghostcyan1.png"
     }
     defaultImage2:Image {
       url: "{__DIR__}images/ghostcyan2.png"
     }

     maze: this
     pacMan: pacMan
     x: 13
     y: 15
     xDirection: 1
     yDirection: 0
     trapTime: 20
   };

   public var ghostClyde = Ghost {
     defaultImage1:Image {
        url: "{__DIR__}images/ghostorange1.png"
     }
     defaultImage2:Image {
       url: "{__DIR__}images/ghostorange2.png"
     }

     maze: this
     pacMan: pacMan
     x: 15
     y: 14
     xDirection: -1
     yDirection: 0
     trapTime: 30
   };

  public var ghosts = [ghostBlinky, ghostPinky, ghostInky, ghostClyde];

  public var dyingPacMan =
    DyingPacMan {
      maze: this
      centerX: 0
      centerY: 0
      radiusX: 13
      radiusY: 13
      startAngle: 90
      length: 360
      type: ArcType.ROUND
      fill: Color.YELLOW
      visible: false;
   } ;

  // the pac man image
  var pacmanImage =Image {
    url: "{__DIR__}images/left1.png"
  }

  // images showing how many lives remaining
  var livesImage = [
    ImageView {
      image: pacmanImage
      x: MazeData.calcGridX(18)
      y: MazeData.calcGridY(30)
      visible: bind livesCount > 0
    },
    ImageView {
      image: pacmanImage
      x: MazeData.calcGridX(16)
      y: MazeData.calcGridY(30)
      visible: bind livesCount > 1
    },
    ImageView {
      image: pacmanImage
      x: MazeData.calcGridX(14)
      y: MazeData.calcGridY(30)
      visible: bind livesCount > 2
    }
  ];

  // level of the game
  public var level : Integer = 1;

  // flag to add a life to the player if the first time scores exceed 10000
  var addLifeFlag: Boolean = true;

  // current lives of the player
  var livesCount = 2;

  // message to start a game
  var waitForStart: Boolean = true;
  var messageBox = Group {
    content: [
      Rectangle {
        x: MazeData.calcGridX(5)
        width: MazeData.GRID_GAP * 19
        y: MazeData.calcGridY(21)
        height: MazeData.GRID_GAP *5
        stroke: Color.RED
        strokeWidth: 5
     
        fill: Color.CYAN
        opacity: 0.75
        arcWidth: 25
        arcHeight: 25
      },
      Text {
        font: Font { size: 19 }
        x: MazeData.calcGridX(7)
        y: MazeData.calcGridY(24)

        content: "PRESS ANY KEY TO START"
        fill: Color.RED
      }
    ]
  };

 // whether the last finished game is won by the player
 var lastGameResult: Boolean = false;

 // text of game winning
 var gameResultText =
   Text {
     font: Font { size: 20
                 }
     x: MazeData.calcGridX(11)
     y: MazeData.calcGridY(11)+8

     content: bind if ( lastGameResult )
                     " YOU WIN "
                   else
                     "GAME OVER";
     fill: Color.RED
     visible: false;
   } ;

 var flashingCount: Integer = 0;
 var flashingTimeline: Timeline =
   Timeline {
     repeatCount: 5
     keyFrames : [
       KeyFrame {
         time : 500ms
         action: function() {
           gameResultText.visible = not gameResultText.visible;
           if ( ++flashingCount == 5) {
             messageBox.visible = true;
             waitForStart = true;
           }
         }
       }
     ]
   };

  var group : Group =
  Group {
    content: [
      Rectangle {
        x:0
        y:0
        width: MazeData.calcGridX(MazeData.GRID_SIZE + 2)
        height: MazeData.calcGridY(MazeData.GRID_SIZE + 3)
        fill: Color.BLACK
      },

      WallRectangle{ x1:0 y1:0 x2:MazeData.GRID_SIZE y2:MazeData.GRID_SIZE },

      WallRectangle { x1:14 y1:-0.5 x2:15 y2:4 },
      WallBlackRectangle { x1:13.8 y1:-1 x2:15.3 y2:0 },

      WallRectangle { x1:2 y1:2 x2:5 y2:4 },
      WallRectangle { x1:7 y1:2 x2:12 y2:4 },
      WallRectangle { x1:17 y1:2 x2:22 y2:4 },
      WallRectangle { x1:24 y1:2 x2:27 y2:4 },
      WallRectangle { x1:2 y1:6 x2:5 y2:7 },

      WallRectangle { x1:14 y1:6.2 x2:15 y2:10 },
      WallRectangle { x1:10 y1:6 x2:19 y2:7 },
      WallBlackLine { x1:14 y1:7 x2:15 y2:7 },

      WallRectangle { x1:7.5 y1:9 x2:12 y2:10 },
      WallRectangle { x1:7 y1:6 x2:8 y2:13 },
      WallBlackLine { x1:8 y1:9 x2:8 y2:10 },

      WallRectangle { x1:17 y1:9 x2:21.5 y2:10 },
      WallRectangle { x1:21 y1:6 x2:22 y2:13 },
      WallBlackLine { x1:21 y1:9 x2:21 y2:10 },

      WallRectangle { x1:24 y1:6 x2:27 y2:7 },

      WallRectangle { x1:-1 y1:9 x2:5 y2:13 },
      WallRectangle { x1:24 y1:9 x2:MazeData.GRID_SIZE + 1 y2:13 },
      WallBlackLine { x1:0 y1:13 x2:0 y2:15  },
      WallBlackLine { x1:MazeData.GRID_SIZE y1:13 x2:MazeData.GRID_SIZE y2:15},

      //cage and the gate
      WallRectangle { x1:10 y1:12 x2:19 y2:17 },
      WallRectangle { x1:10.5 y1:12.5 x2:18.5 y2:16.5 },
      Rectangle {
        x: MazeData.calcGridX(13)
        width: MazeData.GRID_GAP * 3
        y: MazeData.calcGridY(12)
        height: MazeData.GRID_GAP / 2
        stroke: Color.GREY
        fill: Color.GREY
      },

      WallRectangle { x1:7.5 y1:19 x2:12 y2:20 },
      WallRectangle { x1:7 y1:15 x2:8 y2:23 },
      WallBlackLine { x1:8 y1:19 x2:8 y2:20 },

      WallRectangle { x1:17 y1:19 x2:21.5 y2:20 },
      WallRectangle { x1:21 y1:15 x2:22 y2:23 },
      WallBlackLine { x1:21 y1:19 x2:21 y2:20 },

      WallRectangle { x1:14 y1:19 x2:15 y2:27 },
      WallRectangle { x1:10 y1:22 x2:19 y2:23 },
      WallBlackLine { x1:14 y1:22 x2:15 y2:22 },
      WallBlackLine { x1:14 y1:23 x2:15 y2:23 },

      WallRectangle { x1:2 y1:25 x2:5 y2:27 },
      WallRectangle { x1:17 y1:25 x2:22 y2:27 },

      WallRectangle { x1:7 y1:25 x2:12 y2:27 },
      WallRectangle { x1:24 y1:25 x2:27 y2:27 },

      WallRectangle { x1:-1 y1:15 x2:5 y2:17 },
      WallRectangle { x1:4 y1:19 x2:5 y2:23 },
      WallRectangle { x1:2 y1:19 x2:4.5 y2:20 },
      WallBlackRectangle { x1:4 y1:19.05 x2:5 y2:20.2 },
      WallRectangle { x1:-1 y1:22 x2:2 y2:23 },

      WallRectangle { x1:24 y1:15 x2:MazeData.GRID_SIZE + 1 y2:17 },
      WallRectangle { x1:24 y1:19 x2:25 y2:23 },
      WallRectangle { x1:24.5 y1:19 x2:27 y2:20 },
      WallBlackRectangle { x1:24 y1:19.05 x2:25 y2:20.2 },
      WallRectangle { x1:27 y1:22 x2:MazeData.GRID_SIZE + 1 y2:23 },

      WallBlackRectangle { x1:-2 y1:8 x2:0 y2:MazeData.GRID_SIZE },
      WallBlackRectangle {
         x1:MazeData.GRID_SIZE
         y1:8
         x2:MazeData.GRID_SIZE + 2
         y2:MazeData.GRID_SIZE
      },

      Rectangle {
        x: MazeData.calcGridX(-0.5)
        y: MazeData.calcGridY(-0.5)
        width: (MazeData.GRID_SIZE + 1) * MazeData.GRID_GAP
        height: (MazeData.GRID_SIZE + 1) * MazeData.GRID_GAP
        strokeWidth: MazeData.GRID_STROKE
        stroke: Color.BLUE
        fill: null
        arcWidth: 12
        arcHeight: 12
      },
      Line {
        startX: MazeData.calcGridX(-0.5)
        endX: MazeData.calcGridX(-0.5)
        startY: MazeData.calcGridY(13)
        endY: MazeData.calcGridY(15)
        stroke: Color.BLACK
        strokeWidth: MazeData.GRID_STROKE + 1
      },
      Line {
        startX: MazeData.calcGridX(MazeData.GRID_SIZE + 0.5)
        endX: MazeData.calcGridX(MazeData.GRID_SIZE + 0.5)
        startY: MazeData.calcGridY(13)
        endY: MazeData.calcGridY(15)
        stroke: Color.BLACK
        strokeWidth: MazeData.GRID_STROKE + 1
      },
      Line {
        startX: MazeData.calcGridX(-0.5)
        endX: MazeData.calcGridX(0)
        startY: MazeData.calcGridY(13)
        endY: MazeData.calcGridY(13)
        stroke: Color.BLUE
        strokeWidth: MazeData.GRID_STROKE
      },
      Line {
        startX: MazeData.calcGridX(-0.5)
        endX: MazeData.calcGridX(0)
        startY: MazeData.calcGridY(15)
        endY: MazeData.calcGridY(15)
        stroke: Color.BLUE
        strokeWidth: MazeData.GRID_STROKE
      },
      Line {
        startX: MazeData.calcGridX(MazeData.GRID_SIZE + 0.5)
        endX: MazeData.calcGridX(MazeData.GRID_SIZE)
        startY: MazeData.calcGridY(13)
        endY: MazeData.calcGridY(13)
        stroke: Color.BLUE
        strokeWidth: MazeData.GRID_STROKE
      },
      Line {
        startX: MazeData.calcGridX(MazeData.GRID_SIZE + 0.5)
        endX: MazeData.calcGridX(MazeData.GRID_SIZE)
        startY: MazeData.calcGridY(15)
        endY: MazeData.calcGridY(15)
        stroke: Color.BLUE
        strokeWidth: MazeData.GRID_STROKE
      },
      Text {
        font: Font {
              size: 20
              }
        x: MazeData.calcGridX(0), 
        y: MazeData.calcGridY(MazeData.GRID_SIZE + 2)
        content: bind "SCORES: {pacMan.scores} "
        fill: Color.YELLOW
      },
      scoreText,
      dyingPacMan,
      livesImage,
      gameResultText,
      Text { 
        font: Font { size: 20
                    }
        x: MazeData.calcGridX(22)
        y: MazeData.calcGridY(MazeData.GRID_SIZE + 2)
        content: bind "LEVEL: {level}"
        fill: Color.YELLOW
      },

    ]
  }; // end Group

  // put dots into the maze
  postinit {
    putDotHorizontally(2,13,1);
    putDotHorizontally(16,27,1);
    putDotHorizontally(2,27,5);
    putDotHorizontally(2,27,28);

    putDotHorizontally(2,13,24);
    putDotHorizontally(16,27,24);

    putDotHorizontally(2,5,8);
    putDotHorizontally(9,13,8);
    putDotHorizontally(16,20,8);
    putDotHorizontally(24,27,8);

    putDotHorizontally(2,5,18);
    putDotHorizontally(9,13,21);
    putDotHorizontally(16,20,21);
    putDotHorizontally(24,27,18);

    putDotHorizontally(2,3,21);
    putDotHorizontally(26,27,21);

    putDotVertically(1,1,8);
    putDotVertically(1,18,21);
    putDotVertically(1,24,28);

    putDotVertically(28,1,8);
    putDotVertically(28,18,21);
    putDotVertically(28,24,28);

    putDotVertically(6,2,27);
    putDotVertically(23,2,27);

    putDotVertically(3,22,23);
    putDotVertically(9,22,23);
    putDotVertically(20,22,23);
    putDotVertically(26,22,23);

    putDotVertically(13,25,27);
    putDotVertically(16,25,27);

    putDotVertically(9,6,7);
    putDotVertically(20,6,7);

    putDotVertically(13,2,4);
    putDotVertically(16,2,4);

    insert pacMan into group.content;

    insert ghosts into group.content;

    insert WallBlackRectangle{ x1:-3, y1:13, x2:0, y2:15} into group.content;
    insert WallBlackRectangle{ x1:29, y1:13, x2:31, y2:15} into group.content;

    insert messageBox into group.content;
  }

  public override function create(): Node {
    return group;
  } // end create()

  public override var onKeyPressed = function ( e: KeyEvent ) : Void {

    // wait for the player's keyboard input to start the game
    if ( waitForStart ) {
      waitForStart = false;
      startNewGame();
      return;
    }

    if ( e.code == KeyCode.VK_DOWN )
      pacMan.setKeyboardBuffer( pacMan.MOVE_DOWN )
    else
      if ( e.code == KeyCode.VK_UP )
        pacMan.setKeyboardBuffer( pacMan.MOVE_UP )
      else
        if ( e.code == KeyCode.VK_RIGHT )
          pacMan.setKeyboardBuffer( pacMan.MOVE_RIGHT )
        else
          if ( e.code == KeyCode.VK_LEFT )
            pacMan.setKeyboardBuffer( pacMan.MOVE_LEFT );
  }


  // create a Dot GUI object
  public function createDot( x1: Number,  y1:Number, type:Integer ): Dot {
    var d = Dot {
      x: MazeData.calcGridX(x1)
      y: MazeData.calcGridY(y1)
      dotType: type
      visible: true
      }

    if ( d.dotType == MazeData.MAGIC_DOT )
      d.playTimeline();

    // set the dot type in data model
    MazeData.setData( x1, y1, type );

    // set dot reference
    MazeData.setDot( x1, y1, d );

    return d;
  }

  // put dots into the maze as a horizontal line
  public function putDotHorizontally(x1: Integer, x2: Integer, y: Number ) {

    var dots =
    for ( x in [ x1..x2] )
    if ( MazeData.getData(x,y) == MazeData.EMPTY ) {
      var dotType: Integer;

      if ( (x == 28 or x == 1) and (y == 3 or y == 26) )
        dotType = MazeData.MAGIC_DOT
      else
        dotType = MazeData.NORMAL_DOT;

      createDot( x, y, dotType )
    }
    else   [] ;

    insert dots into group.content;
  }

  // put dots into the maze as a vertical line
  public function putDotVertically(x: Integer, y1: Integer, y2: Number ) {

    var dots =
    for ( y in [ y1..y2] )
    if ( MazeData.getData(x,y) == MazeData.EMPTY ) {
      var dotType: Integer;

      if ( (x == 28 or x == 1) and (y == 3 or y == 26) )
        dotType = MazeData.MAGIC_DOT
      else
        dotType = MazeData.NORMAL_DOT;

      createDot( x, y, dotType )
    }
    else  [];

    insert dots into group.content;
  }

  public function makeGhostsHollow() {
 
    ghostEatenCount = 0;

    for ( g in ghosts )
      g.changeToHollowGhost();
  }

  // determine if pacman meets a ghost
  public function hasMet(g:Ghost): Boolean {

    def distanceThreshold = 22;

    var x1 = g.imageX;
    var x2 = pacMan.imageX;

    var diffX = Math.abs(x1-x2);

    if ( diffX >= distanceThreshold ) return false;

    var y1 = g.imageY;
    var y2 = pacMan.imageY;
    var diffY = Math.abs(y1-y2);

    if ( diffY >= distanceThreshold ) return false;

    // calculate the distance to see if pacman touches the ghost
    if ( diffY*diffY + diffX*diffX <= distanceThreshold*distanceThreshold )
      return true;

    return false;
  }

  public function pacManMeetsGhosts() {

    for ( g in ghosts )
      if ( hasMet(g) )
        if ( g.isHollow ) {
          pacManEatsGhost(g);
        }
        else {
          for ( ghost in ghosts )
            ghost.stop();

          pacMan.stop();

          dyingPacMan.startAnimation(pacMan.imageX, pacMan.imageY);
          break;
        }
  }

  public function pacManEatsGhost(g: Ghost) {
      
    ghostEatenCount++;

    var s = 1;
    for ( i in [1..ghostEatenCount] ) s = s + s;

    pacMan.scores += s*100;
    if ( addLifeFlag and pacMan.scores >= 10000 ) {
      addLife();
    }

    var st = scoreText[ghostEatenCount-1];
    st.x = g.imageX - 10;
    st.y = g.imageY;

    g.stop();
    g.resetStatus();
    g.trapCounter = -10;

    st.showText();
  
  }

  // reset status and start a new game
  public function startNewGame() {

    messageBox.visible = false;
    pacMan.resetStatus();

    gameResultText.visible = false;

    if ( lastGameResult == false ) {
      level = 1;
      addLifeFlag = true;
      pacMan.scores = 0;
      pacMan.dotEatenCount = 0;

      livesCount = 2;
    }
    else { 
      lastGameResult = false;
      level ++;
    }

    for ( x in [1..MazeData.GRID_SIZE] )
      for ( y in [1..MazeData.GRID_SIZE] ) {
        var dot : Dot = MazeData.getDot( x, y ) as Dot ;

        if ( dot != null and not dot.visible ) 
          dot.visible = true;
      }

    for ( g in ghosts ) {
      g.resetStatus();
    }
  }

  // reset status and start a new level
  public function startNewLevel() {

    lastGameResult = true;

    pacMan.hide();
    pacMan.dotEatenCount = 0;

    for ( g in ghosts ) {
      g.hide();
    }

    flashingCount = 0;
    flashingTimeline.playFromStart();
  }

  // reset status and start a new life
  public function startNewLife() {

    // reduce a life of Pac-Man
    if ( livesCount > 0 ) {
      livesCount--;
    }
    else {
      lastGameResult = false;
      flashingCount = 0;
      flashingTimeline.playFromStart();
      return;
    }

    pacMan.resetStatus();

    for ( g in ghosts ) {
      g.resetStatus();
    }
  }

  public function addLife():Void {

    if ( addLifeFlag ) {
      livesCount ++;
      addLifeFlag = false;
    }
  }

}
