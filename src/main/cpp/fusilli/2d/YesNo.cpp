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
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL YesNoLayer : public XLayer {
private:
  DISALLOW_COPYASSIGN(YesNoLayer)
  YesNoLayer() {}

public:
  virtual const string Moniker() { return "YesNoLayer"; }
  virtual XLayer* Realize() override;
  virtual ~YesNoLayer() {}

  CREATE_FUNC(YesNoLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
XLayer* YesNoLayer::Realize() {

  auto cfg= XConfig::GetInstance();
  auto fnt= cfg->GetFont("font.OCR");
  auto qn= Label::createWithBMFont(fnt, "quit?");
  auto par = getParent();
  auto cw= CCSX::Center();
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(cfg->GetScale() * 0.25);
  qn->setOpacity(0.9*255);

  CenterImage("game.bg");
  AddItem(qn);

  auto b1= CreateMenuBtn("#continue.png",
      "#continue.png",
      "#continue.png");
  b1->setTarget(par, CC_MENU_SELECTOR(YesNo::OnYes));
  auto b2= CreateMenuBtn("#cancel.png",
      "#cancel.png",
      "#cancel.png");
  b1->setTarget(par, CC_MENU_SELECTOR(YesNo::OnNo));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->addChild(b2);
  menu->setPosition(cw);
  AddItem(menu);

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::CreateWithActions(CallFunc* y, CallFunc* n) {
  YesNo* yn = YesNo::create();
  yn->SetActions(y,n);
  yn->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::Create() {
  CallFunc* y = CallFunc::create([](){
      Director::getInstance()->popToRootScene();
      CCSX::RunScene(XConfig::GetInstance()->StartWith());
      });
  CallFunc* n= CallFunc::create([]() {
      Director::getInstance()->popScene();
      });
  return CreateWithActions(y,n);
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::SetActions(CallFunc* y, CallFunc* n) {
  yes = y;
  no = n;
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
  AddLayer( YesNoLayer::create()->Realize());
  return this;
}





NS_END(fusilli)

