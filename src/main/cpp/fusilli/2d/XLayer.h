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
#include "core/fusilli.h"
#include "2d/CCLayer.h"
#include <stdint.h>


NS_ALIAS(c, cocos2d)
NS_ALIAS(a, ash)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)



//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public c::Layer {
friend class XScene;
protected:

  void CreateAudioIcons(c::MenuItem*& off, c::MenuItem*& on);
  void AudioCallback(c::Ref* sender);

  void AddAudioIcons(c::MenuItem* off, c::MenuItem* on,
      const c::Vec2& anchor, const c::Vec2& pos);

  virtual void OnQuit(c::Ref*);
  XLayer();

private:

  s::map<stdstr, c::SpriteBatchNode*> atlases;
  int lastTag;
  int lastZix;

  NO__COPYASSIGN(XLayer)

public:

  c::MenuItem* CreateMenuBtn( const stdstr& n);

  c::MenuItem*
    CreateMenuBtn(
      const stdstr& n,
      const stdstr& s, const stdstr& d);

  c::SpriteBatchNode*
    RegoAtlas(const stdstr& name,
        int* z = nullptr, int* tag= nullptr);

  virtual XLayer* Realize();

  virtual void PKInput();

  virtual void AddAudioIcon(c::Dictionary* options);

  virtual void CenterAtlasImage
    (const stdstr& atlas, const stdstr& frame);

  virtual void CenterImage(const stdstr& frame);

  virtual void AddAtlasFrame
    (const stdstr& atlas,
     const stdstr& frame, const c::Vec2& pos);

  virtual void AddFrame
    (const stdstr& frame, const c::Vec2& pos);

  virtual c::SpriteBatchNode*
    GetAtlas(const stdstr& name);

  virtual void RemoveAtlasAll(const stdstr& atlas) ;
  virtual void RemoveAll();

  virtual void RemoveItem(c::Node* n);

  virtual void AddAtlasItem(const stdstr& atlas,
      c::Node* n, int* zx = nullptr, int* tag = nullptr);
  virtual void AddItem(c::Node* n,
      int* zx = nullptr, int* tag = nullptr);

  XScene* GetScene();
  int IncIndexZ();

  // tag value
  virtual int GetIID() = 0;

  virtual ~XLayer();

};





NS_END(fusilli)
#endif


