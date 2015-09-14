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
#include "support/XConfig.h"
#include "boot/Boot.h"
#include "App.h"
NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cc, cocos2d)
NS_BEGIN(fusilli)


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
  cc::GLView::setGLContextAttrs(glContextAttrs);
}

//////////////////////////////////////////////////////////////////////////////
//
bool App::applicationDidFinishLaunching() {

  auto director = cc::Director::getInstance();
  auto glview = director->getOpenGLView();

  if (!glview) {
    glview = cc::GLViewImpl::create("My Game");
    director->setOpenGLView(glview);
  }

  auto cfg = XConfig::GetInstance();
  Boot b;

  register_all_packages();

  // run
  director->runWithScene( cfg->StartWith());

  return true;
}

//////////////////////////////////////////////////////////////////////////////
// This function will be called when the app is inactive
// When comes a phone call,it's be invoked too
void App::applicationDidEnterBackground() {

  cc::Director::getInstance()->stopAnimation();
  den::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

//////////////////////////////////////////////////////////////////////////////
// this function will be called when the app is active again
void App::applicationWillEnterForeground() {

  cc::Director::getInstance()->startAnimation();
  den::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

NS_END(fusilli)

