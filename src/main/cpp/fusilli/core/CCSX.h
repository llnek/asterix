// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once

#include "platform/CCCommon.h"
#include "base/ccTypes.h"
#include "Primitives.h"
#include "ComObj.h"
NS_BEGIN(fusii)

#define HHZ(z) z.height * 0.5f
#define HWZ(z) z.width * 0.5f

//////////////////////////////////////////////////////////////////////////////
//
namespace ccsx {

  void resolveElastic(not_null<ComObj*>, not_null<ComObj*>);

  const c::Size scaleSize(const c::Size &z, float scale);

  c::Dictionary* readXmlAsDict(const sstr &fpath);
  c::Array* readXmlAsList(const sstr &fpath);
  j::json readJson(const sstr &fpath);

  const c::Color3B colorRGB(const sstr &hexcolor);
  const s_arr<c::MenuItem*,2> reifyAudioIcons();

  void sfxMusic(const sstr &music, bool repeat);
  void sfxPlay(const sstr &sound);

  void resumeEffects();
  void resumeMusic();
  void pauseEffects();
  void pauseMusic();

  void resumeAudio();
  void pauseAudio();

  bool isDesktop();

  c::MenuItem* createMenuBtn(const sstr &n);
  c::MenuItem* createMenuBtn( const sstr &n, const sstr &s);

  c::MenuItem* reifyMenuBtn(const sstr &n);
  c::MenuItem* reifyMenuBtn( const sstr &n, const sstr &s);

  c::Menu* mkMenu(const s_vec<c::MenuItem*>&, bool vert = false, float padding= 10.0f);
  c::Menu* mkHMenu(const s_vec<c::MenuItem*>&, float padding= 10.0f);
  c::Menu* mkVMenu(const s_vec<c::MenuItem*>&, float padding= 10.0f);
  c::Menu* mkMenu(c::MenuItem*);

  bool pointInBox(const Box4 &box, float x,  float y);
  const c::Color3B white();
  const c::Color3B black();

  bool collideN(not_null<c::Node*>, not_null<c::Node*> );
  bool collide(not_null<ComObj*>, not_null<ComObj*>);

  void setDevRes(float width, float height,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool pointInBox(const Box4 &box, const c::Vec2 &pos);
  bool isIntersect(const Box4&, const Box4&);
  bool isPortrait();

  bool outOfBound(not_null<ComObj*>, const Box4 &B);
  bool outOfBound(const Box4 &src, const Box4 &B);

  c::DelayTime* reifyTimer(not_null<c::Node*>, float millis);
  void undoTimer(c::DelayTime*);
  bool timerDone(c::DelayTime*);

  c::Label* reifyLabel(const sstr &font, float sz, const sstr &text="");
  c::Label* reifyLabel(float x, float y,
    const sstr &font,
    float sz,
    const sstr &text="");

  c::Label* reifyBmfLabel(const sstr &font, const sstr &text="");
  c::Label* reifyBmfLabel(float x, float y,
    const sstr &font,
    const sstr &text="");

  c::Sprite* loadSprite(const sstr &fname, const sstr &path= "pics");
  c::Sprite* reifySpriteFromFile(const sstr &frameName);
  c::Sprite* createSprite(const sstr &frameName);

  float deltaX(not_null<c::Node*> a, not_null<c::Node*> b);
  float deltaY(not_null<c::Node*> a, not_null<c::Node*> b);
  bool isClicked(not_null<c::Node*>, const c::Vec2&);

  const Box4 bbox4B4(not_null<ComObj*>);
  const Box4 bbox4(not_null<c::Node*>);
  inline const Box4 bbox4(not_null<ComObj*> c) {
    return bbox4(c->node);
  }

  void run(not_null<c::Scene*>, float delay);
  void run(not_null<c::Scene*>);
  void runEx(not_null<c::Scene*>);
  void prelude();

  const c::Vec2 calcXY(float angle, float hypot);
  float degToRad(float);

  VOIDFN throttle(VOIDFN func, int waitMillis);
  long long currentTimeInMillis();
  bool isTransitioning();

  const c::Size calcSize(const sstr &frame);

  const c::Size halfHW(not_null<ComObj*>);
  const c::Size halfHW(not_null<c::Node*>);

  const c::Rect bbox(not_null<c::Node*>);

  float getScaledHeight(not_null<c::Node*>);
  float getScaledWidth(not_null<c::Node*>);

  float getHeight(not_null<c::Node*>);
  float getWidth(not_null<c::Node*>);

  float getLeft(not_null<c::Node*>);
  float getRight(not_null<c::Node*>);
  float getTop(not_null<c::Node*>);
  float getBottom(not_null<c::Node*>);

  float getLastLeft(not_null<ComObj*> );
  float getLastRight(not_null<ComObj*> );
  float getLastTop(not_null<ComObj*> );
  float getLastBottom(not_null<ComObj*> );

  const c::Vec2 center();
  float centerX();
  float centerY();

  float screenHeight();
  float screenWidth();

  const c::Vec2 vboxMID(const Box4& );
  const c::Rect visRect();
  const Box4 visBox();

  const c::Vec2 scenter();
  const c::Size screen();

  bool traceEnclosure(float dt, const Box4 &bbox,
      const Box4 &rect, const c::Vec2 &vel,
      c::Vec2 &outPos, c::Vec2 &outVel);

  void mergeDict(c::Dictionary *src, c::Dictionary *d2);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. ship.png)
   */
  c::SpriteFrame* getSpriteFrame(const sstr &frameid);

  const c::Vec2 anchorC();
  const c::Vec2 anchorTL();
  const c::Vec2 anchorT();
  const c::Vec2 anchorTR();
  const c::Vec2 anchorR();
  const c::Vec2 anchorL();
  const c::Vec2 anchorBR();
  const c::Vec2 anchorB();
  const c::Vec2 anchorBL();

  const c::Vec2 clamp(const c::Vec2 &loc, const c::Size &sz, const Box4 &world);
  const c::Vec2 clamp(const c::Vec2 &cur, const Box4&);

  float randFloat(float upper);
  float rand();
  int randInt(int upper);
  int randSign();

};


NS_END




