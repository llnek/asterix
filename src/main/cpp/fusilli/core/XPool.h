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

#pragma once

//////////////////////////////////////////////////////////////////////////////
//

#include "Primitives.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XPool {
  s::function<Poolable* ()> ctor;
  s_vec<Poolable*> objs;
  DECL_IZ(batch)
public:

  const s_vec<Poolable*>& list() { return objs; }
  Poolable* select(s::function<bool (Poolable*)>);
  void preset(s::function<Poolable* ()>, int);

  Poolable* getAndSet(bool create=false);
  Poolable* get(bool create=false);
  Poolable* getAt(int n);

  int size() { return (int)objs.size(); }
  int countActives();

  void foreach(s::function<void (Poolable*)>);
  bool some(s::function<bool (Poolable*)>);
  void clearAll(bool del=true);

  void checkin(not_null<Poolable*>);
  void reset();

  virtual ~XPool() {  clearAll(true); }
  XPool() {}
  NOCPYASS(XPool)
};


NS_END




