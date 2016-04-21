/*
 * WallBlackLine.fx
 *
 * Created on 2008-12-27, 17:52:58
 */

package pacman;

import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import pacman.MazeData;

/**
 * @author Henry Zhang
 */

public class WallBlackLine extends Line {

  public var x1: Number;
  public var y1: Number;
  public var x2: Number;
  public var y2: Number;

  postinit {
    strokeWidth = MazeData.GRID_STROKE + 1;
    stroke = Color.BLACK;
        
    if ( x1 == x2 ) { // vertically line
      startX = MazeData.calcGridX(x1);
      startY = MazeData.calcGridY(y1) + MazeData.GRID_STROKE;
      endX = MazeData.calcGridX(x2);
      endY = MazeData.calcGridY(y2) - MazeData.GRID_STROKE;
    }
    else  { // horizontal line
      startX = MazeData.calcGridX(x1) + MazeData.GRID_STROKE;
      startY = MazeData.calcGridY(y1);
      endX = MazeData.calcGridX(x2) - MazeData.GRID_STROKE;
      endY = MazeData.calcGridY(y2);
    }
  } // end postinit
}
