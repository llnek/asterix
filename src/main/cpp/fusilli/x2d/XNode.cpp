// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "renderer/CCTextureCache.h"
#include "2d/CCMenuItem.h"
#include "2d/CCSprite.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
#include "XNode.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// add a toggle for sound on & off
c::Menu* XNode::addAudioIcons(const s_arr<c::MenuItem*,2> &audios,
    const CCT_PT &anchor, const CCT_PT &pos) {

  c::Vector<c::MenuItem*> items;
  items.pushBack(audios[1]);
  items.pushBack(audios[0]);

  auto cb = [](c::Ref *r) {
    auto t= SCAST(c::MenuItemToggle*, r);
    auto b= t->getSelectedIndex() == 0;
    XCFG()->toggleAudio(b);
  };

  // the toggle
  auto audio = c::MenuItemToggle::createWithCallback(cb, items);
  audio->setSelectedIndex( XCFG()->hasAudio() ? 0 : 1);
  audio->setAnchorPoint(anchor);

  // need null to end var-args
  auto menu= c::Menu::create(audio, CC_NIL);
  menu->setPosition(pos);
  _self->addChild(menu);
  return menu;
}

///////////////////////////////////////////////////////////////////////////////
//
void XNode:: setCtx(SCTX *x, bool clean) {
  if (clean) { mc_del_ptr(_context); }
  _context=x;
}

///////////////////////////////////////////////////////////////////////////////
//
SCTX* XNode::ejectCtx() {
  auto x= _context;
  S__NIL(_context)
  return x;
}

//////////////////////////////////////////////////////////////////////////////
//
XNode::~XNode() {
  mc_del_ptr(_context);
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XNode::gets(const sstr &key, const s_vec<sstr> &pms) {
  return XCFG()->getL10NStr(key, pms);
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr XNode::gets(const sstr &key) {
  return XCFG()->getL10NStr(key);
}

//////////////////////////////////////////////////////////////////////////////
//
c::SpriteBatchNode* XNode::regoAtlas(const sstr &name, int zx) {
  auto a= cx::reifySpriteBatch(name);
  _atlases.insert(S__PAIR(sstr, c::SpriteBatchNode*, name, a));
  _self->addChild(a, zx);
  return a;
}

//////////////////////////////////////////////////////////////////////////////
// Get the atlas
//
c::SpriteBatchNode* XNode::getAtlas(const sstr &name) {
  auto it= _atlases.find(name);
  if (it != _atlases.end()) {
    return it->second;
  } else {
    return CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Add an image chosen from this atlas
//
void XNode::addAtlasFrame(const sstr &atlas,
                          const sstr &frame,
                          const CCT_PT &pos, int z, int tag) {
  auto tt= cx::reifySprite(frame);
  tt->setPosition(pos);
  addAtlasItem(atlas, tt, z, tag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XNode::addAtlasFrame(const sstr &atlas,
                          const sstr &frame,
                          const CCT_PT &pos) {
  addAtlasFrame(atlas, frame,pos,0,0);
}

//////////////////////////////////////////////////////////////////////////////
// Add an image
void XNode::addFrame(const sstr &frame, const CCT_PT &pos, int z, int tag) {
  auto tt= cx::reifySprite(frame);
  tt->setPosition(pos);
  addItem(tt, z, tag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XNode::addFrame(const sstr &frame, const CCT_PT &pos) {
  addFrame(frame, pos,0,0);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child to this atlas
void XNode::addAtlasItem(const sstr &atlas, not_null<c::Node*> n, int z, int tag) {

  auto ss = DCAST(c::Sprite*, n.get());
  auto p= getAtlas(atlas);

  CCASSERT(p != nullptr, "atlas cannot be null");

  if (N_NIL(ss)) { ss->setBatchNode(p); }
  p->addChild(n.get(), z, tag);
}

//////////////////////////////////////////////////////////////////////////////
//
void XNode::addAtlasItem(const sstr &atlas, not_null<c::Node*> n) {
  addAtlasItem(atlas, n, 0,0);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child
void XNode::addItem(not_null<c::Node*> n, int z, int tag) {
  _self->addChild(n.get(), z, tag);
}

//////////////////////////////////////////////////////////////////////////////
// Add a child
void XNode::addItem(not_null<c::Node*> n) {
  addItem(n, 0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Sprite* XNode::centerImage(const sstr &name, int z) {
  auto t= CC_TCAC()->addImage(XCFG()->getImage(name));
  auto s= c::Sprite::createWithTexture(t);
  s->setPosition(cx::center());
  _self->addChild(s,z);
  return s;
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children from this atlas
//
void XNode::removeAtlasAll(const sstr &atlas) {
  auto a=getAtlas(atlas);
  if (N_NIL(a)) { a->removeAllChildren(); }
}

//////////////////////////////////////////////////////////////////////////////
// Remove all children
//
void XNode::removeAll() {
  _self->removeAllChildren();
}

//////////////////////////////////////////////////////////////////////////////
// Remove a child
//
void XNode::removeItem(c::Node *n) {
  if (N_NIL(n)) {
    n->removeFromParent();
  }
}


NS_END


