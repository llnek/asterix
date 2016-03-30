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

#include "fusilli.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// Dynamic array of pointers to a type.
template<typename T>
class FS_DLL FArrayPtr {

  __decl_ptr(T*,_data)
  __decl_iz(_sz)

public:

  FArrayPtr<T>& operator=(const FArrayPtr<T>&);
  FArrayPtr<T>& operator=(FArrayPtr<T>&&);

  FArrayPtr(const FArrayPtr<T>&);
  FArrayPtr(FArrayPtr<T>&&);

  FArrayPtr<T>* clone();

  T* getFirst();
  T* getLast();

  void setFirst(T *value);
  void setLast(T *value);

  void set(int pos, T *value);
  int size() { return _sz; }
  T* swap(int, T*);

  void map(std::function<T* (T*)>);
  bool notAny(T *v);
  bool some(T *v);
  bool all(T *v);
  int find(T *v);
  void fill(T *v);

  T* operator[](int pos);
  T* get(int pos);

  explicit FArrayPtr(int z);
  FArrayPtr();

  virtual ~FArrayPtr();
};

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>& FArrayPtr<T>::operator=(FArrayPtr<T> &&src) {
  mc_del_arr(_data);
  _data=src._data;
  _sz=src._sz;
  S__NIL(src._data)
  src._sz=0;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>::FArrayPtr(FArrayPtr<T> &&src) {
  _data=src._data;
  _sz=src._sz;
  S__NIL(src._data)
  src._sz=0;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>& FArrayPtr<T>::operator=(const FArrayPtr<T> &src) {
  mc_del_arr(_data);
  _sz=src._sz;
  if (_sz > 0) {
    _data= new T* [_sz];
    for (auto i=0; i < _sz; ++i) {
      _data[i] = src._data[i];
    }
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>::FArrayPtr(const FArrayPtr<T> &src) {
  _sz=src._sz;
  S__NIL(_data)
  if (_sz > 0) {
    _data= new T* [_sz];
    for (auto i=0; i < _sz; ++i) {
      _data[i] = src._data[i];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>::FArrayPtr(int z) {
  _data = z > 0 ? new T* [z] : nullptr;
  _sz=z;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>::FArrayPtr() {
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>::~FArrayPtr() {
  mc_del_arr(_data)
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FArrayPtr<T>::swap(int pos, T *np) {
  assert(_sz > 0);
  assert(pos >=0 && pos < _sz);
  auto rc= _data[pos];
  _data[pos]= np;
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArrayPtr<T>::map(std::function<T* (T*)> m) {
  for (auto i = 0; i < _sz; ++i) {
    _data[i] = m(_data[i]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FArrayPtr<T>::find(T *v) {
  for (auto i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return i; }
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArrayPtr<T>::fill(T *v) {
  for (auto i = 0; i < _sz; ++i) {
    _data[i]=v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArrayPtr<T>::some(T *v) {
  for (auto i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArrayPtr<T>::notAny(T *v) {
  for (auto i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArrayPtr<T>::all(T *v) {
  for (auto i = 0; i < _sz; ++i) {
    if (v != _data[i]) { return false; }
  }
  return _sz > 0 ;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArrayPtr<T>* FArrayPtr<T>::clone() {
  auto rc= new FArrayPtr<T>(this->_sz);
  for (auto i=0; i < this->_sz; ++i) {
    rc->_data[i] = this->_data[i];
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArrayPtr<T>::set(int pos, T *v) {
  assert(pos >= 0 && pos < _sz);
  _data[pos] = v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FArrayPtr<T>::getFirst() {
  assert(_sz > 0);
  return _data[0];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FArrayPtr<T>::getLast() {
  assert(_sz > 0);
  return _data[_sz-1];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArrayPtr<T>::setFirst(T *v) {
  assert(_sz > 0);
  _data[0]= v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArrayPtr<T>::setLast(T *v) {
  assert(_sz > 0);
  _data[_sz-1]=v;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FArrayPtr<T>::get(int pos) {
  assert(pos >= 0 && pos < _sz);
  return _data[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T* FArrayPtr<T>::operator[](int pos) {
  assert(pos >= 0 && pos < _sz);
  return _data[pos];
}



NS_END


