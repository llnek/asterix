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

NS_ALIAS(cc, cocos2d)
NS_ALIAS(a, ash)
NS_ALIAS(s, std)
NS_BEGIN(fusilli)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public cc::Layer {
friend class XScene;
protected:

  void CreateAudioIcons(cc::MenuItem*& off, cc::MenuItem*& on);
  void AudioCallback(cc::Ref* sender);
  void AddAudioIcons(cc::MenuItem* off, cc::MenuItem* on,
      const cc::Vec2& anchor, const cc::Vec2& pos);
  virtual void OnQuit(cc::Ref*);
  XLayer();

private:

  s::map<s::string, cc::SpriteBatchNode*> atlases;
  int lastTag;
  int lastZix;

  DISALLOW_COPYASSIGN(XLayer)

public:

  // tag value
  virtual int GetIID() = 0;

  cc::MenuItem* CreateMenuBtn( const s::string& n);

  cc::MenuItem*
    CreateMenuBtn(
      const s::string& n,
      const s::string& s, const s::string& d);

  cc::SpriteBatchNode*
    RegoAtlas(const s::string& name,
        int* z = nullptr, int* tag= nullptr);

  virtual XLayer* Realize();

  virtual void PKInput();

  virtual void AddAudioIcon(cc::Dictionary* options);

  virtual void CenterAtlasImage
    (const s::string& atlas, const s::string& frame);

  virtual void CenterImage(const s::string& frame);

  virtual void AddAtlasFrame
    (const s::string& atlas,
     const s::string& frame, const cc::Vec2& pos);

  virtual void AddFrame
    (const s::string& frame, const cc::Vec2& pos);

  virtual cc::SpriteBatchNode*
    GetAtlas(const s::string& name);

  virtual void RemoveAtlasAll(const s::string& atlas) ;
  virtual void RemoveAll();

  virtual void RemoveItem(cc::Node* n);

  virtual void AddAtlasItem(const s::string& atlas,
      cc::Node* n, int* zx = nullptr, int* tag = nullptr);
  virtual void AddItem(cc::Node* n,
      int* zx = nullptr, int* tag = nullptr);

  XScene* GetScene();
  int IncIndexZ();

  virtual ~XLayer();

};





NS_END(fusilli)
#endif

