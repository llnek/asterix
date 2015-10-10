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

#include "audio/include/SimpleAudioEngine.h"
#include "platform/CCGLView.h"
#include "platform/CCGL.h"
#include "Primitives.h"
#include "XConfig.h"
#include "App.h"

NS_ALIAS(den, CocosDenshion)
NS_BEGIN(fusii)

static c::Size largeSize;
static c::Size mediumSize;
static c::Size smallSize;

//////////////////////////////////////////////////////////////////////////////
// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages() {
  return 0; //flag for packages manager
}

//////////////////////////////////////////////////////////////////////////////
//
App::App() {
}

//////////////////////////////////////////////////////////////////////////////
//
App::~App() {
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
  auto sz = XCFG()->GetGameSize();

  CCLOG("game size, width=%d, height=%d", (int)sz.width, (int)sz.height);

  if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    glview = c::GLViewImpl::createWithRect("z", c::Rect(0, 0, sz.width, sz.height));
#else
    glview = c::GLViewImpl::create("z");
#endif
    CC_DTOR()->setOpenGLView(glview);
  }

  PreLaunch(sz);

  register_all_packages();

  //CCLOG("about to run start scene");
  // run
  CC_DTOR()->runWithScene( XCFG()->StartWith());

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void App::PreLaunch(const c::Size& dz) {

  auto glview = CC_DTOR()->getOpenGLView();
  auto dispFPS= CstVal<c::Bool>("showFPS");
  auto fps = CstVal<c::Integer>("FPS");
  auto portrait = dz.height > dz.width;
  auto fz = glview->getFrameSize();
  s::vector<stdstr> searchPaths;

  CCLOG("frame size, width=%d, height=%d", (int)fz.width, (int)fz.height);

  // set FPS. default is 1.0/60 if you don't call this
  CC_DTOR()->setAnimationInterval(1.0 / fps->getValue());

  // turn on display FPS?
  CC_DTOR()->setDisplayStats( dispFPS->getValue());

  if (portrait) {
    largeSize = c::Size(1536, 2048);
    mediumSize = c::Size(768, 1024);
    smallSize = c::Size(320, 480);
  } else {
    largeSize = c::Size(2048, 1536);
    mediumSize = c::Size(1024, 768);
    smallSize = c::Size(480, 320);
  }

  // if the frame's height is larger than
  // the height of medium size
  if (fz.height > mediumSize.height) {
    CC_DTOR()->setContentScaleFactor(
        MIN(largeSize.height/dz.height, largeSize.width/dz.width));
    searchPaths.push_back("hdr");
  }
  // if the frame's height is larger than
  // the height of small size.
  else if (fz.height > smallSize.height) {
    CC_DTOR()->setContentScaleFactor(
        MIN(mediumSize.height/dz.height, mediumSize.width/dz.width));
    searchPaths.push_back("hd");
  }
  // if the frame's height is smaller than the height of medium size.
  else {
    CC_DTOR()->setContentScaleFactor(
        MIN(smallSize.height/dz.height, smallSize.width/dz.width));
    searchPaths.push_back("sd");
  }

  c::FileUtils::getInstance()->setSearchPaths(searchPaths);

  // Set the design resolution
  glview->setDesignResolutionSize(
    dz.width, dz.height, XCFG()->GetPolicy());

  XCFG()->HandleResolution(fz);
  XCFG()->RunOnce();

  InitAudio();
}

//////////////////////////////////////////////////////////////////////////////
//
void App::InitAudio() {
  auto a = XCFG()->GetEffectFiles();
  for (auto it = a.begin(); it != a.end(); ++it) {
    auto fp = *it;
    den::SimpleAudioEngine::getInstance()->preloadEffect(fp.c_str());
  }
  a= XCFG()->GetMusicFiles();
  for (auto it = a.begin(); it != a.end(); ++it) {
    auto fp = *it;
    den::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fp.c_str());
  }
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




NS_END(fusii)

