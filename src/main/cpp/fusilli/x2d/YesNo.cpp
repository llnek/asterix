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
private:

  NO__CPYASS(YesNoLayer)
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

  SCAST(YesNo*, getParent())->Decorate(this);
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::Decorate(YesNoLayer* layer) {

  auto qn= cx::CreateBmfLabel("font.OCR", GetMsg());
  auto cw= cx::Center();
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(XCFG()->GetScale() * 0.25);
  qn->setOpacity(0.9*255);

  layer->CenterImage("game.bg");
  layer->AddItem(qn);

  auto b1= cx::CreateMenuBtn("continue.png");
  b1->setTarget(this, CC_MENU_SELECTOR(YesNo::OnYes));

  auto b2= cx::CreateMenuBtn("cancel.png");
  b2->setTarget(this, CC_MENU_SELECTOR(YesNo::OnNo));

  auto menu= c::Menu::create();
  menu->addChild(b1);
  menu->addChild(b2);
  menu->alignItemsVerticallyWithPadding(XCFG()->GetBtnPadding());
  menu->setPosition(cw);

  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
owner<YesNo*> YesNo::CreateWithActions(
    not_null<YesNo*> box,
    const stdstr& msg,
    not_null<c::CallFunc*> y,
    not_null<c::CallFunc*> n) {

  box->SetActions(y,n);
  box->SetMsg(msg);
  box->Realize();
  return box;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<YesNo*> YesNo::Create(not_null<YesNo*> box, const stdstr& msg) {

  c::CallFunc* y = c::CallFunc::create([](){
      CC_DTOR()->popToRootScene();
      cx::RunScene(XCFG()->StartWith());
      });

  c::CallFunc* n= c::CallFunc::create([]() {
      CC_DTOR()->popScene();
      });

  return CreateWithActions(box, msg, y,n);
}

//////////////////////////////////////////////////////////////////////////////
//
void YesNo::SetActions(not_null<c::CallFunc*> y,
    not_null<c::CallFunc*> n) {

  yes = y;
  no = n;
  CC_KEEP(y)
  CC_KEEP(n)
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
void YesNo::SetMsg(const stdstr& m) {
  msgText=m;
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

