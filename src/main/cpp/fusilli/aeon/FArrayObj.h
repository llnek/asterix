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

#include "fusilli.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
// Dynamic array of primitive types and
// objects.
template<typename T>
class FS_DLL FArray {

  DECL_PTR(T,_data)
  DECL_IZ(_sz)

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
  int size() { return _sz; }

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
  mc_del_arr(_data);
  _data=src._data;
  _sz=src._sz;
  SNPTR(src._data)
  src._sz=0;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(FArray<T> &&src) {
  _data=src._data;
  _sz=src._sz;
  SNPTR(src._data)
  src._sz=0;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>& FArray<T>::operator=(const FArray<T> &src) {
  mc_del_arr(_data);
  _sz=src._sz;
  if (_sz > 0) {
    _data= new T[_sz];
    for (int i=0; i < _sz; ++i) {
      _data[i] = src._data[i];
    }
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(const FArray<T> &src) {
  SNPTR(_data)
  _sz=src._sz;
  if (_sz > 0) {
    _data= new T[_sz];
    for (int i=0; i < _sz; ++i) {
      _data[i] = src._data[i];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(int z) {
  _data = z > 0 ? new T[z] : nullptr;
  _sz=z;
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
  mc_del_arr(_data)
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FArray<T>::find(T v) {
  for (int i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return i; }
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::fill(T v) {
  for (int i = 0; i < _sz; ++i) {
    _data[i]=v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::some(T v) {
  for (int i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::notAny(T v) {
  for (int i = 0; i < _sz; ++i) {
    if (v == _data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::all(T v) {
  for (int i = 0; i < _sz; ++i) {
    if (v != _data[i]) { return false; }
  }
  return _sz > 0 ;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>* FArray<T>::clone() {
  auto rc= new FArray<T>(this->_sz);
  for (int i=0; i < this->_sz; ++i) {
    rc->_data[i] = this->_data[i];
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::set(int pos, T v) {
  assert(pos >= 0 && pos < _sz);
  _data[pos] = v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::getFirst() {
  assert(_sz > 0);
  return _data[0];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::getLast() {
  assert(_sz > 0);
  return _data[_sz-1];
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::setFirst(T v) {
  assert(_sz > 0);
  _data[0]= v;
}

//////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::setLast(T v) {
  assert(_sz > 0);
  _data[_sz-1]=v;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::get(int pos) {
  assert(pos >= 0 && pos < _sz);
  return _data[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::operator[](int pos) {
  assert(pos >= 0 && pos < _sz);
  return _data[pos];
}






//////////////////////////////////////////////////////////////////////////
//
typedef FArray<float> FArrFloat;
typedef FArray<int> FArrInt;
typedef FArray<bool> FArrBool;


NS_END


