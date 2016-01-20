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

#if !defined(__FARRAYOBJ_H__)
#define __FARRAYOBJ_H__

#include "fusilli.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// Dynamic array of primitive types and
// objects.
template<typename T>
class FS_DLL FArray {

  DECL_PTR(T,data)
  DECL_IZ(sz)

public:

  FArray<T>& operator=(const FArray<T>&);
  FArray<T>& operator=(FArray<T>&&);

  FArray(const FArray<T>&);
  FArray(FArray<T>&&);

  FArray<T>* clone();

  void setFirst(T value);
  void setLast(T value);

  T getFirst();
  T getLast();

  void set(int pos, T value);
  int size() { return sz; }

  bool notAny(T v);
  bool some(T v);
  bool all(T v);
  int find(T v);
  void fill(T v);

  T operator[](int pos);
  T get(int pos);

  explicit FArray(int z);
  FArray();

  virtual ~FArray();
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>& FArray<T>::operator=(FArray<T> &&src) {
  mc_del_arr(data);
  data=src.data;
  sz=src.sz;
  SNPTR(src.data)
  src.sz=0;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(FArray<T> &&src) {
  data=src.data;
  sz=src.sz;
  SNPTR(src.data)
  src.sz=0;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>& FArray<T>::operator=(const FArray<T> &src) {
  mc_del_arr(data);
  sz=src.sz;
  if (sz > 0) {
    data= new T[sz];
    for (int i=0; i < sz; ++i) {
      data[i] = src.data[i];
    }
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(const FArray<T> &src) {
  SNPTR(data)
  sz=src.sz;
  if (sz > 0) {
    data= new T[sz];
    for (int i=0; i < sz; ++i) {
      data[i] = src.data[i];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(int z) {
  data = z > 0 ? new T[z] : nullptr;
  sz=z;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray() {
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::~FArray() {
  mc_del_arr(data)
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FArray<T>::find(T v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return i; }
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::fill(T v) {
  for (int i = 0; i < sz; ++i) {
    data[i]=v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::some(T v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::notAny(T v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::all(T v) {
  for (int i = 0; i < sz; ++i) {
    if (v != data[i]) { return false; }
  }
  return sz > 0 ;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>* FArray<T>::clone() {
  auto rc= new FArray<T>(this->sz);
  for (int i=0; i < this->sz; ++i) {
    rc->data[i] = this->data[i];
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::set(int pos, T v) {
  assert(pos >= 0 && pos < sz);
  data[pos] = v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::getFirst() {
  assert(sz > 0);
  return data[0];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::getLast() {
  assert(sz > 0);
  return data[sz-1];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::setFirst(T v) {
  assert(sz > 0);
  data[0]= v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::setLast(T v) {
  assert(sz > 0);
  data[sz-1]=v;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::get(int pos) {
  assert(pos >= 0 && pos < sz);
  return data[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::operator[](int pos) {
  assert(pos >= 0 && pos < sz);
  return data[pos];
}






//////////////////////////////////////////////////////////////////////////
//
typedef FArray<float> FArrFloat;
typedef FArray<int> FArrInt;
typedef FArray<bool> FArrBool;

NS_END(fusii)
#endif


