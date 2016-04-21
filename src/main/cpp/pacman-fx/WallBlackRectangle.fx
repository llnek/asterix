/*
 * WallBlackRectangle.fx
 *
 * Created on 2008-12-27, 16:35:42
 */

package pacman;

import javafx.scene.CustomNode;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import pacman.MazeData;

/**
 * @author Henry Zhang
 */

public class WallBlackRectangle extends CustomNode {

  public var x1: Number;
  public var y1: Number;
  public var x2: Number;
  public var y2: Number;

  public override function create(): Node {
    Rectangle {
      x: MazeData.calcGridX(x1) + MazeData.GRID_STROKE
      y: MazeData.calcGridY(y1) + MazeData.GRID_STROKE
      width: MazeData.GRID_GAP * (x2-x1) - MazeData.GRID_STROKE * 2
      height: MazeData.GRID_GAP * (y2-y1) - MazeData.GRID_STROKE * 2
      strokeWidth: MazeData.GRID_STROKE
      stroke: Color.BLACK
      arcWidth: 3
      arcHeight: 3
    }  
  }
}
