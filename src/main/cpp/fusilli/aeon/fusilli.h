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
#include <functional>
#include <queue>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//typedef std::string stdstr;
typedef std::string sstr;

//////////////////////////////////////////////////////////////////////////////
//
#if defined(WIN32) || defined(_WIN32)

#if defined(FS_STATIC)
#define FS_DLL
#else
#if defined(_USRDLL)
#define FS_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define FS_DLL    __declspec(dllimport)
#endif
#endif


#else
#define FS_DLL
#endif


//////////////////////////////////////////////////////////////////////////////
//
#if !defined(TChar)

#  if defined(_UNICODE)
#   define  TChar  wchar_t
#  else
#    define  TChar  char
#  endif

#endif


//////////////////////////////////////////////////////////////////////////////
// namespace stuff
#ifdef __cplusplus
#define NS_USING(nsp)  using namespace nsp;
#define NS_BEGIN(nsp)  namespace nsp {
#define NS_END         };
#define NS_ALIAS(x,y)  namespace x = y;
#define BEGIN_NS_UNAMED   namespace {
#define END_NS_UNAMED   };
#else
#define NS_USING(nsp)
#define NS_BEGIN(nsp)
#define NS_END
#define NS_ALIAS(x,y)
#define BEGIN_NS_UNAMED
#define END_NS_UNAMED
#endif


//////////////////////////////////////////////////////////////////////////////
// memory lifecycle stuff
#define mc_free_mem(mem)  { if (mem) ::free(mem); mem = nullptr; }
#define mc_free_fp(fp)    { if (fp) ::fclose(fp); fp = nullptr; }
#define mc_new3(T, p1, p2, p3) new(std::nothrow) T(p1, p2, p3)
#define mc_new2(T, p1, p2) new(std::nothrow) T(p1, p2)
#define mc_new1(T, p1) new(std::nothrow) T(p1)
#define mc_new(T) new(std::nothrow) T()
#define mc_free_ptr(ptr)  mc_free_mem(ptr)
#define mc_free_str(pc)   mc_free_mem(pc)

#define mc_del_arr(arr)   { delete[] arr; arr = nullptr; }
#define mc_del_ptr(ptr)   { delete ptr; ptr = nullptr; }
#define mc_bool_str(b)    ((b) ? "true" : "false")
#define mc_bool_str_u(b)  ((b) ? "TRUE" : "FALSE")

//////////////////////////////////////////////////////////////////////////////
// c++ std collection iteration
#define F__LOOP(x,c) for (auto x=c.begin(); x != c.end(); ++x)
#define S__PAIR(T1,T2,v1,v2) std::pair<T1,T2>(v1,v2)
#define S__COPY(f,t) s::copy(s::begin(f), s::end(f), s::begin(t))


//////////////////////////////////////////////////////////////////////////////
// c++ casting
#define DCAST(type,expr) dynamic_cast<type>(expr)
#define SCAST(type,expr) static_cast<type>(expr)

//////////////////////////////////////////////////////////////////////////////
// pointer macros
#define SNPTR(x) x = nullptr;
#define NNP(p) p != nullptr
#define ENP(p) p == nullptr

//////////////////////////////////////////////////////////////////////////////
// std collection aliases
#define s_vec std::vector
#define s_que std::queue
#define s_arr std::array
#define s_map std::map

//////////////////////////////////////////////////////////////////////////////
// c++ constructor stuff
#define NOCPYASS(T) \
  T&operator =(const T&) = delete;  \
  T(const T&) = delete; \
  T(T&&) = delete;  \
  T&operator =(T&&) = delete;

#define NODFT(T) T()=delete;

//////////////////////////////////////////////////////////////////////////
// member decl stuff
#define DECL_PTR(T,p) T *p=nullptr;
#define DECL_DZ(d) double d=0;
#define DECL_FZ(f) float f=0;
#define DECL_IZ(i) int i=0;
#define DECL_BF(b) bool b=false;
#define DECL_BT(b) bool b=true;
#define DECL_TD(T,m) T m;
#define DECL_TV(T,m,v) T m=v;

//////////////////////////////////////////////////////////////////////////////
//
NS_BEGIN(fusii)

enum class Locale {
  ENGLISH = 0,
  CHINESE,
  FRENCH,
  ITALIAN,
  GERMAN,
  SPANISH,
  DUTCH,
  RUSSIAN,
  KOREAN,
  JAPANESE,
  HUNGARIAN,
  PORTUGUESE,
  ARABIC,
  NORWEGIAN,
  POLISH,
  TURKISH,
  UKRAINIAN,
  ROMANIAN,
  BULGARIAN
};

//////////////////////////////////////////////////////////////////////////////
//
s_vec<sstr> tokenize(const sstr &src, char delim);
int modulo(int, int);
void randSeed();








NS_END

#include "FArrayObj.h"
#include "FArrayPtr.h"
#include "SMPtr.h"
#include "FDList.h"
NS_ALIAS(f,fusii)
NS_ALIAS(s,std)
NS_USING(gsl)


