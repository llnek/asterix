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

#include "fusilli.h"
#include <sstream>
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
s_vec<sstr> tokenize(const sstr &src, char delim) {
  s::stringstream ss(src);
  s_vec<sstr> out;
  sstr tkn;
  while (s::getline(ss, tkn, delim)) {
    if (tkn.length() > 0) {
      out.push_back(tkn);
    }
  }
  return out;
}

//////////////////////////////////////////////////////////////////////////
//
int modulo(int x, int m) {
  int r = x%m;
  return r<0 ? r+m : r;
}
/*
int mod(int x, int m) {
    return (x%m + m)%m;
}
*/


NS_END



