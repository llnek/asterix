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

#include "platform/CCStdC.h"
#include "Boot.h"
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
static cc::Size designResolutionSize = cc::Size(480, 320);
static cc::Size smallResolutionSize = cc::Size(480, 320);
static cc::Size mediumResolutionSize = cc::Size(1024, 768);
static cc::Size largeResolutionSize = cc::Size(2048, 1536);

//////////////////////////////////////////////////////////////////////////////
//
void Boot::PreLaunch() {

  auto director = cc::Director::getInstance();
  auto glview = director->getOpenGLView();

  // turn on display FPS
  director->setDisplayStats(true);

  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);

  // set the design resolution
  glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
  cc::Size frameSize = glview->getFrameSize();

  if (frameSize.height > mediumResolutionSize.height) {
    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
  }
  else if (frameSize.height > smallResolutionSize.height) {
    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
  } else {
    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
  }

  InitAudio();
}

//////////////////////////////////////////////////////////////////////////////
//
void Boot::InitAudio() {
}

//////////////////////////////////////////////////////////////////////////////
//
Boot::~Boot() {
}

//////////////////////////////////////////////////////////////////////////////
//
Boot::Boot() {
  PreLaunch();
  l10n.Init();
}


NS_END(fusilli)

