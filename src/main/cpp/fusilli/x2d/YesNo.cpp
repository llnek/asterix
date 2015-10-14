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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "2d/CCLabel.h"
#include "YesNo.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL YesNoLayer : public XLayer {
public:
  virtual XLayer* Realize();
  NO__CPYASS(YesNoLayer)
  IMPL_CTOR(YesNoLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
XLayer* YesNoLayer::Realize() {
  CC_PCAST(YesNo*)->DecoUI(this);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::DecoUI(XLayer* layer) {

  auto qn= cx::ReifyBmfLabel("font.OCR", this->msgText);
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  qn->setScale(XCFG()->GetScale() * 0.25);
  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setOpacity(0.9*255);

  layer->CenterImage("game.bg");
  layer->AddItem(qn);

  auto b1= cx::ReifyMenuBtn("continue.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(YesNo::OnYesBtn));

  auto b2= cx::ReifyMenuBtn("cancel.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(YesNo::OnNoBtn));

  auto menu= ReifyRefType<cocos2d::Menu>();
  menu->addChild(b1);
  menu->addChild(b2);
  menu->alignItemsVerticallyWithPadding(XCFG()->GetBtnPadding());
  menu->setPosition(cw);

  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::ReifyWithActions(
    not_null<YesNo*> box,
    const stdstr& msg,
    not_null<c::CallFunc*> y,
    not_null<c::CallFunc*> n) {

  box->yes= y;
  box->no=n;
  CC_KEEP(y);
  CC_KEEP(n);
  box->msgText= msg;
  box->Realize();
  return box;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo* YesNo::Reify(not_null<YesNo*> box, const stdstr& msg) {

  c::CallFunc* y = c::CallFunc::create([](){
      CC_DTOR()->popToRootScene();
      cx::RunScene(XCFG()->StartWith());
      });

  c::CallFunc* n= c::CallFunc::create([]() {
      CC_DTOR()->popScene();
      });

  return ReifyWithActions(box, msg, y,n);
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::OnYesBtn(Ref* rr) {
  yes->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::OnNoBtn(Ref* rr) {
  no->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
XScene* YesNo::Realize() {
  auto y= ReifyRefType<YesNoLayer>();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo::YesNo() {
  SNPTR(yes)
  SNPTR(no)
}

//////////////////////////////////////////////////////////////////////////////
//
YesNo::~YesNo() {
  CC_DROP(yes)
  CC_DROP(no)
}



NS_END(fusii)

