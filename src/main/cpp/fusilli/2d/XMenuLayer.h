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

#if !defined(__XMENULAYER_H__)
#define __XMENULAYER_H__

#include "XLayer.h"
USING_NS_CC;
NS_FI_BEGIN


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XMenuLayer : public XLayer {

public:

  virtual ~XMenuLayer();
  XMenuLayer();

  virtual const string& RTTI();

  CREATE_FUNC(XMenuLayer)

private:

  CC_DISALLOW_COPY_AND_ASSIGN(XMenuLayer)
};





NS_FI_END
#endif

