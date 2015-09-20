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

#if !defined(__XGAMELAYER_H__)
#define __XGAMELAYER_H__

#include "deprecated/CCDictionary.h"
#include "ash/System.h"
#include "ash/Engine.h"
#include "XLayer.h"

NS_ALIAS(c, cocos2d)
NS_ALIAS(s, std)
NS_ALIAS(a, ash)
NS_BEGIN(fusilli)

enum class GMode { ONE, TWO, NET };

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XGameLayer : public XLayer {

protected:

  virtual void OnNewGame(const GMode);
  virtual void SetMode(const GMode);
  XGameLayer();

  s::map<int,bool> keyboard;
  a::Engine* engine;

  GMode mode;
  int level;

private:

  NO__COPYASSIGN(XGameLayer)

public:

  virtual const Box4 GetEnclosureBox();
  virtual int GetIID() { return 2; }
  virtual void update(float);

  virtual void SendMsg(const stdstr& topic, void* msg);
  virtual ComObj* GetPlayer();

  virtual XLayer* Realize();
  virtual ~XGameLayer();

  const s::map<int,bool>& Keys();
  bool KeyPoll(int key);
  c::Dictionary* GetLCfg();


  XLayer* GetBackgd(int tag= 1);
  XLayer* GetHUD(int tag=3);


  void NewGame(const GMode);

};





NS_END(fusilli)
#endif

