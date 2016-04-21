/*
 * PacMan.fx
 *
 * Created on 2009-1-1, 11:50:58
 */

package pacman;

import javafx.scene.CustomNode;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.Node;
import javafx.scene.transform.Rotate;
import pacman.MazeData;
import pacman.MovingObject;

/**
 * @author Henry Zhang
 */

public class PacMan extends CustomNode, MovingObject {

  public var defaultImage: Image = Image {
    url: "{__DIR__}images/left1.png"
  };

  // images for animation
  def images = [
    defaultImage,
    Image {
      url: "{__DIR__}images/left2.png"
    },
    defaultImage,
    Image {
      url: "{__DIR__}images/round.png"
    }
  ];

  // the number of dots eaten
  public var dotEatenCount : Integer = 0;

  // scores of the game
  public var scores: Integer = 0;
    
  // angles of rotating the images
  def rotationDegree = [0, 90, 180, 270];

  // GUI image of the man
  var pacmanImage : ImageView = ImageView {
    x: bind imageX - 13
    y: bind imageY - 13
    image: bind images[currentImage]
    transforms: Rotate {
      angle: bind rotationDegree[currentDirection]
      pivotX: bind imageX
      pivotY: bind imageY
      }
   }

  // buffer to keep the keyboard input
  var keyboardBuffer: Integer = -1;

  // current direction of Pacman
  var currentDirection: Integer = MOVE_LEFT;

  postinit {
    imageX = MazeData.calcGridX(x);
    imageY = MazeData.calcGridX(y);
    
    xDirection = -1;
    yDirection = 0;

  }


  public override function create(): Node {
    return pacmanImage;
  }

  // moving horizontally
  public function moveHorizontally() {

    moveCounter++;

    if ( moveCounter < ANIMATION_STEP ) {
      imageX += xDirection * MOVE_SPEED;
    }
    else {
      moveCounter = 0;
      x += xDirection;
      
      imageX = MazeData.calcGridX(x);

      // the X coordinate of the next point in the grid
      var nextX = xDirection + x;

      if ( y == 14 and ( nextX <= 1 or nextX >= 28) ) {
        if ( nextX < - 1 and xDirection < 0 ) {
          x = MazeData.GRID_SIZE;
          imageX = MazeData.calcGridX(x);
        }
        else
        if ( nextX > 30 and xDirection > 0) {
          x = 0;
          imageX = MazeData.calcGridX(x);
        }
      }
      else // check if the character hits a wall
      if ( MazeData.getData(nextX, y) == MazeData.BLOCK ) {
        state = STOP;
      }
    }
  }

  // moving vertically
  public function moveVertically() {
      
    moveCounter++;

    if ( moveCounter < ANIMATION_STEP ) {
      imageY += yDirection * MOVE_SPEED;
    }
    else {
      moveCounter = 0;
      y += yDirection;
      imageY = MazeData.calcGridX(y);

      // the Y coordinate of the next point in the grid
      var nextY = yDirection + y;

      // check if the character hits a wall
      if ( MazeData.getData(x, nextY) == MazeData.BLOCK ) {
        state = STOP;
      }
    }
  }

  // turn pac-man to the right
  public function moveRight(): Void {

    if ( currentDirection == MOVE_RIGHT )  return;

    var nextX = x + 1;

    if ( nextX >= MazeData.GRID_SIZE)  return;

    if ( MazeData.getData(nextX, y) == MazeData.BLOCK )   return;

    xDirection = 1;
    yDirection = 0;

    keyboardBuffer = -1;
    currentDirection = MOVE_RIGHT;

    state = MOVING;
  }

  // turn pac-man to the left
  public function moveLeft(): Void {

    if ( currentDirection == MOVE_LEFT )   return;

    var nextX = x - 1;

    if ( nextX <= 1)  return;

    if ( MazeData.getData(nextX, y) == MazeData.BLOCK )   return;

    xDirection = -1;
    yDirection = 0;

    keyboardBuffer = -1;
    currentDirection = MOVE_LEFT;

    state = MOVING;
  }

  // turn pac-man going up
  public function moveUp(): Void {

    if ( currentDirection == MOVE_UP )  return;

    var nextY = y - 1;

    if ( nextY <= 1)   return;

    if ( MazeData.getData(x,nextY) == MazeData.BLOCK )  return;

    xDirection = 0;
    yDirection = -1;

    keyboardBuffer = -1;
    currentDirection = MOVE_UP;

    state = MOVING;
  }

  // turn pac-man going down
  public function moveDown(): Void {

    if ( currentDirection == MOVE_DOWN ) return;

    var nextY = y + 1;

    if ( nextY >= MazeData.GRID_SIZE )  return;

    if ( MazeData.getData(x,nextY) == MazeData.BLOCK )  return;

    xDirection = 0;
    yDirection = 1;

    keyboardBuffer = -1;
    currentDirection = MOVE_DOWN;

    state = MOVING;
  }

  // handle keyboard input
  public function handleKeyboardInput(): Void {
    if ( keyboardBuffer < 0)
    return;

    if ( keyboardBuffer == MOVE_LEFT )
      moveLeft()
    else
      if ( keyboardBuffer == MOVE_RIGHT )
        moveRight()
      else
        if ( keyboardBuffer == MOVE_UP )
          moveUp()
        else
          if ( keyboardBuffer == MOVE_DOWN )
            moveDown();
  }

  public function setKeyboardBuffer( k: Integer): Void {
    keyboardBuffer = k;
  }

  // update scores if a dot is eaten
  public function updateScores() : Void {
    if ( y != 14 or ( x > 0 and x < MazeData.GRID_SIZE ) ) {
      var dot : Dot = MazeData.getDot( x, y ) as Dot ;

      if ( dot != null and dot.visible ) {
        scores += 10;
        dot.visible = false;
        dotEatenCount ++;

        if ( scores >= 10000 ) {
          maze.addLife();
        }

        if ( dot.dotType == MazeData.MAGIC_DOT ) {
          maze.makeGhostsHollow();
        }

        // check if the player wins and should start a new level
        if ( dotEatenCount >= MazeData.DOT_TOTAL )
          maze.startNewLevel();
      }
    }
  }

  public function hide() {
    visible=false;
    timeline.stop();
  }

  // handle animation of one tick
  public override function moveOneStep() {

    // handle keyboard input only when pac-man is at a point of the grid
    if ( currentImage == 0 )
      handleKeyboardInput();

    if ( state == MOVING) {
      if ( xDirection != 0 )
        moveHorizontally();

      if ( yDirection != 0 )
        moveVertically();

      // switch to the image of the next frame
      if ( currentImage < ANIMATION_STEP - 1  )
        currentImage++
      else {
        currentImage=0;
        updateScores();
      }
    }

    maze.pacManMeetsGhosts();    
  }

  // place Pac-Man at the startup position for a new game
  public function resetStatus() {
    state = MOVING;
    currentDirection = MOVE_LEFT;
    xDirection = -1;
    yDirection = 0;
    
    keyboardBuffer = -1;
    currentImage = 0;
    moveCounter = 0;

    x=15;
    y=18;

    imageX = MazeData.calcGridX(x);
    imageY = MazeData.calcGridY(y);

    visible = true;
    start();
  }
}
