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

#include "cocos2d.h"
#include "XLayer.h"
USING_NS_STD;
USING_NS_CC;
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////////
//
const string& XLayer::RTTI() {
  return m_type;
}

//////////////////////////////////////////////////////////////////////////////
//
SpriteBatchNode* XLayer::RegoAtlas(const string& name, int z, int tag) {
  auto a= SpriteBatchNode::createWithTexture(
      TextureCache::getInstance()->addImage(name));
  if (tag == -911) {
    tag = ++m_lastTag;
  }
  if (z == -911) {
    z = m_lastZix;
  }
  addChild(a, z, tag);
  m_atlases.setObject(a, name);

  return a;
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::Setup() {}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AudioCallback(Ref* sender) {
  auto t = static_cast<MenuItemToggle*>(sender);
  if (t->getSelectedIndex() == 0) {
    //sh.toggleSfx(true);
  } else {
    //sh.toggleSfx(false);
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

  Vector<MenuItem*> items= {on,off};
  auto audio = MenuItemToggle::createWithCallback(
      CC_CALLBACK_1(XLayer::AudioCallback, this), items);

  audio->setAnchorPoint(anchor);
  //xcfg.sound.open ? 0 : 1
  audio->setSelectedIndex(1);

  auto menu= Menu::create(audio);
  menu->setPosition(pos);
  addItem(menu);
}

void XLayer::OnQuit() {
//    let ss= sh.protos[xcfg.game.start],
//    yn= sh.protos[sh.ptypes.yn],
//    dir = cc.director;
//
//    dir.pushScene( yn.reify({
//      onback() {
//        dir.popScene();
//      },
//      yes() {
//        //sh.sfxPlay('game_quit');
//        dir.popToRootScene();
//        ccsx.runScene(ss.reify());
//      }
//    }));
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterAtlasImage(const string& frame, const string& atlas) {
  auto bg= Sprite::create(frame);
  auto cw = CCSX::Center();
  bg.setPosition(cw);
  if (atlas.length() > 0) {
    AddAtlasItem(atlas, bg);
  } else {
    AddItem(bg);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterImage(const string& frame) {
  CenterAtlasImage(frame);
}

  /**
   * Add an image chosen from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addAtlasFrame
   * @param {String} frame
   * @param {cc.Point} pos
   * @param {String} atlas
   */
void XLayer::AddAtlasFrame(const string& frame,
                           const Vec2& pos,
                           const string& atlas) {
  auto tt= Sprite::(frame);
  tt.setPosition(pos);
  if (atlas.length() > 0) {
    AddAtlasItem(atlas, tt);
  } else {
    AddItem(tt);
  }
}

  /**
   * Add an image.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addFrame
   * @param {String} frame
   * @param {cc.Point} pos
   */
void XLayer::AddFrame(const string& frame, const Vec2& pos) {
  AddAtlasFrame(frame, pos);
}

  /**
   * Get the atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method getAtlas
   * @param {String} name
   * @return {cc.SpriteBatchNode}
   */
SpriteBatchNode* XLayer::GetAtlas(const string& name) {
  return static_cast<SpriteBatchNode*>(m_atlases.objectForKey(name));
}

  /**
   * Remove all children from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasAll
   * @param {String} atlas
   * @param {Boolean} c
   */
void XLayer::RemoveAtlasAll(const string& atlas, bool c) {
  auto a=GetAtlas(atlas);
  if (a != nullptr) { a->removeAllChildren(c); }
}

  /**
   * Remove child from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasItem
   * @param {String} atlas
   * @param {String} n - child
   * @param {Boolean} c
   */
void XLayer::RemoveAtlasItem(Node* n, bool c) {
  if (n != nullptr) {
    n->removeFromParent(c);
  }
}

  /**
   * Remove all children.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAll
   * @param {Boolean} c
   */
void XLayer::RemoveAll(bool c) {
  RemoveAllChildren(c);
}

  /**
   * Remove a child.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeItem
   * @param {Object} n
   * @param {Boolean} c
   */
void XLayer::RemoveItem(Node* n, bool c) {
  if (n != nullptr) {
    n->removeFromParent(c);
  }
}

  /**
   * Add a child to this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addAtlasItem
   * @param {String} atlas
   * @param {Object} n
   * @param {Number} zx
   * @param {Number} tag
   */
void XLayer::AddAtlasItem(const string& atlas, Node* n, int zx, int tag) {
  auto ptag = tag == -911 ? ++m_lastTag : tag;
  auto pzx = zx == -911 ? m_lastZix : zx;
  auto p= GetAtlas(atlas);

  if (dynamic_cast<SpriteBatchNode*>(p) != nullptr &&
      dynamic_cast<Sprite*>(n) != nullptr) {
    n->setBatchNode(p);
  }

  if (p != nullptr) { p->addChild(n, pzx, ptag);  }
}

  /**
   * Add a child.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addChild
   * @param {Object} n - child
   * @param {Number} zx
   * @param {Number} tag
   */
void XLayer::AddItem(Node* n, int zx, int tag) {
  auto ptag = tag == -911 ? ++m_lastTag : tag;
  auto pzx = zx == -911 ? m_lastZix : zx;

  AddChild(n, pzx, ptag);
}


  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method incIndexZ
   * @param {cc.Scene} par
   */
void XLayer::IncIndexZ() {
  ++m_lastZix;
}


  /**
   * Remember the parent scene object.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method setParentScene
   * @param {cc.Scene} par
   */
void XLayer::SetParentScene(Scene* par) {
  m_ptScene=par;
}

  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method scene
   * @return {cc.Scene}
   */
Scene* XLayer::Scene() {
  return m_ptScene;
}


  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method ctor
   * @param {Object} options
   */

XLayer::XLayer() {
  m_lastTag= 0;
  m_lastZix= 0;
}


MenuItem* XLayer::CreateMenuButton(const string& n, const string& s,
    const string& d, cb) {
  return MenuItemSprite::create(Sprite::create(n),
                                Sprite::create(s),
                                Sprite::create(d),
                                cb);
}


NS_FI_END
