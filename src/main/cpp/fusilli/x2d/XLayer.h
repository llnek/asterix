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

#if !defined(__XLAYER_H__)
#define __XLAYER_H__


#include "platform/CCCommon.h"
#include "aeon/fusilli.h"
#include "2d/CCLayer.h"
#include <stdint.h>
NS_ALIAS(c, cocos2d)
NS_ALIAS(a, ash)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)


class XScene;
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public c::Layer {
friend class XScene;
protected:

  void AudioCallback(c::Ref* sender);
  virtual void OnQuit(c::Ref*);
  XLayer();

  s::map<stdstr, c::SpriteBatchNode*> atlases;
  int lastTag;
  int lastZix;

private:
  NO__COPYASSIGN(XLayer)

public:

  c::SpriteBatchNode* RegoAtlas(const stdstr& name, int* z = nullptr, int* tag= nullptr);
  virtual XLayer* Realize();

  virtual c::SpriteBatchNode* GetAtlas(const stdstr& name);

  virtual void AddAtlasItem(const stdstr& atlas,
      c::Node* n, int* zx = nullptr, int* tag = nullptr);
  virtual void AddItem(c::Node* n,
      int* zx = nullptr, int* tag = nullptr);

  void AddAtlasFrame
    (const stdstr& atlas,
     const stdstr& frame, const c::Vec2& pos);

  void AddFrame
    (const stdstr& frame, const c::Vec2& pos);

  void CenterAtlasImage(const stdstr& atlas, const stdstr& frame);
  void CenterImage(const stdstr& frame);

  void RemoveAtlasAll(const stdstr& atlas) ;
  void RemoveAll();
  void RemoveItem(c::Node* n);

  void AddAudioIcons(c::MenuItem* off, c::MenuItem* on,
      const c::Vec2& anchor,
      const c::Vec2& pos);

  XScene* GetScene();
  int IncIndexZ();

  // tag value
  virtual int GetIID() = 0;
  virtual ~XLayer();

};



NS_END(fusilli)
#endif

