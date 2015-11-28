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

#if !defined(__CONFIG_H__)
#define __CONFIG_H__

#include "core/XConfig.h"

//////////////////////////////////////////////////////////////////////////
//
enum class Moves { RUSH, VERT, HORZ, OLAP };

enum class Attacks { TSUIHIKIDAN, NORMAL };

struct EnemyType {

  EnemyType(Attacks attackMode, Moves moveType,
    int type,
    stdtsr textureName,
    stdstr bulletType,
    int HP, int scoreValue) {

  this->attackMode = attackMode;
  this->moveType= moveType;
  this->ytpe= type;
  this->textureName = textureName;
  this->bulletType = bulletType;
  this->HP= HP;
  this->scoreValue = scoreValue;
  }

  Attacks attackMode;
  Moves moveType;
  int type;
  stdtsr textureName;
  stdstr bulletType;
  int HP;
  int scoreValue;
};

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Config : public f::XConfig {
protected:

  NO__CPYASS(Config)
  Config();

public:

  virtual void handleResolution(const c::Size& );

  virtual void runOnce();

  virtual ~Config();

  virtual const stdstr appKey() { return "4d6b93c4-05d7-42f1-95cc-98ce8adeac0a"; }

  virtual const stdstr color() { return "yellow"; }

  virtual const stdstr appId() { return "terra"; }

  virtual const c::Size gameSize() { return c::Size(320,480); }


  static Config* reify();
};


NS_END(terra)
#endif



