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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {

  auto p= MGMS()->getPool("FallingObjects");
  auto &pl = p->list();
  auto ss= CC_GNLF(GVars,shared,"slots");

  if (!ss->shockWave->isVisible()) {
    return;
  }


        for (i = count-1; i >= 0; i--) {
            auto sprite =  _fallingObjects.at(i);
            diffx = _shockWave->getPositionX() - sprite->getPositionX();
            diffy = _shockWave->getPositionY() - sprite->getPositionY();
            if (pow(diffx, 2) + pow(diffy, 2) <= pow(_shockWave->getBoundingBox().size.width * 0.5f, 2)) {
                sprite->stopAllActions();
                sprite->runAction( _explosion->clone());
                SimpleAudioEngine::getInstance()->playEffect("boom.wav");
                if (sprite->getTag() == kSpriteMeteor) {
                    _shockwaveHits++;
                    _score += _shockwaveHits * 13 + _shockwaveHits * 2;
                }
                //play sound
                _fallingObjects.erase(i);
            }
        }
        if (_ufo->isVisible() && !_ufoKilled) {

            diffx = _shockWave->getPositionX() - _ufo->getPositionX();
            diffy = _shockWave->getPositionY() - _ufo->getPositionY();
            if (pow(diffx, 2) + pow(diffy, 2) <= pow(_shockWave->getBoundingBox().size.width * 0.6f, 2)) {
                _ufoKilled = true;
                SimpleAudioEngine::getInstance()->stopAllEffects();
                _ufo->stopAllActions();
                ray->stopAllActions();
                ray->setVisible(false);
                _ufo->runAction( _explosion->clone());
                SimpleAudioEngine::getInstance()->playEffect("boom.wav");

                 _shockwaveHits++;
                 _score += _shockwaveHits * 13 + _shockwaveHits * 4;
            }
        }

        _scoreDisplay->setString(String::createWithFormat("%i", _score)->getCString());


}


NS_END




