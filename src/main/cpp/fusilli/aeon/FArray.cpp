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

#include "FArray.h"
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
static void initor() {
::srand ( time(nullptr) );
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>& FArray<T>::operator=(FArray<T>&& src) {
  mc_del_arr(data);
  data=src.data;
  size=src.size;
  src.data=nullptr;
  src.size=0;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(FArray<T>&& src) {
  data=src.data;
  size=src.size;
  src.data=nullptr;
  src.size=0;
  initor();
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>& FArray<T>::operator=(const FArray<T>& src) {
  mc_del_arr(data);
  data= new T[src.size];
  size=src.size;
  for (int i=0; i < src.size; ++i) {
    data[i] = src.data[i];
  }
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(const FArray<T>& src) {
  data= new T[src.size];
  size=src.size;
  for (int i=0; i < src.size; ++i) {
    data[i] = src.data[i];
  }
  initor();
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray(int sz) {
  data = sz > 0 ? new T[sz] : nullptr;
  size=sz;
  initor();
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
FArray<T>::FArray() {
  data = nullptr;
  size=0;
  initor();
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
int FArray<T>::Find(T v) {
  for (int i = 0; i < size; ++i) {
    if (v == data[i]) { return i; }
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::Fill(T v) {
  for (int i = 0; i < size; ++i) {
    data[i]=v;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::Some(T v) {
  for (int i = 0; i < size; ++i) {
    if (v == data[i]) { return true; }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::NotAny(T v) {
  for (int i = 0; i < size; ++i) {
    if (v == data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
bool FArray<T>::All(T v) {
  for (int i = 0; i < size; ++i) {
    if (v != data[i]) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
int FArray<T>::RandomIndex() {
  auto pos= rand() % size;
  if (pos >= size) {
    return (int) size/2;
  } else {
    return pos;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::Clone(const FArray<T>& other) {
  assert(size == other.size);
  for (int i=0; i < size; ++i) {
    data[i] = other.data[i];
  }
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
void FArray<T>::Set(int pos, T v) {
  assert(pos >= 0 && pos < size);
  data[pos] = v;
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::Get(int pos) {
  assert(pos >= 0 && pos < size);
  return data[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
template<typename T>
T FArray<T>::operator[](int pos) {
  assert(pos >= 0 && pos < size);
  return data[pos];
}







NS_END(fusii)



