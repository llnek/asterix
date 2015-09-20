// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "support/CCSX.h"
#include "YesNo.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL YesNoLayer : public XLayer {

private:

  NO__COPYASSIGN(YesNoLayer)
  YesNoLayer() {}

public:

  virtual int GetIID() { return 1; }
  virtual XLayer* Realize();
  virtual ~YesNoLayer() {}

  CREATE_FUNC(YesNoLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
XLayer* YesNoLayer::Realize() {

  auto par = SCAST(YesNo*, getParent());
  auto cfg= XConfig::GetInstance();
  auto msg= par->GetMsg();
  auto fnt= cfg->GetFont("font.OCR");
  auto qn= c::Label::createWithBMFont(fnt, msg);

  auto cw= cx::Center();
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(cfg->GetScale() * 0.25);
  qn->setOpacity(0.9*255);

  CenterImage("game.bg");
  AddItem(qn);

  auto b1= cx::CreateMenuBtn("#continue.png");
  b1->setTarget(par, CC_MENU_SELECTOR(YesNo::OnYes));
  auto b2= cx::CreateMenuBtn("#cancel.png");
  b1->setTarget(par, CC_MENU_SELECTOR(YesNo::OnNo));
  auto menu= c::Menu::create();

  menu->addChild(b1);
  menu->addChild(b2);
  menu->setPosition(cw);
  AddItem(menu);

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::CreateWithActions(const stdstr& msg,
    c::CallFunc* y, c::CallFunc* n) {

  YesNo* yn = YesNo::create();
  yn->SetActions(y,n);
  yn->SetMsg(msg);
  yn->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::Create(const stdstr& msg) {
  c::CallFunc* y = c::CallFunc::create([](){
      c::Director::getInstance()->popToRootScene();
      cx::RunScene(XConfig::GetInstance()->StartWith());
      });
  c::CallFunc* n= c::CallFunc::create([]() {
      c::Director::getInstance()->popScene();
      });
  return CreateWithActions(msg, y,n);
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::SetActions(c::CallFunc* y, c::CallFunc* n) {
  yes = y;
  no = n;
  y->retain();
  n->retain();
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::OnYes(Ref* rr) {
 yes->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::OnNo(Ref* rr) {
 no->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
XScene* YesNo::Realize() {
  auto y= YesNoLayer::create();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo::YesNo()
  : yes(nullptr)
  , no(nullptr) {

}

//////////////////////////////////////////////////////////////////////////////
//
YesNo::~YesNo() {
  yes->release();
  no->release();
}



NS_END(fusilli)

