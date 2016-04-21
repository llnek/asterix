/*
 * Ghost.fx
 *
 * Created on 2009-1-28, 14:26:09
 */

package pacman;

import java.lang.Math;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.CustomNode;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.Node;
import pacman.MazeData;

/**
 * @author Henry Zhang
 */

public class Ghost extends CustomNode, MovingObject{

  public def TRAPPED=10;

  // the pacman character
  public var pacMan: PacMan;

  public var hollowImage1 = Image {
    url: "{__DIR__}images/ghosthollow2.png"
    }
  public var hollowImage2 = Image {
    url: "{__DIR__}images/ghosthollow3.png"
    }
  public var hollowImage3 = Image {
    url: "{__DIR__}images/ghosthollow1.png"
    }

  // images for ghosts when they become hollow
  public var hollowImg =
    [ hollowImage1,
      hollowImage2,
      hollowImage1,
      hollowImage2 ];

  // images for ghosts when they become hollow and flashing
  public var flashHollowImg =
    [ hollowImage1,
      hollowImage3,
      hollowImage1,
      hollowImage3 ];

  // time for a ghost to stay hollow
  var hollowMaxTime: Integer = 80;
  var hollowCounter : Integer;

  // the images of animation
  public var defaultImage1: Image;
  public var defaultImage2: Image;

  def  defaultImg  = [
        defaultImage1,
        defaultImage2,
        defaultImage1,
        defaultImage2,
  ];

  // animation images
  var images = defaultImg;

  // initial direction and position of a ghost, used in status reset
  public var initialLocationX : Number;
  public var initialLocationY : Number;
  public var initialDirectionX : Number;
  public var initialDirectionY : Number;

  // time to stay in the cage
  public var trapTime: Integer;
  public var trapCounter: Integer=0;

  // variables to determine if a ghost should chase pacman,
  // and the probability
  public var changeFactor = 0.75;
  public var chaseFactor = 0.5;
  public var chaseCount = 0;

  // the flag is set if a ghost becomes hollow
  public var isHollow: Boolean = false;

  // the GUI of a ghost
  var ghostNode : ImageView = ImageView {
    x: bind imageX  - 13
    y: bind imageY  - 13
    image: bind images[currentImage]
    
    }

  postinit {
    initialLocationX = x;
    initialLocationY = y;
    initialDirectionX = xDirection;
    initialDirectionY = yDirection;

    imageX = MazeData.calcGridX(x);
    imageY = MazeData.calcGridY(y);
    
 //   resetStatus();
  }

  // reset the status of a ghost and place it into the cage
  public function resetStatus() {
    x = initialLocationX;
    y = initialLocationY;

    xDirection = initialDirectionX;
    yDirection = initialDirectionY;

    isHollow = false;

    moveCounter = 0;
    trapCounter = 0;
    currentImage = 0;

    imageX = MazeData.calcGridX(x);
    imageY = MazeData.calcGridY(y);

    images = defaultImg;
    state = TRAPPED;
        
    timeline.keyFrames[0].time = 50ms;

    visible = true;
    start();
  }

  public function changeToHollowGhost() {
    hollowCounter = 0;
    isHollow = true;

    // switch the animation images
    images = hollowImg;

    // make it moves slower
    timeline.stop();
    timeline.keyFrames[0].time = 140ms;
    timeline.play();
  }

  // decide whether to change the current direction of a ghost
  public function changeDirectionXtoY(mustChange: Boolean): Void {
    if ( not mustChange and Math.random() > changeFactor ) {
      return;  // no change of direction
    }

    // will change to a Y direction if possible
    var goUp = MoveDecision {
      x: this.x
      y: this.y - 1 };
    var goDown = MoveDecision {
      x: this.x
      y: this.y + 1
    };

    // evaluate the moving choices to pick the best one
    goUp.evaluate(pacMan, isHollow);
    goDown.evaluate(pacMan, isHollow);

    if ( goUp.score < 0 and goDown.score < 0 )
      return;  // no change of direction

    if ( Math.random() < chaseFactor and chaseCount == 0 )
      chaseCount += Math.random() * 10 + 3;

    var continueGo =  MoveDecision {
      x: this.x + xDirection
      y: this.y
    };

    continueGo.evaluate(pacMan, isHollow);

    if ( continueGo.score > 0 and continueGo.score > goUp.score
         and continueGo.score > goDown.score and chaseCount>0) {
      chaseCount--;
      return;
    }

    var decision = -1; // make it goes up first, then decide if we need to change it
    if ( goUp.score  < 0 )
      decision = 1
    else
      if ( goDown.score > 0 ) {
        if ( chaseCount > 0 ) {
          if ( goDown.score > goUp.score) {
            decision = 1;
            chaseCount -- ;
          }
        }
        else {
          // random pick
          if ( Math.random() > 0.5 )
            decision = 1;
        }
      }

    yDirection = decision;
    xDirection = 0;
  }

