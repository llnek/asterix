/*
 * Dot.fx
 *
 * Created on 2008-12-21, 21:59:45
 */

package pacman;

/**
 * @author Henry Zhang
 */

import java.lang.Math;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.CustomNode;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

public class Dot extends CustomNode {

  public var dotType: Integer;

  // location of the dot
  public var x : Number ;
  public var y : Number ;

  // radius of the dot
  public var r: Number =
  if (  dotType == MazeData.MAGIC_DOT ) 5 else 1;

  // the dot
  var circle = Circle{
    centerX: x
    centerY: y
    radius: bind r
    fill: Color.YELLOW
    visible: bind visible   // bind to Dot.visible
    } ;

  // variables for magic dot's growing/shrinking animation
  public var animationRadius: Number = 3;
  public var delta: Number = -1;
  var timeline: Timeline;

  // create the animation timeline for magic dot
  public function createTimeline(): Timeline {
    Timeline {
      repeatCount: Timeline.INDEFINITE
      keyFrames: [
        KeyFrame {
          time: 250ms
          action: function() {
            doOneTick();
          }
        }
      ]
        }
  }

  public function playTimeline() {
    if ( timeline == null )
    timeline = createTimeline();

    timeline.play();
  }

  // do the animation
  public function doOneTick () {

    if ( visible == false )
    return;

    animationRadius += delta;
    var x = Math.abs(animationRadius) + 3;

    if ( x > 5 ) {
      delta = -delta;
    }

    r = x;
  }

  public override function create(): Node {
        return circle;
  }

}