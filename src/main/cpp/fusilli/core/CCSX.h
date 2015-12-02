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

#if !defined(__CCSX_H__)
#define __CCSX_H__

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

  c::Dictionary* readXmlAsDict(const stdstr& fpath);
  c::Array* readXmlAsList(const stdstr& fpath);

  void reifyAudioIcons(c::MenuItem*& off, c::MenuItem*& on);
  void resolveElastic(not_null<ComObj*>, not_null<ComObj*>);

  const c::Color3B colorRGB(const stdstr& hexcolor);

  void sfxMusic(const stdstr& music, bool repeat);
  void sfxPlay(const stdstr& sound);
  void pauseAudio();

  bool isDesktop();

  c::MenuItem* reifyMenuBtn(const stdstr& n);

  c::MenuItem* reifyMenuBtn(
      const stdstr& n,
      const stdstr& s, const stdstr& d);

  c::Menu* mkMenu(c::MenuItem*);

  c::Menu* mkMenu(const s::vector<c::MenuItem*>& items,
      bool vert = false, float padding= 10.0f);

  bool pointInBox(const Box4& box, float x,  float y);
  const c::Color3B white();
  const c::Color3B black();

  bool collideN(not_null<c::Node*>, not_null<c::Node*> );
  bool collide(not_null<ComObj*>, not_null<ComObj*>);

  void setDevRes(float width, float height,
      ResolutionPolicy pcy = ResolutionPolicy::NO_BORDER);

  bool isPortrait();

  bool outOfBound(not_null<ComObj*>, const Box4& B);
  bool outOfBound(const Box4& src, const Box4& B);

  c::DelayTime* reifyTimer(not_null<c::Node*>, float millis);
  void undoTimer(not_null<c::DelayTime*>);
  bool timerDone(not_null<c::DelayTime*>);

  c::Label* reifyBmfLabel(const stdstr& font, const stdstr& text);

  c::Label* reifyBmfLabel(float x, float y,
    const stdstr& font,
    const stdstr& text);

  c::Sprite* reifySprite(const stdstr& frameName);

  const Box4 bbox4B4(not_null<ComObj*>);
  const Box4 bbox4(not_null<c::Node*>);

  void runScene(not_null<c::Scene*>, float delay);
  void runScene(not_null<c::Scene*>);

  const c::Size scaleSize(const c::Size& z, float scale);

  bool isTransitioning();

  const c::Size calcSize(const stdstr& frame);

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

  bool traceEnclosure(float dt, const Box4& bbox,
      const c::Rect& rect, const c::Vec2& vel,
      c::Vec2& outPos, c::Vec2& outVel);

  void mergeDict(c::Dictionary* src, c::Dictionary* d2);

  /**
   * Get the sprite from the frame cache using
   * its id (e.g. ship.png)
   */
  c::SpriteFrame* getSpriteFrame(const stdstr& frameid);

  const c::Vec2 anchorC();
  const c::Vec2 anchorTL();
  const c::Vec2 anchorT();
  const c::Vec2 anchorTR();
  const c::Vec2 anchorR();
  const c::Vec2 anchorL();
  const c::Vec2 anchorBR();
  const c::Vec2 anchorB();
  const c::Vec2 anchorBL();

};

NS_END(fusii)
#endif



