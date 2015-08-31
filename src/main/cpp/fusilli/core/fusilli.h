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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>


#if defined(WIN32) || defined(_WIN32)

#if defined(MS_STATIC)
#define MS_DLL
#else
#if defined(_USRDLL)
#define MS_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define MS_DLL    __declspec(dllimport)
#endif
#endif


#else
#define MS_DLL
#endif



#if !defined(TChar)

#  if defined(_UNICODE)
#   define  TChar  wchar_t
#  else
#    define  TChar  char
#  endif

#endif

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


#ifdef __cplusplus
# define USING_NS_FI    using namespace fusilli
# define NS_FI          ::fusilli
# define NS_FI_BEGIN    namespace fusilli {
# define NS_FI_END      }
#else
# define NS_FI_BEGIN
# define NS_FI_END
# define USING_NS_FI
# define NS_FI
#endif

#define mc_free_mem(mem)  { if (mem) ::free(mem); mem = nullptr; }
#define mc_free_fp(fp)    { if (fp) ::fclose(fp); fp = nullptr; }
#define mc_free_ptr(ptr)  mc_free_mem(ptr)
#define mc_free_str(pc)   mc_free_mem(pc)
#define mc_del_ptr(ptr)   { delete ptr; ptr = nullptr; }
#define mc_del_arr(arr)   { delete[] arr; arr = nullptr; }
#define mc_bool_str(b)    ((b) ? "true" : "false")
#define mc_bool_str_u(b)  ((b) ? "TRUE" : "FALSE")

#define mc_pcast(classname, expr) ((##classname *) (expr))
#define mc_rcast(classname, expr) ((##classname &) (expr))

#define DISALLOW_COPY_AND_ASSIGN(T) \
  T(const T&) = delete; \
  T&operator =(const T&) = delete;





#endif

