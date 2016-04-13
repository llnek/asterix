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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "audio/include/SimpleAudioEngine.h"
#include "platform/CCCommon.h"
#include "base/ccTypes.h"
#include "Primitives.h"
#include "COMP.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
namespace ccsx {

  void testCollide(not_null<ecs::Node*>, not_null<ecs::Node*>);
  void testCollide(not_null<f::FPool*>, not_null<f::FPool*>);
  void testCollide(not_null<f::FPool*>, not_null<ecs::Node*>);

  void resolveNodes(not_null<f::FPool*>);

  void resurrect(not_null<ecs::Node*>, float x, float y);
  void resurrect(not_null<ecs::Node*>);
  void hibernate(not_null<ecs::Node*>);
  void kumakaze(not_null<ecs::Node*>);

  void resolveElastic(not_null<c::Node*> rA,
      CCT_V2 &velA,
      not_null<c::Node*> rB, CCT_V2 &velB);

  const CCT_SZ scaleSize(const CCT_SZ&, float scale);

  c::Dictionary* readXmlAsDict(const sstr &fpath);
  c::Array* readXmlAsList(const sstr &fpath);
  j::json readJson(const sstr &fpath);

  const c::Color3B colorRGB(const sstr &hexcolor);

  const s_arr<c::MenuItem*,2> createAudioIcons();
  const s_arr<c::MenuItem*,2> reifyAudioIcons();

  void sfxMusic(const sstr &music, bool repeat);
  void sfxPlay(const sstr &sound);

  void resumeEffects();
  void pauseEffects();
  void stopEffects();

  void resumeMusic();
  void pauseMusic();
  void stopMusic();

  void resumeAudio();
  void pauseAudio();
  void stopAudio();

  bool isDesktop();

  const CCT_PT normalize(not_null<c::Node*>, not_null<c::Node*> );
  float calcDist(not_null<c::Node*>,not_null<c::Node*>);

  owner<c::Animation*> createAnimation(
      float dpu,
      bool restore=false, int loops= -1);

  c::SpriteBatchNode* reifySpriteBatch(const sstr &name);

  const CCT_SZ calcOffsets(const CCT_SZ &sz);

  c::MenuItem* reifyMenuText(const sstr &fnt, float pt,
      const sstr &txt,
      const c::Color3B &c= c::Color3B::WHITE);

  c::MenuItem* reifyMenuText(const sstr &fnt, const sstr &txt,
      const c::Color3B &c= c::Color3B::WHITE);

  c::MenuItem* createMenuBtn(const sstr &n);
  c::MenuItem* createMenuBtn( const sstr &n, const sstr &s);

  c::MenuItem* reifyMenuBtn(const sstr &n);
  c::MenuItem* reifyMenuBtn( const sstr &n, const sstr &s);

  c::Menu* mkMenu(const s_vec<c::MenuItem*>&, bool vert = false, float pad= 10);
  c::Menu* mkHMenu(const s_vec<c::MenuItem*>&, float pad= 10);
  c::Menu* mkVMenu(const s_vec<c::MenuItem*>&, float pad= 10);
  c::Menu* mkMenu(c::MenuItem*);

  bool pointInBox(const Box4 &box, float x,  float y);
  const c::Color3B white();
  const c::Color3B black();

  void randomPos(not_null<c::Node*>);

  bool collide(not_null<c::Node*>, not_null<c::Node*> );

  void setDevRes(float width, float height,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool pointInBox(const Box4 &box, const CCT_PT &pos);
  bool isIntersect(const Box4&, const Box4&);
  bool isPortrait();

  bool outOfBound(not_null<c::Node*>, const Box4 &B);
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
  c::Sprite* reifySprite(const sstr &frameName);
  c::Sprite* createSprite(const sstr &frameName);

  float deltaX(not_null<c::Node*> a, not_null<c::Node*> b);
  float deltaY(not_null<c::Node*> a, not_null<c::Node*> b);
  bool isTapped(not_null<c::Node*>, const CCT_PT&);

  const Box4 bbox4(not_null<c::Node*>);

  void push(not_null<c::Scene*>, float delay);
  void push(not_null<c::Scene*>);
  void pushEx(not_null<c::Scene*>);

  inline void pop() { CC_DTOR()->popScene(); }

  void run(not_null<c::Scene*>, float delay);
  void run(not_null<c::Scene*>);
  void runEx(not_null<c::Scene*>);
  void prelude();

  const CCT_PT calcXY(float angle, float hypot);
  float degToRad(float);

  VOIDFN throttle(VOIDFN func, int waitMillis);
  long long timeInMillis();
  bool isTransitioning();

  const CCT_SZ calcSize(const sstr &frame);

  const CCT_SZ halfHW(not_null<c::Node*>);
  const CCT_RT bbox(not_null<c::Node*>);

  float getContentHeight(not_null<c::Node*>);
  float getContentWidth(not_null<c::Node*>);

  float getHeight(not_null<c::Node*>);
  float getWidth(not_null<c::Node*>);

  float getLeft(not_null<c::Node*>);
  float getRight(not_null<c::Node*>);
  float getTop(not_null<c::Node*>);
  float getBottom(not_null<c::Node*>);

  const CCT_PT center();
  float centerX();
  float centerY();

  float screenHeight();
  float screenWidth();

  const CCT_PT vboxMID(const Box4&);
  const CCT_RT visRect();
  const CCT_SZ visSize();
  const Box4 visBox();

  const CCT_PT scenter();
  const CCT_SZ screen();

  bool traceEnclosure(float dt, const Box4 &bbox,
      const Box4 &rect, const CCT_V2 &vel,
      CCT_PT &outPos, CCT_V2 &outVel);

  void mergeDict(c::Dictionary *src, c::Dictionary *d2);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. ship.png)
   */
  c::SpriteFrame* getSpriteFrame(const sstr &frameid, const CCT_RT&);
  c::SpriteFrame* getSpriteFrame(const sstr &frameid);

  const CCT_PT anchorC();
  const CCT_PT anchorTL();
  const CCT_PT anchorT();
  const CCT_PT anchorTR();
  const CCT_PT anchorR();
  const CCT_PT anchorL();
  const CCT_PT anchorBR();
  const CCT_PT anchorB();
  const CCT_PT anchorBL();

  const CCT_PT clamp(const CCT_PT &loc, const CCT_SZ &sz, const Box4 &world);
  const CCT_PT clamp(const CCT_PT &cur, const Box4&);

  float randFloat(float upper);
  float rand();
  int randInt(int upper);
  int randSign();

};


NS_END




