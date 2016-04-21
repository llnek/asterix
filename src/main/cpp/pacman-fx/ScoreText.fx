/*
 * ScoreText.fx
 *
 * Created on 2009-2-6, 17:52:42
 *
 * text object for showing scores of eating a ghost, then disappears after 2s
 *
 */

package pacman;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.scene.paint.Color;

/**
 * @author Henry Zhang
 */

public class ScoreText extends Text{
  override var font = Font { size: 11 };

  override var fill = Color.YELLOW;

  var timeline= Timeline {
    repeatCount: 1
    keyFrames: [
      KeyFrame {
        time: 2s
        action: function() {
          visible = false;
        }
      }
    ]
    };


  public function showText() {
    visible = true;
    timeline.stop();
    timeline.play();
  }
}
