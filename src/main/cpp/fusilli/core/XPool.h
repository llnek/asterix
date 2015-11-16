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

  s::vector<ComObj*> objs;
  NO__CPYASS(XPool)

public:

  const s::vector<ComObj*>& Elements() { return objs; }
  void Preset(s::function<ComObj* ()>, int);
  ComObj* Select(s::function<bool (ComObj*)>);

  ComObj* GetAndSet();
  ComObj* Get();
  ComObj* GetAt(int n);

  int Size() { return (int)objs.size(); }
  int CountActives();

  void Foreach(s::function<void (ComObj*)>);
  void ClearAll(bool del=true);

  void Checkin(not_null<ComObj*>);
  void Reset();

  DECL_CTOR(XPool)
};





NS_END(fusii)
#endif

