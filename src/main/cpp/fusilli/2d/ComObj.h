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

#include "deprecated/CCDictionary.h"
#include "2d/CCSprite.h"
#include "ash/Component.h"
NS_USING(ash)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ComObj : public Component {
private:

public:
  Sprite sprite;
  bool status;
  Vec2 lastPos;

  void Hurt(float damage);

  void Inflate(Dictionary* options);
  void Inflate(float x, float y);

  void Deflate();

  float Height();
  float Width();

  void SetPos(float x, float y);

  const Vec2 Pos();

  const Size CSize();

  int Pid();

  /**
   * @private
   */
  ComObj(Sprite*, float health, float score);
  ComObj(Sprite*);

  virtual ~ComObj();
}

NS_END(fusilli)
#endif

