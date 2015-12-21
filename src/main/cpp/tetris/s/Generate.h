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

#if !defined(__GENERATE_H__)
#define __GENERATE_H__

#include "core/XSystem.h"
#include "EFactory.h"
#include "n/GNodes.h"
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Generate : public f::XSystem<EFactory> {

  virtual const a::SystemType typeId() { return "s/Generate"; }

  Generate(not_null<EFactory*>, not_null<c::Dictionary*>);

  virtual void addToEngine(not_null<a::Engine*>);
  virtual bool update(float);
  virtual int priority() { return a::AI + 60; }

  Shape * reifyNextShape(not_null<a::Node*>, not_null<f::XLayer*>);
  void previewNextShape(not_null<a::Node*>, not_null<f::XLayer*>);
  Shape * randNextInfo();

  Shape *nextShapeInfo=nullptr;
  Shape *nextShape=nullptr;
  a::NodeList *arena = nullptr;

  virtual ~Generate() {}
  NODFT(Generate)
  NOCPYASS(Generate)
};


NS_END(tetris)
#endif


