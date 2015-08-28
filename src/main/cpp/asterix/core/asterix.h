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

#ifndef __ASTERIX_H__
#define __ASTERIX_H__

#include <string>


#if !defined(ZL_DLLEXPORT)

# if defined(WIN32) || defined(_WIN32)
#   define ZL_DLLEXPORT  __declspec(dllexport)
# else
#   define ZL_DLLEXPORT
# endif

#endif

#if !defined(ZL_DLLIMPORT)

# if defined(WIN32) || defined(_WIN32)
#   define ZL_DLLIMPORT  __declspec(dllimport)
# else
#   define ZL_DLLIMPORT
# endif

#endif


#if !defined(TCHAR)

#	if defined(_UNICODE)
#   define	TCHAR	wchar_t
#	else
#		define	TCHAR	char
#	endif

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
# define USING_NS_AX    using namespace asterix
# define NS_AX          ::asterix
#	define NS_AX_BEGIN    namespace asterix {
#	define NS_AX_END      }
#else
# define NS_AX_BEGIN
# define NS_AX_END
# define USING_NS_AX
# define NS_AX
#endif








#endif

