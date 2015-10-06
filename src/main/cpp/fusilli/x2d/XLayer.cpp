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

#include "renderer/CCTextureCache.h"
#include "2d/CCSprite.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
//#include "YesNo.h"
#include "XLayer.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XLayer::Realize() { return this; }

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AudioCallback(c::Ref* r) {
  auto t = SCAST(c::MenuItemToggle*, r);
  if (t->getSelectedIndex() == 0) {
    XCFGS()->ToggleAudio(true);
  } else {
    XCFGS()->ToggleAudio(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::OnQuit(c::Ref* rr) {
  //c::Scene* s= YesNo::Create()->Realize();
  //c::Director::getInstance()->pushScene(s);
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer::XLayer() {
  lastTag = 0;
  lastZix = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer::~XLayer() {
  for (auto it= atlases.begin();
      it != atlases.end(); ++it) {
    //it->second->release();
  }
  //atlases.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
c::SpriteBatchNode*
XLayer::RegoAtlas(const stdstr& name, int* z, int* tag) {
  auto i= c::TextureCache::getInstance()->addImage(
      XCFGS()->GetImage(name));
  auto a= c::SpriteBatchNode::createWithTexture(i);
  auto t = ENP(tag) ? ++lastTag : *tag;
  auto x = ENP(z) ? lastZix : *z;

  atlases.insert(s::pair<stdstr, c::SpriteBatchNode*>(name,a));
  this->addChild(a, x,t);
  //a->retain(); // for the map
  return a;
}

//////////////////////////////////////////////////////////////////////////////
// Get the atlas
//
c::SpriteBatchNode* XLayer::GetAtlas(const stdstr& name) {
  auto it= atlases.find(name);
  if (it != atlases.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Add an image chosen from this atlas
//
void XLayer::AddAtlasFrame(const stdstr& atlas,
                           const stdstr& frame,
                           const c::Vec2& pos) {
  auto tt= cx::CreateSprite(frame);
  tt->setPosition(pos);
  AddAtlasItem(atlas, tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add an image
//
void XLayer::AddFrame(const stdstr& frame, const c::Vec2& pos) {
  auto tt= cx::CreateSprite(frame);
  tt->setPosition(pos);
  AddItem(tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child to this atlas
//
void XLayer::AddAtlasItem(const stdstr& atlas,
    not_null<c::Node*> n, int* zx, int* tag) {

  auto ptag = ENP(tag) ? ++lastTag : *tag;
  auto pzx = ENP(zx) ? lastZix : *zx;
  auto p= GetAtlas(atlas);
  auto ss = DCAST(c::Sprite*, n.get());

  CCASSERT(p != nullptr, "atlas cannot be null");
  CCASSERT(ss != nullptr, "sprite cannot be null");

  if (NNP(p)) {
    if (NNP(ss)) { ss->setBatchNode(p); }
    p->addChild(n.get(), pzx, ptag);
  }
}

//////////////////////////////////////////////////////////////////////////////
// Add a child
//
void XLayer::AddItem(not_null<c::Node*> n, int* zx, int* tag) {
  auto ptag = ENP(tag) ?  ++lastTag : *tag;
  auto pzx = ENP(zx) ? lastZix : *zx;
  this->addChild(n.get(), pzx, ptag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterImage(const stdstr& name, int z) {
  auto fp= XCFGS()->GetImage(name);
  auto t= c::TextureCache::getInstance()->addImage(fp);
  auto s= c::Sprite::createWithTexture(t);
  s->setPosition(cx::Center());
  this->addChild(s,z);
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children from this atlas
//
void XLayer::RemoveAtlasAll(const stdstr& atlas) {
  auto a=GetAtlas(atlas);
  if (NNP(a)) { a->removeAllChildren(); }
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children
//
void XLayer::RemoveAll() {
  this->removeAllChildren();
}

//////////////////////////////////////////////////////////////////////////////
// Remove a child
//
void XLayer::RemoveItem(not_null<c::Node*> n) {
  if (NNP(n)) {
    n->removeFromParent();
  }
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
  return (XScene*) getScene();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddAudioIcons(not_null<c::MenuItem*> off,
    not_null<c::MenuItem*> on,
    const c::Vec2& anchor, const c::Vec2& pos) {
  c::Vector<c::MenuItem*> items;
  items.pushBack(on);
  items.pushBack(off);

  auto audio = c::MenuItemToggle::createWithTarget(
      this,
      CC_MENU_SELECTOR(XLayer::AudioCallback), items);

  audio->setSelectedIndex( XCFGS()->HasAudio() ? 0 : 1);
  audio->setAnchorPoint(anchor);

  // need null to end var-args
  auto menu= c::Menu::create(audio, nullptr);
  menu->setPosition(pos);
  AddItem(menu);
}


NS_END(fusii)

