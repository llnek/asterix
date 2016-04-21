/*
 * Main.fx
 *
 * Created on 2009-3-29, 1:17:22
 */

package minesweeper;

import java.lang.Object;
import javafx.animation.Timeline;
import javafx.lang.FX;
import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.input.MouseEvent;
import javafx.scene.Node;
import javafx.scene.paint.Color;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.Scene;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import minesweeper.Main.TitleBar;
import minesweeper.Model;

import java.awt.Dimension;
import java.awt.Toolkit;


/**
 * @author Administrator
 */

def CellLength = 26;
def RowNum = 16;
def ColumnNum = 14;
def MineNum = 35;
public def TitleBarHeight = 21;
def StageWidth = CellLength * RowNum + 1;
def StageHeight = CellLength * ColumnNum + TitleBarHeight + 1;
var titleBar = TitleBar{
    stageX: (getScreenDimension().width-StageWidth)/2,
    stageY: (getScreenDimension().height-StageHeight)/2,
    width: StageWidth - 1,
    height: TitleBarHeight - 1
};
var model = Model{
    w: RowNum
    h: ColumnNum
    mineNum: MineNum
    cellLength: CellLength
};
var windowBGFill = LinearGradient {
    startX:0.0 startY:0.0 endX:0.6 endY:0.8
    stops: [
        Stop {
            color: Color.web("#4e95ec")
            offset: 0.0
        },
        Stop {
            color: Color.WHITE
            offset: 1.0
        }
    ]
};

var isWin = bind  model.isWin on replace oldValue{
    if(isWin) showDialog("U Win");
}

var isOver = bind model.isOver on replace oldValue{
    if(isOver) showDialog("Losed");
}


public function run() {    
    Stage {
        //title: "Mine Sweeper"
        x: bind titleBar.stageX
        y: bind titleBar.stageY
        width: StageWidth
        height: StageHeight
        resizable: false
        style: StageStyle.UNDECORATED

        scene: Scene {
            content: [              
                model,
                titleBar
            ]
            fill: windowBGFill
        }
    }
}

def DialogWidth: Integer = 200;
def DialogHeight: Integer = 120 + TitleBarHeight;
public function showDialog(textContent:String):Void{
    var stage: Stage;
    var titleBar = TitleBar{
        stageX: (getScreenDimension().width - DialogWidth) / 2,
        stageY: (getScreenDimension().height - DialogHeight) / 2,
        width: DialogWidth - 1,
        height: TitleBarHeight - 1
        onClosed: function( e: MouseEvent ):Void {
            stage.close();
            model.reset();
        }
    };
    

    stage = Stage{
    
        x: bind titleBar.stageX
        y: bind titleBar.stageY
        var scene = Scene{
            content: [
                titleBar,
                Text {
                    x: 60
                    y: 60 + TitleBarHeight
                    rotate: -20
                    fill: Color.BLACK
                    font: Font {
                        name: "Arial Bold"
                        size: 25
                    }
                    content: textContent
                }
            ]
            
            fill: windowBGFill
        }
        scene: scene
        width: DialogWidth
        height: DialogHeight
        title: "WIN"
        resizable: false
        style: StageStyle.UNDECORATED
    }

    displayStage(stage);
}

function displayStage(f:Stage):Void{
    // Simple effect to make the stage to be opened smoothly
    def displayStage = Timeline {
        keyFrames: [
            at (0s) {f.visible => true}
            at (0s) {f.scene.content[0].opacity => 0.0}
            at (1s) {f.scene.content[0].opacity => 1.0}
        ]
    }
    displayStage.play();
}

public function getScreenDimension():Dimension{
        var toolkit = Toolkit.getDefaultToolkit();
        return toolkit.getScreenSize();
}

class TitleBar extends CustomNode{
    var stageX = 0;
    var stageY = 0;
    var height = 0;
    var width = 0;
    var onClosed = function( e: MouseEvent ):Void {FX.exit();}
    public override function create():Node{
        Group{
            content: [
                Rectangle {//drag bar
                    x: 0,
                    y: 0
                    width: width,
                    height: height
                    fill: LinearGradient {
                        startX: 0.0
                        startY: 0.0
                        endX: 0.0
                        endY: 1.0
                        stops: [
                            Stop {
                                color: Color.web("#4e95ec")
                                //color: Color.GRAY
                                offset: 0.0
                            },
                            Stop {
                                color: Color.WHITE
                                //color: Color.BLACK
                                offset: 1.0
                            },

                        ]
                    }
                    stroke: Color.BLACK
                    strokeWidth: 1
                    opacity: 0.9
                    onMouseDragged: function( e: MouseEvent ):Void {
                        stageX += e.dragX;
                        stageY += e.dragY;
                    }
                },



                Rectangle {//close button
                    x: width - 25,
                    y: 3
                    arcHeight: 5
                    arcWidth: 5
                    width: 15,
                    height: 15
                    stroke: Color.BLACK
                    strokeWidth: 1
                    var opacity = 0.6;
                    opacity: bind opacity
                    fill: LinearGradient {
                        startX: 0.0
                        startY: 0.0
                        endX: 0.0
                        endY: 1.0
                        stops: [
                            Stop {
                                color: Color.ORANGE
                                offset: 0.0
                            },
                            Stop {
                                color: Color.DARKRED
                                offset: 0.5
                            },
                            Stop {
                                color: Color.ORANGE
                                offset: 1.0
                            },
                        ]
                    }

                    onMouseClicked: onClosed
                    onMouseMoved: function( e: MouseEvent ):Void {
                        opacity = 1.0
                    }
                    onMouseExited: function( e: MouseEvent ):Void {
                        opacity = 0.6
                    }
                },

                /* Cross in title bar */

                Text {
                    font: Font {
                        size: 15
                        name: "Arial Bold"
                    }
                    x: width - 21,
                    y: 15
                    content: "x"
                }
            ]
        }
    }
}


