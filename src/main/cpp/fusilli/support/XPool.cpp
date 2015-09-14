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

#include "2d/ComObj.h"
#include "XPool.h"
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
// Pre-populate a bunch of objects in the pool
void XPool::PreSet(s::function<ComObj* (XPool*)> f, int count) {
  for (int n=0; n < count; ++n) {
    auto rc= f(this);
    if (NNP(rc)) {
      objs.push_back(rc);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// Find an object by applying this filter
ComObj* XPool::Select(s::function<bool (ComObj*)> f) {
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    auto e = *it;
    if (f(e)) {
      return e;
    }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Get an object from the pool and set it's status to true
ComObj* XPool::GetAndSet() {
  auto rc= Get();
  if (NNP(rc)) {
    rc->status=true;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
// Get an object from the pool.  More like a peek
ComObj* XPool::Get() {
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    auto e= *it;
    if (! e->status) { return e; }
  }
  return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Get the count of active objects
int XPool::CountActives() {
  auto c=0;
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    if ((*it)->status) {
      ++c;
    }
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////
//
void XPool::Foreach(s::function<void (ComObj*)> f) {
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    f(*it);
  }
}

//////////////////////////////////////////////////////////////////////////
// Hibernate (status off) all objects in the pool
void XPool::Reset() {
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    (*it)->Deflate();
  }
}

//////////////////////////////////////////////////////////////////////////
//
XPool::~XPool() {
  for (auto it = objs.begin(); it != objs.end(); ++it) {
    delete *it;
  }
  objs.clear();
}

//////////////////////////////////////////////////////////////////////////
//
XPool::XPool() {
}



NS_END(fusilli)
