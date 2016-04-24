// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(p2048)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto g=CC_GEC(f::CGesture,_player,"f/CGesture");
  onKeys(g, dt);
  if (g->right) { swipeRight(); }
  if (g->left) { swipeLeft(); }
  if (g->up) { swipeUp(); }
  if (g->down) { swipeDown(); }
  postSwipe();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::swipeRight(GVars *ss) {
  for (auto r= 0; r < ss->cardArr.size(); ++r) {
    swipeRight( ss->cardArr[r], true);
  }
}

void GLayer::swipeRight(CardArr *arr) {

  auto lastPos= arr.size()-1;
  for (auto i=arr.size()-1; i >= 0; --i) {
    auto v= arr->get(i)->getNumber();
    if (v > 0)
    for (auto n=i+1; n <= lastPos; ++n) {
      auto v2=arr->get(n)->getNumber();
    }
  }


  auto canMerge= true;
  auto lastZ= -1;
  for (auto i=arr.size()-2; i >= 0; --i) {
    auto rv= arr->get(i+1);
    auto lv= arr->get(i);
    // where is the lastZ
    if (lastZ < 0) {
      if (rv== 0) { lastZ=i+1;}
      else if (lv==0) { lastZ=i;}
    }
    if (lv > 0) {
      if (rv > 0) {
        if (lv==rv && canMerge) {
          arr->set(i+1, rv+lv);
          arr->set(i,0);
          lastZ= i;
          canMerge=false;
        }
      } else {
        if (lastZ >= 0) {
          arr->set(lastZ,lv);
          arr->set(i,0);
          --lastZ;
          if (arr->get(lastZ)->getNumber() > 0) {
            lastZ=i;
          }
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::swipeLeft() {
    bool isMove = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            for (int x1 = x+1; x1<4; x1++)
            {
                if (cardArr[x1][y]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x--;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);

                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }

    return isMove;
}

//右滑动
bool GameScene::doRight()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 3; x >= 0; x--)
        {
            for (int x1 = x-1; x1>=0; x1--)
            {
                if (cardArr[x1][y]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x++;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }

    return isMove;
}

//上滑动
bool GameScene::doUp()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 3; y >= 0; y--)
        {
            for (int y1 = y-1; y1>=0; y1--)
            {
                if (cardArr[x][y1]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y++;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }

    return isMove;
}

//下滑动
bool GameScene::doDown()
{
    //判断有没有发生移动
    bool isMove = false;
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y <4; y++)
        {
            for (int y1 = y+1; y1<4; y1++)
            {
                if (cardArr[x][y1]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber() <= 0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
                        cardArr[x][y1]->setNumber(0);
                        y--;
                        isMove = true;
                    }
                    else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
                    {
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x][y1]->setNumber(0);
                        //改变分数
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }

    return isMove;
}
//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(f::Gesture *g, float dt) {
}


NS_END




