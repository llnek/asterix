/*
 * MovingObject.fx
 *
 * Created on 2009-1-1, 11:40:49
 */

package pacman;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import pacman.Maze;
import pacman.MazeData;

/**
 * @author Henry Zhang
 */

public abstract class MovingObject {

  // animation frames total and movement distance
  public def ANIMATION_STEP=4;
  public def MOVE_SPEED = MazeData.GRID_GAP / ANIMATION_STEP;

  public def MOVING = 1;
  public def STOP =0;
  
  public def MOVE_LEFT=0;
  public def MOVE_UP=1;
  public def MOVE_RIGHT=2;
  public def MOVE_DOWN=3;

  public var maze: Maze;
  public var state : Integer;

  public var currentImage=0;
  public var moveCounter: Integer=0;

  // grid coordinates
  public var x: Number;
  public var y: Number;

  // graphical coordinates
  public var imageX: Number ;
  public var imageY: Number ;

  public var xDirection: Number = 0;
  public var yDirection: Number = 0;

  public var timeline: Timeline =  createTimeline();

  public function stop() {
    timeline.stop();
  }

  public function pause() {
    timeline.pause();
  }

  public function start() {
    timeline.play();
  }

  // animation time line, moving the pacman
  public function createTimeline(): Timeline {
    Timeline {
      repeatCount: Timeline.INDEFINITE
      keyFrames: [
        KeyFrame {
          time: 50ms
          action: function() {
            moveOneStep();
          }
        }
      ]
     }
  }

  public abstract function moveOneStep(): Void;
}
