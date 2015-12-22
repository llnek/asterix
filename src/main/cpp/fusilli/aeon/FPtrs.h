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

#if !defined(__FPTRS_H__)
#define __FPTRS_H__

#include "fusilli.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
template<typename T> class FS_DLL FPtrs {
private:
  T **data = nullptr;
  int sz = 0;
public:

  FPtrs<T>& operator=(const FPtrs<T>&);
  FPtrs<T>& operator=(FPtrs<T>&&);

  FPtrs(const FPtrs<T>&);
  FPtrs(FPtrs<T>&&);

  FPtrs<T>* clone();

  void setFirst(T *value);
  void setLast(T *value);

  void set(int pos, T *value);
  int size() { return sz; }

  int randomIndex();
  void fill(T *v);

  bool notAny(T *v);
  bool some(T *v);
  bool all(T *v);
  int find(T *v);

  T* operator[](int pos);
  T* get(int pos);

  explicit FPtrs(int z);
  FPtrs();

  virtual ~FPtrs();
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>& FPtrs<T>::operator=(FPtrs<T> &&src) {
  mc_del_arr(data);
  data=src.data;
  sz=src.sz;
  src.data=nullptr;
  src.sz=0;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>::FPtrs(FPtrs<T> &&src) {
  data=src.data;
  sz=src.sz;
  src.data=nullptr;
  src.sz=0;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>& FPtrs<T>::operator=(const FPtrs<T> &src) {
  mc_del_arr(data);
  sz=src.sz;
  if (sz > 0) {
    data= new T* [sz];
    for (int i=0; i < sz; ++i) {
      data[i] = src.data[i];
    }
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>::FPtrs(const FPtrs<T> &src) {
  data= nullptr;
  sz=src.sz;
  if (sz > 0) {
    data= new T* [sz];
    for (int i=0; i < sz; ++i) {
      data[i] = src.data[i];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>::FPtrs(int z) {
  data = z > 0 ? new T* [z] : nullptr;
  sz=z;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>::FPtrs() {
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>::~FPtrs() {
  mc_del_arr(data)
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FPtrs<T>::find(T *v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return i; }
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FPtrs<T>::fill(T *v) {
  for (int i = 0; i < sz; ++i) {
    data[i]=v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FPtrs<T>::some(T *v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FPtrs<T>::notAny(T *v) {
  for (int i = 0; i < sz; ++i) {
    if (v == data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FPtrs<T>::all(T *v) {
  for (int i = 0; i < sz; ++i) {
    if (v != data[i]) { return false; }
  }
  return sz > 0 ;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FPtrs<T>::randomIndex() {
  if (sz == 0) { return -1;  }
  if (sz == 1) { return 0; }
  return rand() % sz;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FPtrs<T>* FPtrs<T>::clone() {
  auto rc= new FPtrs<T>(this->sz);
  for (int i=0; i < this->sz; ++i) {
    rc->data[i] = this->data[i];
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FPtrs<T>::set(int pos, T *v) {
  assert(pos >= 0 && pos < sz);
  data[pos] = v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FPtrs<T>::setFirst(T *v) {
  assert(sz > 0);
  data[0]= v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FPtrs<T>::setLast(T *v) {
  assert(sz > 0);
  data[sz-1]=v;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FPtrs<T>::get(int pos) {
  assert(pos >= 0 && pos < sz);
  return data[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FPtrs<T>::operator[](int pos) {
  assert(pos >= 0 && pos < sz);
  return data[pos];
}



NS_END(fusii)
#endif


