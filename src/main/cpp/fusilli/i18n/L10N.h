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

#if !defined(__L10N_H__)
#define __L10N_H__

#include "fusilli.h"
#include "cocos2d.h"

NS_FI_BEGIN

class L10NCache {

public:

  const std::string& getStr(const std::string& key, const std::string& dft);
  void purge();
  void init();

  virtual ~L10NCache();
  L10NCache();

private:
  std::map<std::string, std::string> _cache;
  DISALLOW_COPY_AND_ASSIGN(L10NCache)
};

NS_FI_END

#endif

