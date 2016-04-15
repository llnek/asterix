// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "n/C.h"
#include "Game.h"
#include "Splash.h"
#include "LevelSelector.h"

//this is for testing purposes... so imagine we have 100 levels!
#define LEVELS_PER_SCREEN 25
#define TOTAL_LEVELS 100

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL LabelBtn {
  LabelBtn(c::Label *a, c::Sprite *b) {
    label=a;
    btn=b;
  }
  __decl_ptr(c::Label, label)
  __decl_ptr(c::Sprite,btn)
};

enum {
  kTagButtonOn,
  kTagButtonOff
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL LLayer : public f::XLayer {

  __decl_create_layer(LLayer)
  __decl_deco_ui()

  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  __decl_vec(LabelBtn, _levelLabels)

  __decl_ptr(c::Menu, _btnNext)
  __decl_iz(_levelsCompleted)
  __decl_iz(_firstIndex)

  void createScreen();
  void createMenu();
};

//////////////////////////////////////////////////////////////////////////////
//
void LLayer::decoUI() {

  _levelsCompleted = CC_APPDB()->getIntegerForKey("levelsCompleted");
  if (_levelsCompleted == 0) {
    _levelsCompleted = 1;
    CC_APPDB()->setIntegerForKey("levelsCompleted", 1);
    CC_APPDB()->flush();
  }

  _firstIndex = 1 + (int) floor(_levelsCompleted/LEVELS_PER_SCREEN) * LEVELS_PER_SCREEN;

  enableListeners();
  createScreen();
}

//////////////////////////////////////////////////////////////////////////////
//
bool LLayer::onMouseStart(const CCT_PT &tap) {
  auto i=_firstIndex;

  F__LOOP(it,_levelLabels) {
    auto &z= *it;
    if (cx::isTapped(z.btn, tap)) {
      if (z.btn->getTag() == kTagButtonOff) {
         z.btn->setDisplayFrame(cx::getSpriteFrame("btn_num_on.png"));
         break;
      }
    }
    ++i;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool LLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void LLayer::onMouseClick(const CCT_PT &tap) {
  auto i=0;
  F__LOOP(it, _levelLabels) {
    auto &z= *it;
    if (cx::isTapped(z.btn,tap)) {
      if (z.btn->getTag() == kTagButtonOff) {
        cx::sfxPlay("button");
        z.btn->setDisplayFrame(cx::getSpriteFrame("btn_num_off.png"));
        auto x= new GameCtx(_firstIndex + i, _levelsCompleted);
        CC_DTOR()->replaceScene( c::TransitionMoveInR::create(0.2f,
            Game::reify(x)));
        return;
      }
    }
    ++i;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void LLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick( touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void LLayer::createScreen() {
  auto wb= cx::visBox();

  centerImage("game.bg");

    //add snow particle
  auto snow = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("snow"));
  CC_POS2(snow, wb.cx, wb.top);
  addItem(snow, kBackground);

  regoAtlas("game-pics", kMiddleground);

  auto ground = cx::reifySprite("intro_igloo.png");
  ground->setAnchorPoint(CCT_PT(0.5, 0.05));
  CC_POS2(ground, wb.cx, 0);
  addAtlasItem("game-pics", ground);

  auto logo = cx::reifySprite("logo.png");
  CC_POS2(logo, wb.cx, wb.top * 0.82);
  addAtlasItem("game-pics", logo);

  //build level picker menu
  int i = _firstIndex;
  int cols = 5;
  int rows = 5;
  int tag;
  c::Sprite *mi;

  for (auto r = 0; r < rows; ++r) {
    for (auto c = 0; c < cols; ++c) {
      auto pos= CCT_PT(wb.right * 0.2 + c * wb.right * 0.15,
                       wb.top * 0.65 - wb.right * 0.15 * r );
      if (i > _levelsCompleted) {
        mi = cx::reifySprite("btn_num_on.png");
        tag= kTagButtonOn;
      } else {
        mi = cx::reifySprite("btn_num_off.png");
        tag= kTagButtonOff;
      }
      CC_POS1(mi, pos);

      auto label= cx::reifyBmfLabel("font_levels", FTOS(i));
      label->setAnchorPoint(cx::anchorC());
      CC_POS1(label, pos);

      addAtlasItem("game-pics", mi, 0, tag);
      addItem(label, kForeground);

      _levelLabels.push_back(LabelBtn(label,mi));
      ++i;
    }
  }

  // the nav buttons
  // next
  auto btn= cx::reifyMenuBtn("switch_2.png");
  CC_POS2(btn, wb.right * 0.85, wb.top * 0.1);
  btn->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    if (_firstIndex < TOTAL_LEVELS - LEVELS_PER_SCREEN) {
      _firstIndex += LEVELS_PER_SCREEN;
      createMenu();
    }
  });
  auto m= cx::mkMenu(btn);
  _btnNext=m;
  addItem(m, kForeground);

  // back
  btn= cx::reifyMenuBtn("switch_3.png");
  m= cx::mkMenu(btn);
  CC_POS2(btn, wb.right * 0.15, wb.top * 0.1);
  btn->setCallback([=](c::Ref*) {
    cx::sfxPlay("button");
    if (_firstIndex == 1) {
      CC_DTOR()->replaceScene(
          c::TransitionMoveInL::create(0.2, Splash::reify()));
    } else {
      _firstIndex -= LEVELS_PER_SCREEN;
      createMenu();
      CC_SHOW(_btnNext);
    }
  });
  addItem(m, kForeground);

  if (_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN + 1) {
    CC_HIDE(_btnNext);
  }
}

//update the level labels with the current displayed range (1-25, 26-50, 51-75, 76-100)
//////////////////////////////////////////////////////////////////////////////
//
void LLayer::createMenu() {
  int index = _firstIndex;

  F__LOOP(it,_levelLabels) {
    auto &z= *it;
    if (index > _levelsCompleted) {
      z.btn->setDisplayFrame(cx::getSpriteFrame("btn_num_on.png"));
      z.btn->setTag(kTagButtonOn);
    } else {
      z.btn->setDisplayFrame(cx::getSpriteFrame("btn_num_off.png"));
      z.btn->setTag(kTagButtonOff);
    }
    z.label->setString(s::to_string(index));
    ++index;
  }

  if (_firstIndex == TOTAL_LEVELS - LEVELS_PER_SCREEN + 1) {
    CC_HIDE(_btnNext);
  }
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void LevelSelector::decoUI() {
  LLayer::reify(this);
}


NS_END

