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

#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XLayer.h"
#include "MsgBox.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL MsgBoxLayer : public XLayer {
protected:
  NO__CPYASS(MsgBoxLayer)
public:

  virtual XLayer* Realize() {

    CC_PCAST(MsgBox)->DecoUI(this);
    return this;
  }

  IMPL_CTOR(MsgBoxLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void MsgBox::DecoUI(XLayer* layer) {

  auto qn= cx::ReifyBmfLabel("font.OCR", this->textMsg);
  auto wz= cx::VisRect();
  auto cw= cx::Center();
  auto wb = cx::VisBox();

  layer->CenterImage("game.bg");

  qn->setPosition(cw.x, wb.top * 0.75);
  qn->setScale(XCFG()->GetScale() * 0.25);
  qn->setOpacity(0.9*255);
  layer->AddItem(qn);

  auto b1= cx::ReifyMenuBtn("ok.png");
    auto menu= ReifyRefType<cocos2d::Menu>();

  b1->setTarget(this,
      CC_MENU_SELECTOR(MsgBox::OnYes));

  menu->addChild(b1);
  menu->setPosition(cw.x, wb.top * 0.1);
  layer->AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
MsgBox* MsgBox::ReifyWithAction(
    not_null<MsgBox*> box,
    c::CallFunc* cb, const stdstr& msg) {

  box->textMsg = msg;
  box->action = cb;
  CC_KEEP(cb)
  box->Realize();

  return box;
}

//////////////////////////////////////////////////////////////////////////////
//
MsgBox* MsgBox::ReifyWithMsg(not_null<MsgBox*> box, const stdstr& msg) {

  auto cb= c::CallFunc::create([](){
      CC_DTOR()->popScene();
      });
  return ReifyWithAction(box, cb, msg);
}

//////////////////////////////////////////////////////////////////////////
//
void MsgBox::OnYes(c::Ref* rr) {
  action->execute();
}

//////////////////////////////////////////////////////////////////////////
//
MsgBox::~MsgBox() {
  CC_DROP(action)
}

//////////////////////////////////////////////////////////////////////////
//
MsgBox::MsgBox() {
  SNPTR(action)
}

//////////////////////////////////////////////////////////////////////////
//
XScene* MsgBox::Realize() {
  auto y = ReifyRefType<MsgBoxLayer>();
  AddLayer(y);
  y->Realize();
  return this;
}





NS_END(fusii)

