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

#if !defined(__FARRAY_H__)
#define __FARRAY_H__

#include "aeon/fusilli.h"
NS_BEGIN(fusii)

template<typename T>
class FS_DLL FArray {
protected:
  size_t size;
  T* data;
public:

  void Clone(const FArray<T>& other);
  void Set(size_t pos, T value);
  int Size() { return size; }
  void Fill(T v);
  bool NotAny(T v);
  int RandomIndex();
  bool Some(T v);
  bool All(T v);
  int Find(T v);
  T operator[](size_t pos);
  T Get(size_t pos);

  FArray& operator=(FArray<T>&&);
  FArray(FArray<T>&&);

  FArray& operator=(const FArray<T>&);
  FArray(const FArray<T>&);

  FArray(size_t size = 0);
  virtual ~FArray();
};







NS_END(fusii)
#endif


