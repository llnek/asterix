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

#include "audio/include/SimpleAudioEngine.h"
#include "platform/CCGLView.h"
#include "platform/CCGL.h"
#include "XConfig.h"
#include "CCSX.h"
#include "App.h"

NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages() {
  return 0; //flag for packages manager
}

//////////////////////////////////////////////////////////////////////////////
//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void App::initGLContextAttrs() {

  //set OpenGL context attributions,now can only set six attributions:
  //red,green,blue,alpha,depth,stencil
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
  c::GLView::setGLContextAttrs(glContextAttrs);

}

//////////////////////////////////////////////////////////////////////////////
//
bool App::applicationDidFinishLaunching() {

  auto glview = CC_DTOR()->getOpenGLView();
  auto sz = XCFG()->gameSize();

  if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    glview = c::GLViewImpl::createWithRect(appName, c::Rect(0, 0, sz.width, sz.height));
#else
    glview = c::GLViewImpl::create(appName);
#endif
    CC_DTOR()->setOpenGLView(glview);
  }

  register_all_packages();

  preLaunch(sz);

  //CCLOG("about to run start scene");
  // run
  CC_DTOR()->runWithScene( XCFG()->prelude());

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void App::preLaunch(const c::Size &dz) {

  auto glview = CC_DTOR()->getOpenGLView();
  auto dispFPS= CC_CSV(c::Bool,"showFPS");
  auto fps = CC_CSV(c::Integer,"FPS");

  auto portrait = XCFG()->isPortrait();
  auto fz = glview->getFrameSize();

  s_vec<sstr> searchPaths;
  c::Size largeSize;
  c::Size medSize;
  c::Size smallSize;
  auto spath="rd";

  // Set the design resolution
  cx::setDevRes(dz.width, dz.height, XCFG()->policy());

  // FPS. dft is 1.0/60 if you don't call this
  CC_DTOR()->setAnimationInterval(1.0f / fps);

  // turn on display FPS?
  CC_DTOR()->setDisplayStats( dispFPS);

  if (portrait) {
    largeSize = c::Size(1536, 2048);
    medSize = c::Size(768, 1024);
    smallSize = c::Size(320, 480);
  } else {
    largeSize = c::Size(2048, 1536);
    medSize = c::Size(1024, 768);
    smallSize = c::Size(480, 320);
  }

  // if the frame's height is larger than
  // the height of medium size
  if (fz.height > medSize.height) {
    CC_DTOR()->setContentScaleFactor(
        MIN(largeSize.height/dz.height, largeSize.width/dz.width));
    spath= "rd";
  }
  // if the frame's height is larger than
  // the height of small size.
  else if (fz.height > smallSize.height) {
    CC_DTOR()->setContentScaleFactor(
        MIN(medSize.height/dz.height, medSize.width/dz.width));
    spath= "hd";
  }
  // if the frame's height is smaller than the height of medium size.
  else {
    CC_DTOR()->setContentScaleFactor(
        MIN(smallSize.height/dz.height, smallSize.width/dz.width));
    spath="sd";
  }

  CCLOG("frame size, width=%d, height=%d", (int)fz.width, (int)fz.height);
  CCLOG("game size, width=%d, height=%d", (int)dz.width, (int)dz.height);
  CCLOG("image search path=%s", spath);
  CCLOG("sound search path=%s", "sfx");
  CCLOG("content scale factor=%f", CC_DTOR()->getContentScaleFactor());

  searchPaths.push_back(spath);
  c::FileUtils::getInstance()->setSearchPaths(searchPaths);

  XCFG()->handleResolution(fz);
  XCFG()->runOnce();

  initAudio();
}

//////////////////////////////////////////////////////////////////////////////
//
void App::initAudio() {
  auto a = XCFG()->getEffectFiles();
  F__LOOP(it, a) {
    auto &fp = *it;
    CCLOG("preloading sound effect: %s", fp.c_str());
    den::SimpleAudioEngine::getInstance()->preloadEffect(fp.c_str());
  }
  a= XCFG()->getMusicFiles();
  F__LOOP(it, a) {
    auto &fp = *it;
    CCLOG("preloading music: %s", fp.c_str());
    den::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fp.c_str());
  }

  den::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
  den::SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
}

//////////////////////////////////////////////////////////////////////////////
// This function will be called when the app is inactive
// When comes a phone call,it's be invoked too
void App::applicationDidEnterBackground() {

  CC_DTOR()->stopAnimation();
  den::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
// this function will be called when the app is active again
void App::applicationWillEnterForeground() {

  CC_DTOR()->startAnimation();
  den::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}



NS_END



