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

#include "L10N.h"
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
void L10NCache::Purge() {
  _cache.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
void L10NCache::Init() {
  Purge();
}

//////////////////////////////////////////////////////////////////////////////
//
//Application::getInstance()->getCurrentLanguage();
const s::string L10NCache::GetStr(const s::string& key, const s::string& dft) {
  auto it= _cache.find(key);
  s::string rc;
  if (it != _cache.end()) {
    rc= it->second;
  } else {
    rc= dft;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
L10NCache::~L10NCache() {

}

//////////////////////////////////////////////////////////////////////////////
//
L10NCache::L10NCache() {

}





NS_END(fusilli)



