/*
 * Cell.fx
 *
 * Created on 2009-4-8, 0:58:05
 */

package minesweeper;

import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Polygon;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;

/**
 * @author Administrator
 */

package class Cell extends CustomNode{
    package var x: Integer = 0;
    package var y: Integer = 0;
    package var cellLength: Integer = 0;
    

    package var index: Integer = 0;
    package var isMine: Boolean = false;
    package var isMineExploded: Boolean = false;
    package var isCover: Boolean  = true;
    package var isNum: Boolean = false;
    package var isMarked: Boolean = false;
    package var isBlank: Boolean = true;
    package var number: Integer = 0;
    


    bound function getPaint():Paint{
        if(isMarked and isCover){
            Color.rgb(221,119,39);
            //Color.web("#a0c4f1")


        }else if(isCover){
            Color.rgb(221,119,39);
        }else if(isMine){
            if(isMineExploded){
                Color.RED
            }else {
                //Color.WHITE
                Color.rgb(0, 0, 0, 0)
            }
        }else if(isNum){
            //            Color.PINK
            Color.rgb(0, 0, 0, 0)
        }else {
            Color.rgb(0, 0, 0, 0)
        }
    }

    bound function getTextPaint():Paint{
        [Color.LIGHTSKYBLUE,Color.LIGHTPINK,Color.LIGHTYELLOW,Color.LIGHTCYAN,Color.GREEN,Color.PURPLE,Color.BEIGE,Color.CORAL][
        number - 1]
    }



    var mineNode = Group {//show mine
        visible: bind isMine and not isCover
        content: [
            Circle{
                centerX: x + cellLength / 2
                centerY: y + cellLength / 2
                radius: cellLength / 2 - 8
                fill: Color.BLACK
            }
            Rectangle{
                x: x + cellLength / 2 - 1
                y: y + 4
                width: 2
                height: cellLength / 2
            }
            Rectangle{
                x: x + 10
                y: y + 5
                width: 2
                height: 2
            }
        ]
    }

    var bgNode = Rectangle {
        x: x,
        y: y,
        height: cellLength
        width: cellLength
        fill: bind getPaint()
        stroke: Color.BLACK
        strokeWidth: 1
        smooth: false
    }

    var flagNode = Polygon { //show flag when mark cell
        points: [ x + cellLength / 2, y + 5, x + cellLength - 5, y + cellLength / 2, x + cellLength / 2 + 1, y + cellLength / 2,x + cellLength / 2 + 1, y + cellLength - 5,x + cellLength / 2, y + cellLength - 5 ]
        fill: Color.RED
        visible: bind isMarked and isCover
    }
    var numberNode = Text{
        x: x + 9
        y: y + 20
        visible: bind isNum and not isCover
        fill: Color.BLACK
        content: bind "{number}"
        font: Font {
            size: 18
            name: "Arial Bold"
        }
    }
    public override function create(): Node {
        //getNode();
        Group {            
            content: [
                bgNode
                flagNode
                mineNode
                numberNode
//                getRectangle()
                //ImageView {
//                    image: bind makeImage(getImageName()),
//                    x: bind x,
//                    y: bind y,
//                }
                //
                // A "glass pane".
                //
//                Rectangle {
//                    x: bind x
//                    y: bind y
//                    height: cellLength
//                    width: cellLength
//                    //fill: Color.web("#a0c4f1")
//                    fill: Color.rgb(0, 0, 0, 0)
//                    //stroke: Color.BLACK
//                    //strokeWidth: 1
//                    smooth: false
//                    //arcWidth: 5  arcHeight: 5
//                },
            ]
        }
    }
}
