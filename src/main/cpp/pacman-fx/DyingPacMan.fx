/*
 * DyingPacMan.fx
 *
 * Created on 2009-2-6, 17:52:42
 */

package pacman;

import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.shape.Arc;

/**
 * @author Henry Zhang
 */

public class DyingPacMan extends Arc {

  public var maze : Maze;

  var timeline = Timeline {
    repeatCount: 1
    keyFrames: [
    
      KeyFrame {
        time: 600ms
        action: function() {
          // hide the pacMan character and ghosts before the animation
          maze.pacMan.visible = false;
          
          for ( g in maze.ghosts ) {
            g.hide();
          }

          visible = true;
        }
        values: [ startAngle => 90, length=>360 ];  
      },

      KeyFrame {
        time: 1800ms
        action: function() {
          visible = false;
          maze.startNewLife();
         }
        values: [ startAngle => 270 tween Interpolator.LINEAR,
                  length => 0 tween Interpolator.LINEAR ]
      },
    ]
  }

  public function startAnimation(x: Number, y: Number) : Void {

    startAngle = 90;
    centerX = x;
    centerY = y;

 //   timeline.stop();
    timeline.playFromStart();
  }
}
