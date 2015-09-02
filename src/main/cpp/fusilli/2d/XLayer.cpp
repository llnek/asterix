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

void XLayer::Setup() {}

void XLayer::AudioCallback(Ref* sender) {
  auto t = static_cast<MenuItemToggle*>(sender);
  if (t->getSelectedIndex() == 0) {
    //sh.toggleSfx(true);
  } else {
    //sh.toggleSfx(false);
  }
}

void XLayer::CreateAudioIcons(MenuItem*& off, MenuItem*& on) {
  off= MenuItemSprite::create(Sprite::create("#sound_off.png"),
                                   Sprite::create("#sound_off.png"),
                                   Sprite::create("#sound_off.png"));

  on= MenuItemSprite::create(Sprite::create("#sound_on.png"),
                              Sprite::create("#sound_on.png"),
                              Sprite::create("#sound_on.png"));
}

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

void XLayer::centerAtlasImage(const string& frame,const string& atlas) {
    let bg= new cc.Sprite(frame),
    cw = ccsx.center();
    bg.setPosition(cw);
    if (!!atlas) {
      this.addAtlasItem(atlas, bg);
    } else {
      this.addItem(bg);
    }
}

  /**
   * Center an image.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method centerImage
   * @param {String} frame
   */
  centerImage(frame) {
    this.centerAtlasImage(frame);
  },

  /**
   * Add an image chosen from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addAtlasFrame
   * @param {String} frame
   * @param {cc.Point} pos
   * @param {String} atlas
   */
  addAtlasFrame(frame,pos, atlas) {
    let tt= new cc.Sprite(frame);
    tt.setPosition(pos);
    if (!!atlas) {
      this.addAtlasItem(atlas, tt);
    } else {
      this.addItem(tt);
    }
  },

  /**
   * Add an image.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addFrame
   * @param {String} frame
   * @param {cc.Point} pos
   */
  addFrame(frame,pos) {
    this.addAtlasFrame(frame, pos);
  },

  /**
   * Get the atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method getAtlas
   * @param {String} name
   * @return {cc.SpriteBatchNode}
   */
  getAtlas(name) {
    return this.atlases[name || ""];
  },

  /**
   * Remove all children from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasAll
   * @param {String} atlas
   * @param {Boolean} c
   */
  removeAtlasAll(atlas, c) {
    const a=this.getAtlas(atlas);
    if (!!a) { a.removeAllChildren(c || true); }
  },

  /**
   * Remove child from this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAtlasItem
   * @param {String} atlas
   * @param {String} n - child
   * @param {Boolean} c
   */
  removeAtlasItem(atlas, n,c) {
    if (!!n) {
      n.removeFromParent(c);
    }
  },

  /**
   * Remove all children.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeAll
   * @param {Boolean} c
   */
  removeAll(c) {
    this.removeAllChildren(c);
  },

  /**
   * Remove a child.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method removeItem
   * @param {Object} n
   * @param {Boolean} c
   */
  removeItem(n,c) {
    if (!!n) {
      n.removeFromParent(c);
    }
  },

  /**
   * Add a child to this atlas.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addAtlasItem
   * @param {String} atlas
   * @param {Object} n
   * @param {Number} zx
   * @param {Number} tag
   */
  addAtlasItem(atlas, n, zx, tag) {
    let p= this.getAtlas(atlas),
    pzx = zx,
    ptag = tag;

    if (! sjs.echt(pzx)) {
      pzx = this.lastZix;
    }

    if (! sjs.echt(ptag)) {
      ptag = ++this.lastTag;
    }

    if (p instanceof cc.SpriteBatchNode &&
        n instanceof cc.Sprite) {
      n.setBatchNode(p);
    }

    p.addChild(n, pzx, ptag);
  },

  /**
   * Add a child.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method addChild
   * @param {Object} n - child
   * @param {Number} zx
   * @param {Number} tag
   */
  addItem(n,zx,tag) {
    let pzx = zx,
    ptag = tag;

    if (! sjs.echt(pzx)) {
      pzx = this.lastZix;
    }

    if (! sjs.echt(ptag)) {
      ptag = ++this.lastTag;
    }

    this.addChild(n, pzx, ptag);
  },
  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method incIndexZ
   * @param {cc.Scene} par
   */
  incIndexZ() {
    ++this.lastZix;
  },
  /**
   * Remember the parent scene object.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method setParentScene
   * @param {cc.Scene} par
   */
  setParentScene(par) {
    this.ptScene=par;
  },

  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method scene
   * @return {cc.Scene}
   */
  scene() {
    return ptScene;
  },

  /**
   * Init.
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method init
   */
  init() {
    this._super();
    this.setup();
  },

  /**
   * @memberof module:zotohlab/asx/scenes~XLayer
   * @method ctor
   * @param {Object} options
   */
  ctor(options) {
    this.options = options || {};
    this._super();
    this.lastTag= 0;
    this.lastZix= 0;
    this.atlases= {};
  }

});



NS_AX_END
//////////////////////////////////////////////////////////////////////////////
//EOF
