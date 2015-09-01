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

#include "../core/fusilli.h"
#include "2d/CCLayer.h"
USING_NS_CC;
NS_FI_BEGIN


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL XLayer : public cocos2d::Layer {

public:

  const std::string& rtti();

  SpriteBatchNode* regoAtlas(const std::string& name, int z, int tag);

  void setup();

  void pkInput();

  void addAudioIcon(Dictionary* options);

  void onQuit();

  void centerAtlasImage(const std::string& frame,
      const std::string& atlas);

  void centerImage(const std::string& frame);

  void addAtlasFrame(const std::string& frame,
      const Vec2& pos, const std::string& atlas);

  void addFrame(const std::string& frame, const Vec2& pos);

  SpriteBatchNode* getAtlas(const std::string& name);

  void removeAtlasAll(const std::string& atlas, bool c) ;

  void removeAtlasItem(const std::string& atlas, Node* n, bool c);

  void removeAll(bool c);

  void removeItem(Node* n, bool c);

  void addAtlasItem(const std::string& atlas, Node* n, int zx, int tag);

  void addItem(Node* n, int zx, int tag);

  int incIndexZ();

  void setParentScene(Scene* par);

  Scene* scene();

  virtual bool init();

  XLayer(Dictionary* options);
  XLayer();
  virtual ~XLayer();

  CREATE_FUNC(XLayer)

private:

  void audioCallback(Ref*);

  Dictionary m_atlases;
  std::string m_type;
  int m_lastTag;
  int m_lastZix;

  CC_DISALLOW_COPY_AND_ASSIGN(XLayer)
};





NS_FI_END
#endif

