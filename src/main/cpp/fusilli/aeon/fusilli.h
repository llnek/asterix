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

#if !defined(__FUSILLI_H__)
#define __FUSILLI_H__

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

typedef std::string stdstr;


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
//
#ifdef __cplusplus
  #define NS_USING(nsp)  using namespace nsp;
  #define NS_BEGIN(nsp)  namespace nsp {
  #define NS_END(nsp)    };
  #define NS_ALIAS(x,y)  namespace x = y;
#else
  #define NS_USING(nsp)
  #define NS_BEGIN(nsp)
  #define NS_END(nsp)
  #define NS_ALIAS(x,y)
#endif


//////////////////////////////////////////////////////////////////////////////
//
#define mc_free_mem(mem)  { if (mem) ::free(mem); mem = nullptr; }
#define mc_free_fp(fp)    { if (fp) ::fclose(fp); fp = nullptr; }
#define mc_new(T) new T()
#define mc_free_ptr(ptr)  mc_free_mem(ptr)
#define mc_free_str(pc)   mc_free_mem(pc)

#define mc_del_arr(arr)   { delete[] arr; arr = nullptr; }
#define mc_del_ptr(ptr)   { delete ptr; ptr = nullptr; }
#define mc_bool_str(b)    ((b) ? "true" : "false")
#define mc_bool_str_u(b)  ((b) ? "TRUE" : "FALSE")

//////////////////////////////////////////////////////////////////////////////
//
#define DCAST(type,expr) dynamic_cast<type>(expr)
#define SCAST(type,expr) static_cast<type>(expr)
#define NNP(p) p != nullptr
#define ENP(p) p == nullptr

//////////////////////////////////////////////////////////////////////////////
//
#define NO__CPYASS(T) \
  T&operator =(const T&) = delete;  \
  T(const T&) = delete; \
  T(T&&) = delete;  \
  T&operator =(T&&) = delete;

//////////////////////////////////////////////////////////////////////////
//
#define IMPL_CTOR(T) \
  virtual ~T() {} \
  T() {}

//////////////////////////////////////////////////////////////////////////
//
#define DECL_CTOR(T) \
  virtual ~T(); \
  T();

//////////////////////////////////////////////////////////////////////////////
//
#define DEFCREATE_FUNC(T) static owner<T*> Reify() { return new(std::nothrow) T(); }
#define SNPTR(x) x = nullptr;

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

NS_END(fusii)
#include "FArray.h"
#include "FPtr.h"
NS_ALIAS(f,fusii)
NS_USING(gsl)
#endif


