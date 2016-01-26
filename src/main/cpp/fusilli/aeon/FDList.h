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

#include "GSL/gsl.h"
NS_USING(gsl)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
struct FS_DLL FDListItem {
  DECL_PTR(T , previous)
  DECL_PTR(T , next)
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
struct FS_DLL FDListAnchor {
  DECL_PTR(T , head)
  DECL_PTR(T , tail)
};

//////////////////////////////////////////////////////////////////////////////
// owns all items in this list
template <typename T>
struct FS_DLL FDList {

  virtual bool isEmpty() { return head==nullptr; }
  virtual void release(not_null<T*>);
  virtual void purge(not_null<T*>);
  virtual void add(not_null<T*> );

  const s_vec<T*> list();
  void clear() ;
  int size();

  DECL_PTR(T , head)
  DECL_PTR(T , tail)

  virtual ~FDList();
  FDList() {}
  NOCPYASS(FDList)
};

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::add(not_null<T*> e) {
  if (ENP(head)) {
    assert(tail == nullptr);
    SNPTR(e->previous)
    SNPTR(e->next)
    head = tail = e;
  } else {
    e->previous = tail;
    SNPTR(e->next)
    tail->next = e;
    tail = e;
  }
}
//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FDList<T>::size() {
  auto n= head;
  int c=0;
  while (NNP(n)) {
    n = n->next;
    ++c;
  }
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::release(not_null<T*> e) {
  if (tail == e) { tail = tail->previous; }
  if (head == e) { head = head->next; }
  if (NNP(e->previous)) {
    e->previous->next = e->next;
  }
  if (NNP(e->next)) {
    e->next->previous = e->previous;
  }
  SNPTR(e->previous)
  SNPTR(e->next)
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::purge(not_null<T*> e) {
  release(e);
  delete e.get();
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
void FDList<T>::clear() {
  while (NNP(head)) {
    auto e= head;
    head = head->next;
    delete e;
  }
  SNPTR(head)
  SNPTR(tail)
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
const s_vec<T*> FDList<T>::list() {
  s_vec<T*> v;
  for (auto p= head; NNP(p); p=p->next) {
    v.push_back(p);
  }
  return v;
}

//////////////////////////////////////////////////////////////////////////////
//
template <typename T>
FDList<T>::~FDList() {
  //printf("FDList dtor\n");
  clear();
}


NS_END


