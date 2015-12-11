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
#include "2d/CCMenuItem.h"
#include "2d/CCSprite.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
#include "XLayer.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onQuit(c::Ref* ) {
  cx::runScene( XCFG()->startWith());
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::initEx(XScene* par, int zx) {
  if (c::Layer::init()) {
    par->addLayer(this, zx);
    decorate();
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
c::SpriteBatchNode*
XLayer::regoAtlas(const sstr& name, int zx) {

  auto i= c::TextureCache::getInstance()->addImage( XCFG()->getImage(name));
  auto a= c::SpriteBatchNode::createWithTexture(i);

  atlases.insert(S__PAIR(sstr, c::SpriteBatchNode*, name, a));
  this->addChild(a, zx, ++lastTag);
  return a;
}

//////////////////////////////////////////////////////////////////////////////
// Get the atlas
//
c::SpriteBatchNode* XLayer::getAtlas(const sstr& name) {
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
void XLayer::addAtlasFrame(const sstr& atlas,
                           const sstr& frame,
                           const c::Vec2& pos) {
  auto tt= cx::reifySprite(frame);
  tt->setPosition(pos);
  addAtlasItem(atlas, tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add an image
//
void XLayer::addFrame(const sstr& frame, const c::Vec2& pos) {
  auto tt= cx::reifySprite(frame);
  tt->setPosition(pos);
  addItem(tt);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child to this atlas
//
void XLayer::addAtlasItem(const sstr& atlas,
    not_null<c::Node*> n,
    const MaybeInt& zx, const MaybeInt& tag) {

  auto ptag = tag.isNone() ? ++lastTag : tag.get();
  auto pzx = zx.isNone() ? lastZ : zx.get();
  auto p= getAtlas(atlas);
  auto ss = DCAST(c::Sprite*, n.get());

  //CCASSERT(ss != nullptr, "sprite cannot be null");
  CCASSERT(p != nullptr, "atlas cannot be null");

  if (NNP(ss)) { ss->setBatchNode(p); }
  p->addChild(n.get(), pzx, ptag);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child
//
void XLayer::addItem(not_null<c::Node*> n,
    const MaybeInt&  zx, const MaybeInt& tag) {

  auto ptag = tag.isNone() ?  ++lastTag : tag.get();
  auto pzx = zx.isNone() ? lastZ : zx.get();
  this->addChild(n.get(), pzx, ptag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::centerImage(const sstr& name, int z) {
  auto t= c::TextureCache::getInstance()->addImage(XCFG()->getImage(name));
  auto s= c::Sprite::createWithTexture(t);
  s->setPosition(cx::center());
  this->addChild(s,z);
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children from this atlas
//
void XLayer::removeAtlasAll(const sstr& atlas) {
  auto a=getAtlas(atlas);
  if (NNP(a)) { a->removeAllChildren(); }
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children
//
void XLayer::removeAll() {
  this->removeAllChildren();
}

//////////////////////////////////////////////////////////////////////////////
// Remove a child
//
void XLayer::removeItem(not_null<c::Node*> n) {
  if (NNP(n)) {
    n->removeFromParent();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int XLayer::incIndexZ() {
  return ++lastZ;
}

//////////////////////////////////////////////////////////////////////////////
// Remember the parent scene object
//
XScene* XLayer::getSceneX() {
  return (XScene*) getParent();
}


NS_END(fusii)

