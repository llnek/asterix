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

#ifndef  __XLIVE_H__
#define  __XLIVE_H__

#include "cocos2d.h"
#include "asterix.h"
NS_AX_BEGIN

class ZL_DLLEXPORT XLive : public cocos2d::Sprite {

private:
  CC_DISALLOW_COPY_AND_ASSIGN(XLive)

public:

  void initEx(const std::string& frame);

  virtual ~XLive();
  XLive();


  CREATE_FUNC(XLive)
};

NS_AX_END
#endif

