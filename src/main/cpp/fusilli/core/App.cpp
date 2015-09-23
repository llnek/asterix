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
#include "XConfig.h"
#include "App.h"

NS_ALIAS(den, CocosDenshion)
NS_BEGIN(fusilli)

static c::Size designSize = c::Size(480, 320);
static c::Size smallSize = c::Size(480, 320);
static c::Size mediumSize = c::Size(1024, 768);
static c::Size largeSize = c::Size(2048, 1536);


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

  auto director = c::Director::getInstance();
  auto glview = director->getOpenGLView();
  auto cfg = XConfig::GetInstance();
  auto sz = cfg->GetGameSize();

  if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    glview = c::GLViewImpl::createWithRect("z", c::Rect(0, 0, sz.width, sz.height));
#else
    glview = c::GLViewImpl::create("z");
#endif
    director->setOpenGLView(glview);
  }

  PreLaunch(sz);

  register_all_packages();

  // run
  director->runWithScene( cfg->StartWith());

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void App::PreLaunch(const c::Size& dz) {

  auto director = c::Director::getInstance();
  auto glview = director->getOpenGLView();

  // turn on display FPS
  director->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);

  // Set the design resolution
  glview->setDesignResolutionSize(dz.width, dz.height, ResolutionPolicy::NO_BORDER);
  c::Size fz = glview->getFrameSize();

  // if the frame's height is larger than the height of medium size.
  if (fz.height > mediumSize.height) {
    director->setContentScaleFactor(
        MIN(largeSize.height/dz.height, largeSize.width/dz.width));
  }
  // if the frame's height is larger than the height of small size.
  else if (fz.height > smallSize.height) {
    director->setContentScaleFactor(
        MIN(mediumSize.height/dz.height, mediumSize.width/dz.width));
  }
  // if the frame's height is smaller than the height of medium size.
  else {
    director->setContentScaleFactor(
        MIN(smallSize.height/dz.height, smallSize.width/dz.width));
  }

  InitAudio();
}


//////////////////////////////////////////////////////////////////////////////
//
void App::InitAudio() {

}


//////////////////////////////////////////////////////////////////////////////
// This function will be called when the app is inactive
// When comes a phone call,it's be invoked too
void App::applicationDidEnterBackground() {

  c::Director::getInstance()->stopAnimation();
  den::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
// this function will be called when the app is active again
void App::applicationWillEnterForeground() {

  c::Director::getInstance()->startAnimation();
  den::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

NS_END(fusilli)

