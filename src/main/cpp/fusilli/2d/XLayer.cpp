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

#include "support/XConfig.h"
//#include "YesNo.h"
#include "XLayer.h"
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
cc::SpriteBatchNode*
XLayer::RegoAtlas(const s::string& name, int* z, int* tag) {

  auto img = XConfig::GetInstance()->GetImage(name);
  auto a= cc::SpriteBatchNode::createWithTexture(
      cc::TextureCache::getInstance()->addImage(img));
  auto t = ENP(tag) ? ++lastTag : *tag;
  auto x = ENP(z) ? lastZix : *z;
  addChild(a, x,t);
  atlases.insert(pair<s::string, cc::SpriteBatchNode*>(name,a));
  a->retain(); // for the map
  return a;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XLayer::Realize() { return this; }

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::PKInput() {}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AudioCallback(cc::Ref* sender) {
  auto t = SCAST(cc::MenuItemToggle*,sender);
  auto cfg = XConfig::GetInstance();
  if (t->getSelectedIndex() == 0) {
    cfg->ToggleAudio(true);
  } else {
    cfg->ToggleAudio(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CreateAudioIcons(cc::MenuItem*& off, cc::MenuItem*& on) {
  auto n="#sound_off.png";
  off= cc::MenuItemSprite::create(cc::Sprite::create(n),
                              cc::Sprite::create(n),
                              cc::Sprite::create(n));
  n="#sound_on.png";
  on= cc::MenuItemSprite::create(cc::Sprite::create(n),
                             cc::Sprite::create(n),
                             cc::Sprite::create(n));
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddAudioIcons(cc::MenuItem* off, cc::MenuItem* on,
    const cc::Vec2& anchor, const cc::Vec2& pos) {

  cc::Vector<cc::MenuItem*> items {on,off};
  auto cfg = XConfig::GetInstance();
  cc::MenuItemToggle* audio = cc::MenuItemToggle::createWithCallback(
      CC_CALLBACK_1(XLayer::AudioCallback, this), items);
  audio->setAnchorPoint(anchor);
  audio->setSelectedIndex( cfg->HasAudio() ? 0 : 1);
  //
  auto menu= cc::Menu::create(audio);
  menu->setPosition(pos);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::OnQuit(cc::Ref* rr) {
  //cc::Scene* s= YesNo::Create()->Realize();
  //cc::Director::getInstance()->pushScene(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterAtlasImage(const s::string& atlas,
    const s::string& frame) {

  AddAtlasFrame(atlas, CCSX::Center(), frame);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterImage(const s::string& frame) {
  AddFrame(frame, CCSX::Center());
}

//////////////////////////////////////////////////////////////////////////////
// Add an image chosen from this atlas
//
void XLayer::AddAtlasFrame(const s::string& atlas,
                           const cc::Vec2& pos,
                           const s::string& frame) {
  auto tt= cc::Sprite::create(frame);
  tt->setPosition(pos);
  AddAtlasItem(atlas, tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add an image
//
void XLayer::AddFrame(const s::string& frame, const cc::Vec2& pos) {
  auto tt= cc::Sprite::create(frame);
  tt->setPosition(pos);
  AddItem(tt);
}

//////////////////////////////////////////////////////////////////////////////
// Get the atlas
//
cc::SpriteBatchNode* XLayer::GetAtlas(const s::string& name) {
  auto it= atlases.find(name);
  if (it != atlases.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children from this atlas
//
void XLayer::RemoveAtlasAll(const s::string& atlas) {
  auto a=GetAtlas(atlas);
  if (NNP(a)) { a->removeAllChildren(); }
}

//////////////////////////////////////////////////////////////////////////////
// Remove child from this atlas
//
void XLayer::RemoveAtlasItem(const s::string& atlas, cc::Node* n) {
  if (NNP(n)) {
    n->removeFromParent();
  }
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children
//
void XLayer::RemoveAll(bool c) {
  this->removeAllChildren();
}

//////////////////////////////////////////////////////////////////////////////
// Remove a child
//
void XLayer::RemoveItem(cc::Node* n) {
  if (NNP(n)) {
    n->removeFromParent();
  }
}

//////////////////////////////////////////////////////////////////////////////
// Add a child to this atlas
//
void XLayer::AddAtlasItem(const s::string& atlas,
    cc::Node* n, int zx, int tag) {

  auto ptag = tag == INT32_MIN ? ++lastTag : tag;
  auto pzx = zx == INT32_MIN ? lastZix : zx;
  auto p= GetAtlas(atlas);
  auto ss = DCAST(cc::Sprite*,n);

  if (DCAST(cc::SpriteBatchNode*,p) != nullptr &&
      NNP(ss)) {
    ss->setBatchNode(p);
  }

  if (NNP(p)) { p->addChild(n, pzx, ptag);  }
}


//////////////////////////////////////////////////////////////////////////////
// Add a child
//
void XLayer::AddItem(cc::Node* n, int zx, int tag) {
  auto ptag = tag == INT32_MIN ? ++lastTag : tag;
  auto pzx = zx == INT32_MIN ? lastZix : zx;
  addChild(n, pzx, ptag);
}

//////////////////////////////////////////////////////////////////////////////
//
int XLayer::IncIndexZ() {
  return ++lastZix;
}

//////////////////////////////////////////////////////////////////////////////
// Remember the parent scene object
//
XScene* XLayer::GetScene() {
  return SCAST(XScene*,getParent());
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer::XLayer() {
  lastTag= 0;
  lastZix= 0;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer::~XLayer() {
  for (auto it= atlases.begin(); it != atlases.end(); ++it) {
    it->second->release();
  }
  atlases.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
cc::MenuItem* XLayer::CreateMenuBtn(const s::string& n,
    const s::string& s,
    const s::string& d) {

  return cc::MenuItemSprite::create(cc::Sprite::create(n),
                                cc::Sprite::create(s),
                                cc::Sprite::create(d));
}


NS_END(fusilli)

