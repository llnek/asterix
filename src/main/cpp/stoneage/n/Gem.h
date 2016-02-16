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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/XConfig.h"
#include "core/ComObj.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gem : public c::Sprite {

  static Gem* create() {

  };

  Gem() {
    self.visibleSize = cc.Director:getInstance():getVisibleSize()
    self.origin = cc.Director:getInstance():getVisibleOrigin()
    self.type = constants.TYPE_GEM_BLUE;
    self.gemContainer = cc.Node:create()
    self.skin = cc.Sprite:create("gem_blue.png")
    self.selected = false
  }

};

function Gem:ctor()
    self:init()
end

function Gem:select()
    if (self.selected == true) then return end
    --show selected state
    self.selected = true
end

function Gem:deselect ()
    if (self.selected == false) then return end
    --clear selected state
    self.selected = false
end

function Gem:reset ()
    self.gemContainer:setScale (1, 1)
    self.gemContainer:setRotation(0)
end

function Gem:getType ()
    return self.type
end

function Gem:setType (value)

    self.type = value

    local textureCache = cc.Director:getInstance():getTextureCache()
    if (self.type == constants.TYPE_GEM_BLUE) then
        self.skin:setTexture(textureCache:addImage("gem_blue.png"))
    elseif (self.type == constants.TYPE_GEM_GREEN) then
        self.skin:setTexture(textureCache:addImage("gem_green.png"))
    elseif (self.type == constants.TYPE_GEM_YELLOW) then
        self.skin:setTexture(textureCache:addImage("gem_yellow.png"))
    elseif (self.type == constants.TYPE_GEM_RED) then
        self.skin:setTexture(textureCache:addImage("gem_red.png"))
    elseif (self.type == constants.TYPE_GEM_PINK) then
        self.skin:setTexture(textureCache:addImage("gem_pink.png"))
    elseif (self.type == constants.TYPE_GEM_WHITE) then
        self.skin:setTexture(textureCache:addImage("gem_white.png"))
   end
end

function Gem:init ()
	--self:setVisible(true)
    self:setVisible(false)
    self.gemContainer:addChild(self.skin)
    self:addChild(self.gemContainer)
end



NS_END




