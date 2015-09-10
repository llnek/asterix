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
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
SpriteBatchNode* XLayer::RegoAtlas(const string& name, int z, int tag) {
  auto cfg = Config::GetInstance();
  auto img= cfg->GetImage(name);
  auto a= SpriteBatchNode::createWithTexture(
      TextureCache::getInstance()->addImage(img));
  if (tag == INT32_MIN) {
    tag = ++lastTag;
  }
  if (z == INT32_MIN) {
    z = lastZix;
  }
  addChild(a, z, tag);
  atlases.insert(pair<string,Node*>(name,a));
  a->retain(); // for the map
  return a;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XLayer::Realize() { return this; }

void XLayer::PKInput() {}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AudioCallback(Ref* sender) {
  auto t = SCAST(MenuItemToggle*,sender);
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
  //
  auto menu= Menu::create(audio);
  menu->setPosition(pos);
  AddItem(menu);
}

//void XLayer::OnQuit() {
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
//}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterAtlasImage(const string& atlas, const string& frame) {
  AddAtlasFrame(atlas, CCSX::Center(), frame);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::CenterImage(const string& frame) {
  AddFrame(frame, CCSX::Center());
}

  /**
   * Add an image chosen from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addAtlasFrame
   * @param {String} frame
   * @param {cc.Point} pos
   * @param {String} atlas
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddAtlasFrame(const string& atlas,
                           const Vec2& pos,
                           const string& frame) {
  auto tt= Sprite::(frame);
  tt->setPosition(pos);
  AddAtlasItem(atlas, tt);
}

  /**
   * Add an image.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addFrame
   * @param {String} frame
   * @param {cc.Point} pos
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddFrame(const string& frame, const Vec2& pos) {
  auto tt= Sprite::(frame);
  tt->setPosition(pos);
  AddItem(frame);
}

  /**
   * Get the atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method getAtlas
   * @param {String} name
   * @return {cc.SpriteBatchNode}
   */
//////////////////////////////////////////////////////////////////////////////
//
SpriteBatchNode* XLayer::GetAtlas(const string& name) {
  auto it= atlases.find(name);
  if (it != atlases.end()) {
    return SCAST(SpriteBatchNode*, it->second);
  } else {
    return nullptr;
  }
}

  /**
   * Remove all children from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasAll
   * @param {String} atlas
   * @param {Boolean} c
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::RemoveAtlasAll(const string& atlas, bool c) {
  auto a=GetAtlas(atlas);
  if (NNP(a)) { a->removeAllChildren(c); }
}

  /**
   * Remove child from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasItem
   * @param {String} atlas
   * @param {String} n - child
   * @param {Boolean} c
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::RemoveAtlasItem(Node* n, bool c) {
  if (NNP(n)) {
    n->removeFromParent(c);
  }
}

  /**
   * Remove all children.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAll
   * @param {Boolean} c
   */
//////////////////////////////////////////////////////////////////////////////
//
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
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::RemoveItem(Node* n, bool c) {
  if (NNP(n)) {
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
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddAtlasItem(const string& atlas, Node* n, int zx, int tag) {
  auto ptag = tag == INT32_MIN ? ++lastTag : tag;
  auto pzx = zx == INT32_MIN ? lastZix : zx;
  auto p= GetAtlas(atlas);

  if (DCAST(SpriteBatchNode*,p) != nullptr &&
      DCAST(Sprite*,n) != nullptr) {
    n->setBatchNode(p);
  }

  if (NNP(p)) { p->addChild(n, pzx, ptag);  }
}

  /**
   * Add a child.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addChild
   * @param {Object} n - child
   * @param {Number} zx
   * @param {Number} tag
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::AddItem(Node* n, int zx, int tag) {
  auto ptag = tag == INT32_MIN ? ++lastTag : tag;
  auto pzx = zx == INT32_MIN ? lastZix : zx;
  addChild(n, pzx, ptag);
}


  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method incIndexZ
   * @param {cc.Scene} par
   */
//////////////////////////////////////////////////////////////////////////////
//
void XLayer::IncIndexZ() {
  ++lastZix;
}


  /**
   * Remember the parent scene object.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method setParentScene
   * @param {cc.Scene} par
   */
//////////////////////////////////////////////////////////////////////////////
//
Scene* XLayer::GeScene() {
  return SCAST(Scene*,getParent());
}


  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method ctor
   * @param {Object} options
   */
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
MenuItem* XLayer::CreateMenuBtn(const string& n, const string& s,
    const string& d) {
  return MenuItemSprite::create(Sprite::create(n),
                                Sprite::create(s),
                                Sprite::create(d));
}


NS_END(fusilli)

