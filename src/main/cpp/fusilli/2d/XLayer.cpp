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
#include "YesNo.h"
#include "XLayer.h"
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
SpriteBatchNode* XLayer::RegoAtlas(const string& name,
    int z, int tag) {

  auto img = XConfig::GetInstance()->GetImage(name);
  auto a= SpriteBatchNode::createWithTexture(
      TextureCache::getInstance()->addImage(img));
  if (tag == INT32_MIN) {
    tag = ++lastTag;
  }
  if (z == INT32_MIN) {
    z = lastZix;
  }
  addChild(a, z, tag);
  atlases.insert(pair<string,SpriteBatchNode*>(name,a));
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
void XLayer::AudioCallback(Ref* sender) {
  auto t = SCAST(MenuItemToggle*,sender);
  auto cfg = XConfig::GetInstance();
  if (t->getSelectedIndex() == 0) {
    cfg->ToggleAudio(true);
  } else {
    cfg->ToggleAudio(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CreateAudioIcons(MenuItem*& off, MenuItem*& on) {
  auto n="#sound_off.png";
  off= MenuItemSprite::create(Sprite::create(n),
                              Sprite::create(n),
                              Sprite::create(n));
  n="#sound_on.png";
  on= MenuItemSprite::create(Sprite::create(n),
                             Sprite::create(n),
                             Sprite::create(n));
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddAudioIcons(MenuItem* off, MenuItem* on,
    const Vec2& anchor, const Vec2& pos) {

  auto cfg = XConfig::GetInstance();
  Vector<MenuItem*> items= {on,off};
  MenuItemToggle* audio = MenuItemToggle::createWithCallback(
      CC_CALLBACK_1(XLayer::AudioCallback, this), items);
  audio->setAnchorPoint(anchor);
  audio->setSelectedIndex( cfg->HasAudio() ? 0 : 1);
  //
  auto menu= Menu::create(audio);
  menu->setPosition(pos);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::OnQuitAction(Ref* rr) {
  Scene* s= YesNo::Create()->Realize();
  Director::getInstance()->pushScene(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterAtlasImage(const string& atlas,
    const string& frame) {

  AddAtlasFrame(atlas, CCSX::Center(), frame);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterImage(const string& frame) {
  AddFrame(frame, CCSX::Center());
}

//////////////////////////////////////////////////////////////////////////////
// Add an image chosen from this atlas
//
void XLayer::AddAtlasFrame(const string& atlas,
                           const Vec2& pos,
                           const string& frame) {
  auto tt= Sprite::create(frame);
  tt->setPosition(pos);
  AddAtlasItem(atlas, tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add an image
//
void XLayer::AddFrame(const string& frame, const Vec2& pos) {
  auto tt= Sprite::create(frame);
  tt->setPosition(pos);
  AddItem(tt);
}

//////////////////////////////////////////////////////////////////////////////
// Get the atlas
//
SpriteBatchNode* XLayer::GetAtlas(const string& name) {
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
void XLayer::RemoveAtlasAll(const string& atlas) {
  auto a=GetAtlas(atlas);
  if (NNP(a)) { a->removeAllChildren(); }
}

//////////////////////////////////////////////////////////////////////////////
// Remove child from this atlas
//
void XLayer::RemoveAtlasItem(const string& atlas, Node* n) {
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
void XLayer::RemoveItem(Node* n) {
  if (NNP(n)) {
    n->removeFromParent();
  }
}

//////////////////////////////////////////////////////////////////////////////
// Add a child to this atlas
//
void XLayer::AddAtlasItem(const string& atlas,
    Node* n, int zx, int tag) {

  auto ptag = tag == INT32_MIN ? ++lastTag : tag;
  auto pzx = zx == INT32_MIN ? lastZix : zx;
  auto p= GetAtlas(atlas);
  auto ss = DCAST(Sprite*,n);

  if (DCAST(SpriteBatchNode*,p) != nullptr &&
      NNP(ss)) {
    ss->setBatchNode(p);
  }

  if (NNP(p)) { p->addChild(n, pzx, ptag);  }
}


//////////////////////////////////////////////////////////////////////////////
// Add a child
//
void XLayer::AddItem(Node* n, int zx, int tag) {
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
Scene* XLayer::GetScene() {
  return SCAST(Scene*,getParent());
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
MenuItem* XLayer::CreateMenuBtn(const string& n,
    const string& s,
    const string& d) {

  return MenuItemSprite::create(Sprite::create(n),
                                Sprite::create(s),
                                Sprite::create(d));
}


NS_END(fusilli)

