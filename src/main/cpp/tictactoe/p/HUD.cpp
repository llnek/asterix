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

#include "core/CCSX.h"
#include "HUD.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::Realize() {

  this->color = cx::ColorRGB("#5e3178");
  RegoAtlas("game-pics");

  InitLabels();
  InitIcons();

  auto r= cx::ReifyMenuBtn("icon_replay.png");
  r->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::OnReplay));
  r->setColor(c3);
  //r->setScale(scale);
  AddReplayIcon(r, cx::AnchorB());

  auto b= cx::ReifyMenuBtn("icon_menu.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::ShowMenu));
  b->setColor(c3);
  //b->setScale(scale);
  AddMenuIcon(b, cx::AnchorB());

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitScores() {
  scores.clear();
  scores[1]= 0;
  scores[2]= 0;
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitLabels() {
  auto soff= CC_CSV(c::Integer, "S_OFF");
  auto tile= CC_CSV(c::Integer, "TILE");
  auto scale= XCFG()->GetScale();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  //title
  title = cx::ReifyBmfLabel( "font.JellyBelly", "");
  title->setScale(scale * 0.6);
  title->setAnchor(cx::AnchorT());
  title->setColor(this->color);
  title->setPosition(cw.x, wb.top - 2*tile);
  AddItem(title);

  //score1
  score1= cx::ReifyBmfLabel("font.SmallTypeWriting", "0");
  score1->setAnchor(cx::AnchorTL());
  score1->setColor(cx::White());
  score1->setScale(scale);
  score1->setPosition(tile + soff + 2, wb.top - tile - soff);
  AddItem(score1);

  //score2
  score2= cx::ReifyBmfLabel( "font.SmallTypeWriting", "0");
  score2->setAnchor(cx::AnchorTR());
  score2->setColor(cx::White());
  score2->setScale(scale);
  score2->setPosition(wb.right - tile - soff,
              wb.top - tile - soff);
  AddItem(score2);

  // status
  status= cx::ReifyBmfLabel( "font.CoffeeBuzzed", "");
  status->setColor(cx::White());
  status->setScale(scale * 0.3);
  status->setPosition(cw.x, wb.bottom + tile * 10);
  AddItem(status);

  // result
  result= cx::ReifyBmfLabel( "font.CoffeeBuzzed", "");
  result->setColor(cx::White());
  result->setScale(scale * 0.3);
  result->setPosition(cw.x, wb.bottom + tile * 10);
  result->setVisible(false);
  AddItem(result);

  auto b = cx::ReifyMenuBtn("icon_menu.png");
  auto hh = cx::GetHeight(b) * 0.5;
  auto hw = cx::GetWidth(b) * 0.5;
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::ShowMenu));
  b->setColor(this->color);
  auto menu = cx::MkMenu(b);
  menu->setPosition(wb.right - tile - hw, wb.bottom + tile  + hh);
  this->AddItem(menu, 10);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::ShowTimer() {
  auto ptt= CC_CSV(c::Integer, "PLAYER_THINK_TIME");
  auto tile= CC_CSV(c::Integer,"TILE");
  auto scale= XCFG()->GetScale();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  // timer is already showing, go away
  if (countDownState) {
    return;
  }

  if (ENP(countDown)) {
    countDown= cx::ReifyBmfLabel(
      "font.AutoMission", "");
    countDown->setScale(scale * 0.5);
    countDown->setColor(cx::White());
    countDown->setPosition(cw.x, wb.top - 10*tile);
    countDown->setAnchor(cx::AnchorC());
    AddItem(countDown);
  }

  countDownState= true;
  countDownValue= ptt;

  ShowCountDown();

  schedule(this->UpdateTimer, 1.0);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::UpdateTimer(float dt) {
  if (!countDownState) { return; } else {
    countDownValue -= 1;
  }
  if (countDownValue < 0) {
    KillTimer();
    MGML()->SendMsg("/player/timer/expired");
  } else {
    ShowCountDown();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::ShowCountDown(const stdstr& msg) {
  if (NNP(countDown)) {
    countDown->setString(msg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::KillTimer() {
  if (countDownState) {
    unschedule(this->UpdateTimer);
    ShowCountDown(" ");
  }
  countDownValue=0;
  countDownState=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::UpdateScore(int pcolor, int value) {
  scores[pcolor] += value;
  DrawScores();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::Draw(bool running, int pnum) {
  if (running) {
    DrawStatus(pnum);
  } else {
    DrawResult(pnum);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::EndGame(int winner) {
  status->setVisible(false);
  result->setVisible(true);
  KillTimer();
  DrawResult(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::DrawXXXText(c::Label* obj, const stdstr& msg) {
  obj->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::DrawScores() {
  auto s2 = scores[this->play2];
  auto s1 = scores[this->play1];
  auto n2 = s::to_string(s2);
  auto n1 = s::to_string(s1);

  score1->setString(n1);
  score2->setString(n2);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::DrawResult(int pnum) {
  auto msg = XCFG()->GetL10NStr("whodraw");

  switch (pnum) {
    case 2:
      msg= XCFG()->GetL10NStr("whowin",
          s::vector<stdstr> { this->p2Long});
      break;
    case 1:
      msg= XCFG()->GetL10NStr("whowin",
          s::vector<stdstr> { this->p1Long});
      break;
  }

  DrawXXXText(result, msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::DrawStatus(int pnum) {
  if (pnum > 0) {
    auto pfx = pnum == 1 ? p1Long : p2Long;
    DrawXXXText(status,
      XCFG()->GetL10NStr("whosturn",
        s::vector<stdstr> { pfx }));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::RegoPlayers(const stdstr& color1,
    const stdstr& p1k, const stdstr& p1n,
    const stdstr& color2, const stdstr& p2k, const stdstr& p2n) {

  scores.insert(CC_PAIR(stdstr,int, color2, 0));
  scores.insert(CC_PAIR(stdstr,int, color1, 0));
  play2= color2;
  play1= color1;
  p2Long= p2n;
  p1Long= p1n;
  p2ID= p2k;
  p1ID= p1k;
  title->setString(p1ID + " / " + p2ID);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::ResetAsNew() {
  InitScores();
  Reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::Reset() {
  result->setVisible(false);
  status->setVisible(true);
}


NS_END(tttoe)

