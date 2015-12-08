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

#include "core/BaseSystem.h"
#include "EFactory.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Render : public f::BaseSystem<EFactory> {
protected:

  virtual bool onUpdate(float);
  void processMovement(float);

public:

  virtual const a::SystemType typeId() { return "n/Render"; }

  Render(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*> );

  virtual int priority() { return a::Render; }

  virtual ~Render() {}

  NOCPYASS(Render)
  NODFT(Render)

};


NS_END(terra)


