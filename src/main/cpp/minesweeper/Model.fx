/*
 * Model.fx
 *
 * Created on 2009-4-8, 0:58:19
 */

package minesweeper;

import java.lang.Math;
import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.Node;
import minesweeper.Main;

/**
 * @author Administrator
 */

package class Model extends CustomNode{

    package var w:Integer = 0;
    package var h:Integer = 0;
    package var mineNum:Integer = 0;
    package var cellLength:Integer = 0;
    package var isWin = false;
    package var isOver = false;

    //var markedNum = 0;

    var cells: Cell[] = for( i in [0..
        h - 1] ) {
        for( j in [0..
        w - 1] ) {
            Cell {
                index: w * i + j
                x: bind j * cellLength,
                y: bind i * cellLength + Main.TitleBarHeight,
                cellLength: bind cellLength                
                var o =1.0;
                opacity: bind o
                onMouseMoved: function( event ){
                    if(isWin or isOver) return;
                    o = 0.8
                }
                onMouseExited: function( event ){
                   // if(isWin or isOver) return;
                    o = 1.0
                }
                onMousePressed: function(event) {  
                    if(isWin or isOver)
                    return;
                    var cell = event.node as Cell;
                    if(event.primaryButtonDown){
                        if(cell.isCover){
                            unCover(cell);                            
                        }else if(not cell.isCover and cell.isNum){
                            spread(cell);
                        }                        
                    }else if(event.secondaryButtonDown){
                        mark(cell);
                    }
                }
            }
        }
    }

    init {
        reset();
       // markedNum = 0;

    }

    package function reset():Void{
        for(e in cells){
            e.isCover = true;
            e.isMarked = false;
            e.isMine = false;
            e.isBlank = true;
            e.isNum = false;
            e.number = 0;
            //e.opacity = 1.0;
        }
        createMine();
        createNumber();
        isWin = false;
        isOver = false;
    }


    function unCover(cell:Cell):Void{
        if(cell.isCover and not cell.isMarked){
            cell.isCover = false;
            if(cell.isMine){
                cell.isMineExploded = true;
                isOver = true;
                for(e in cells){
                    if(e.isMine and e.isCover)
                    e.isCover = false;
                }
            }else if(not cell.isNum){//blank
                spread(cell);
            }
        }
        var isWinTemp = true;
        for(e in cells){
            if(e.isCover and not e.isMine){
                isWinTemp = false;
                break;
            }
        }
        if(isWinTemp){
             for(e in cells){
                 if(not e.isMarked and e.isCover)
                 e.isMarked = true;
             }
             isWin = isWinTemp;
        }
    }

    function mark(cell:Cell):Void{
        if(cell.isCover){
            cell.isMarked = not cell.isMarked;
        }

    }

    function spread(cell:Cell):Void{
        var list = getAroundCells(cell);
        if(cell.isNum and not cell.isCover){
            var num = cell.number;
            //println(num);
            for(e in list){
                if(e.isCover and e.isMarked){num--;

                }
            }

            if(num == 0){
                for(e in list) {
                    unCover(e);
                }
            }
        }else if(not cell.isNum){//is blank
            for(e in list){
                unCover(e);
            }
        }
    }

    function getAroundCells(cell:Cell):Cell[]{
        var i: Integer = cell.index;
        var indexes = if(i mod w == 0){//left
            [i - w, i + w, i + 1,i + w + 1,i - w + 1];
        }else if(i mod w == w - 1){//right
            [i - w, i + w, i - 1,i + w - 1,i - w - 1];
        }else if(i / w == 0 ){//top
            [i - 1,i + 1,i + w + 1,i + w,i + w - 1];
        }else if(i / w == h - 1){//bottom
            [i - 1,i + 1,i - w - 1,i - w,i - w + 1];
        }else {//center
            [i - 1,i - w - 1,i - w,i - w + 1,i + 1,i + w + 1,i + w,i + w - 1];
        }


        for(j in indexes){
            cells[j];
        }
        //return cells[n | isIn(n.index,indexes) ];


    }

    function createMine():Void{
        for(i in [0..mineNum]){
            var max = sizeof cells - 1;
            var j =
            (Math.random() * max).intValue();
            cells[j].isMine = true;           
        }
    }
    function createNumber():Void{
        for(e in cells){
            if(e.isMine){
                var arround = getAroundCells(e);
                for(one in arround){
                    if(not one.isMine){
                        one.number++;
                        one.isNum = true;
                    }
                }
            }
        }
    }

    public override function create(): Node{
        Group {
            content: bind[
                //[[Cell{x:10,y:10},Cell{x:20,y:20}],[Cell{x:30,y:30},Cell{x:40,y:40}]]
                cells
            ]
        }
    }
}

