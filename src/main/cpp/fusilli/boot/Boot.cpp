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

#include "Boot.h"
#include "L10N.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////
//
NS_AX_BEGIN

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);


//////////////////////////////////////////////////////////////////////////////
//
void Boot::preLaunch() {

  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();

  // turn on display FPS
  director->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);

  // set the design resolution
  glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
  Size frameSize = glview->getFrameSize();

  if (frameSize.height > mediumResolutionSize.height) {
    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
  }
  else if (frameSize.height > smallResolutionSize.height) {
    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
  } else {
    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
  }

  initAudio();
}

void Boot::initAudio() {

}

//////////////////////////////////////////////////////////////////////////////
//
void Boot::init() {

  preLaunch();
  l10n.init();
}

Boot::~Boot() {

}

Boot::Boot() {

}


NS_AX_END
//////////////////////////////////////////////////////////////////////////////
//EOF