  // decide whether to change the current direction of a ghost
  public function changeDirectionYtoX(mustChange: Boolean): Void {

    if ( not mustChange and Math.random() > changeFactor )
      return;  // no change of direction

    // will change to X directions if possible
    var goLeft = MoveDecision {
      x: this.x - 1
      y: this.y
    };

    var goRight = MoveDecision {
      x: this.x + 1
      y: this.y
    };

    // evaluate the moving choices to pick the best one
    goLeft.evaluate(pacMan, isHollow);
    goRight.evaluate(pacMan, isHollow);

    if ( goLeft.score < 0 and goRight.score < 0 ) {
      return;  // no change of direction
    }

    if ( Math.random() < chaseFactor and chaseCount == 0 )
      chaseCount += Math.random() * 10 + 3;

    var continueGo = MoveDecision {
      x: this.x
      y: this.y + yDirection
    };

    continueGo.evaluate(pacMan, isHollow);

    if ( continueGo.score > 0 and continueGo.score > goLeft.score
         and continueGo.score > goRight.score and chaseCount>0 ) {
      chaseCount --;
      return;
    }

    // make it goes up first, then decide if we need to change it to down
    var decision = -1;
    if ( goLeft.score  < 0 )
      decision = 1
    else
      if ( goRight.score > 0 ) {
        if ( chaseCount > 0 ) {
          if ( goRight.score > goLeft.score) {
            decision = 1;
            chaseCount--;
          }
        }
        else { // random pick
          if ( Math.random() > 0.5 )
            decision = 1;
        }
      }

    xDirection=decision;
    yDirection = 0;
  }

  // move the ghost horizontally
  public function moveHorizontally() {

    moveCounter++;

    if ( moveCounter > ANIMATION_STEP - 1) {
      moveCounter=0;
      x += xDirection;
      imageX= MazeData.calcGridX(x);

      var nextX = xDirection + x;

      if ( y == 14 and ( nextX <= 1 or nextX >= 28) ) {
        if ( nextX < - 1 and xDirection < 0 ) {
          x=MazeData.GRID_SIZE;
          imageX= MazeData.calcGridX(x);
        }
        else
          if ( nextX > 30 and xDirection > 0) {
            x=0;
            imageX= MazeData.calcGridX(x);
          }
      }
      else
        if (nextX < 0 or nextX > MazeData.GRID_SIZE) {
          changeDirectionXtoY(true)
        }
      else
        if ( MazeData.getData(nextX, y) == MazeData.BLOCK ) {
          changeDirectionXtoY(true)
        }
        else {
          changeDirectionXtoY(false);
        }
    }
    else {
      imageX += xDirection * MOVE_SPEED;
    }
  }

  // move the ghost vertically
  public function moveVertically() {
      
    moveCounter++;

    if ( moveCounter > ANIMATION_STEP - 1) {
      moveCounter = 0;
      y += yDirection;
      imageY = MazeData.calcGridX(y);

      var nextY= yDirection + y;
      if ( nextY < 0 or nextY > MazeData.GRID_SIZE) {
        changeDirectionYtoX(true);
      }
      else
        if ( MazeData.getData(x, nextY) == MazeData.BLOCK ) {
          changeDirectionYtoX(true);
        }
        else {
          changeDirectionYtoX(false);
        }
    }
    else {
      imageY += yDirection * MOVE_SPEED;
    }
  }

  // move the ghost horizontally in the cage
  public function moveHorizontallyInCage() {
    
    moveCounter++;

    if ( moveCounter > ANIMATION_STEP - 1) {

      moveCounter=0;
      x += xDirection;
      imageX = MazeData.calcGridX(x);

      var nextX = xDirection + x;

      if ( nextX < 12 ) {
        xDirection = 0;
        yDirection = 1;
      }
      else
        if ( nextX > 17) {
          xDirection = 0;
          yDirection = -1;
        }
    }
    else {
      imageX += xDirection * MOVE_SPEED;
    }
  }

  // move the ghost vertically in a cage
  public function moveVerticallyInCage() {

    moveCounter++;

    if ( moveCounter > ANIMATION_STEP - 1) {
      moveCounter=0;
      y += yDirection;
      imageY= MazeData.calcGridX(y) + 8;

      var nextY = yDirection + y;

      if ( nextY < 13 ) {
        yDirection = 0;
        xDirection = -1;
      }
      else
        if ( nextY > 15) {
          yDirection = 0;
          xDirection = 1;
        }
    }
    else {
      imageY += yDirection * MOVE_SPEED;
    }
  }

  public function hide() {
    visible=false;
    timeline.stop();
  }

  // move one tick
  public override function moveOneStep() {

    if ( state == MOVING or state == TRAPPED ) {
      if ( xDirection != 0 ) {
        if ( state == MOVING )
          moveHorizontally()
        else
          moveHorizontallyInCage();
      }
      else
        if ( yDirection != 0 ) {
          if ( state == MOVING )
            moveVertically()
          else
            moveVerticallyInCage();
        }

      if ( currentImage < ANIMATION_STEP - 1 )
        currentImage++
      else {
        currentImage=0;
        if ( state == TRAPPED ) { 
          trapCounter++;

          if ( trapCounter > trapTime and x == 14 and y == 13) {
            // go out of the cage
            y = 12;

            xDirection = 0;
            yDirection = -1;
            state = MOVING;
          }
        }
      }
    }

    // check to see if need to switch back to a normal status
    if ( isHollow ) {
    
      hollowCounter++;

      if ( hollowCounter == hollowMaxTime - 30 )
        images = flashHollowImg
      else
        if ( hollowCounter > hollowMaxTime ) {
          isHollow = false;
          images = defaultImg;
                
          timeline.stop();
          timeline.keyFrames[0].time = 50ms;
          timeline.play();
        }
    }
  }

  public override function create(): Node {
        return ghostNode;
  }

}
