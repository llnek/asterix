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

#if !defined(__XPOOL_H__)
#define __XPOOL_H__

#include "base/CCRef.h"
#include "Primitives.h"
#include "ComObj.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XPool {
private:

  s_vec<ComObj*> objs;

public:

  const s_vec<ComObj*>& elements() { return objs; }
  ComObj* select(s::function<bool (ComObj*)>);
  void preset(s::function<ComObj* ()>, int);

  ComObj* getAndSet();
  ComObj* get();
  ComObj* getAt(int n);

  int size() { return (int)objs.size(); }
  int countActives();

  void foreach(s::function<void (ComObj*)>);
  void clearAll(bool del=true);

  void checkin(not_null<ComObj*>);
  void reset();

  NOCPYASS(XPool)
  DECLCZ(XPool)
};





NS_END(fusii)
#endif

