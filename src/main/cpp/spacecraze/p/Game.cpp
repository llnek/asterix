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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "BackDrop.h"
//#include "MMenu.h"
#include "HUD.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)
BEGIN_NS_UNAMED


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*) getSceneX()->getLayer(3); }

    virtual void postReify();
  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void createLevel();

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  regoAtlas("game-pics");
  engine= mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  //char level_file[64] = {0};
  //::sprintf(level_file, "Level%02d.xml", MGMS()->getLevel());
  auto cl= MGMS()->getLevel();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto mez = cx::calcSize("sfenmy3");
  auto top = wb.top * 0.8f;
  sstr fp = cl < 10 ? "0" : "";
  auto map= cx::readXmlAsDict("pics/Level" + fp + s::to_string(cl) + ".xml");
  auto d= f::dictVal<c::Dictionary>(map, "player");
  auto r1= CC_GDV(c::Double, d, "fireRate");

  d= f::dictVal<c::Dictionary>(map, "enemy");
  auto dur= CC_GDV(c::Double, d, "moveDuration");
  auto r2= CC_GDV(c::Double, d, "fireRate");
  auto scores= f::dictVal<c::Dictionary>(d, "scores");
  auto layout= f::dictVal<c::Array>(d, "layout");
  Ref *ref= nullptr;
  CCARRAY_FOREACH(layout, ref) {
    auto s = (c::String*) ref;
    auto v= f::tokenize(s->getCString(), ',');
    float height_t=0;
    float width_t=0;
    float gap=0;
    gap= s::stof(v[0]);
    v.erase(v.begin());
    s_vec<c::Sprite*> aliens;
    auto z= v.size();
    for (auto j=0; j < z; ++j) {
      auto png="sfenmy" + v[j];
      auto sp= cx::reifySprite(png);
      auto sz= sp->getContentSize();
      width_t += sz.width;
      height_t= MAX(sz.height, height_t);
      aliens.push_back(sp);
    }
    width_t += gap * (z-1);
    auto lf= wb.left + 0.5f * (wb.right - wb.left - width_t);
    F__LOOP(it,aliens) {
      auto sp= *it;
      auto sw= sp->getContentSize().width;
      sp->setPosition(lf + sw * 0.5f, top);
      MGML()->addAtlasItem("game-pics",sp);
      lf += sw + gap;
    }
    top -= height_t * 1.5f;
  }

}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createLevel() {

  auto wb = cx::visBox();

  //score_ = score_to_carry_;

  regoAtlas("game-pics");

}



END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &t, void *m) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Game::decorate() {
  BackDrop::reify(this, 1);
  HUDLayer::reify(this, 3);
  GLayer::reify(this,2);
  //play();
}



NS_END



