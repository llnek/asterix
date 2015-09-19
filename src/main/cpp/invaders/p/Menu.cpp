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

#include "deprecated/CCInteger.h"
#include "support/XConfig.h"
#include "Menu.h"
NS_ALIAS(cx,fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MenuLayer : public f::XMenuLayer {
private:
  NO__COPYASSIGN(MenuLayer)
  MenuLayer();

public:

  virtual int GetIID() { return 1; }
  virtual void OnInit();
  virtual ~MenuLayer();

  void OnBack(c::Ref*);
  void OnQuit(c::Ref*);

  CREATE_FUNC(MenuLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::~MenuLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::MenuLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::Title() {
  auto cfg = f::XConfig::GetInstance();
  auto wb= cx::VisBox();
  auto cw= cx::Center();
  auto fp= cfg->GetFont("font.JellyBelly");
  auto lb= cx::CreateBmfLabel(cw.x, wb.top * 0.9,
                          fp,
                          "some text");
  lb->setScale(cfg->GetScale());
  lb->setColor(cx::WHITE);
  AddItem(lb);
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* MenuLayer::Realize() {
  auto tile = CstVal<c::Integer>("TILE")->getValue();
  auto cfg = f::XConfig::GetInstance();
  auto color= c::Color3B(94,49,120);
  auto cw = cx::Center();
  auto wb = cx::VisBox();

  // background
  auto fp = cfg->GetImage("mmenus.bg");
  CenterImage(fp);

  // title
  fp= cfg->GetFont("font.JellyBelly");
  auto lb= cx::CreateBmfLabel(cw.x, wb.top * 0.9,
                          fp,
                          "some text");
  lb->setScale(cfg->GetScale());
  lb->setColor(cx::WHITE);
  AddItem(lb);

  // play button
  auto b1= CreateMenuBtn("#play.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  auto menu= c::Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw);
  AddItem(menu);

  // back-quit button
  auto b= CreateMenuBtn("#icon_back.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnBack));
  auto q= CreateMenuBtn("#icon_quit.png");
  q->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnQuit));
  auto m2= MkBackQuit(false, b, q);
  auto sz= b->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  AddItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  CreateAudioIcons(on,off);
  off->setColor(cx::WHITE);
  on->setColor(cx::WHITE);

  AddAudioIcons(off, on,
      cx::AnchorBR(),
      c::Vec2(wb.right - gap, wb.bottom + gap));
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay() {
//  auto g= Game::create()->Realize();
//  cx::RunScene(g);
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::OnBack(c::Ref* r) {
  auto p = SCAST(MainMenu*, getParent());
  p->OnBackAction();
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::OnQuit(c::Ref* r) {
  auto p = SCAST(MainMenu*, getParent());
  p->OnQuitAction();
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::CreateWithBackAction(c::CallFunc* back) {
  backAction= back;
  back->retain();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::OnBackAction(c::Ref* s) {
  backAction->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::Realize() {
  auto y = MenuLayer::create();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {
  backAction->release();
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu()
  : backAction(nullptr) {
}





NS_END(invaders)

