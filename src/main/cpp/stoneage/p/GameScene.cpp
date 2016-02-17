


struct CC_DLL GLayer : public f::GameLayer {

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {

  shared=engine->getNodeList(SharedNode().typeId());
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb= cx::vixBox();

  ss->gridController = GridController:create();
  ss->gridAnimations = GridAnimations:create();

  ss->selectedGemPosition = f::Cell2P();
  ss->gemsContainer = c::Node:create();
  ss->selectedIndex = f::Cell2P();
  ss->targetIndex = f::Cell2P();
  ss->schedulerID = CC_NIL;
  ss->selectedGem = CC_NIL;
  ss->targetGem = CC_NIL;
  ss->combos = 0;
  ss->addingCombos = false;

  ss->gemsContainer->setPosition( 25, 80);
  addItem(ss->gemsContainer);

  auto frame = cx::loadSprite("frame.png");
  frame->setPosition(wb.cx, wb.cy);
  addItem(frame);

  buildGrid();
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::decorate() {
  centerImage("game.bg");
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchDown(t->getLocation());
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchMove(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(f::ComObj *co, c::Touch *t) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->gridController->onTouchUp(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::buildGrid () {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->enabled = false;
  for (auto c = 0; c < GRID_SIZE_X; ++c) {
    auto m= new f::FArrayPtr<Gem>(GRID_SIZE_Y);
    auto g= mc_new1(f::FArrInt,GRID_SIZE_Y);

    ss->gridGemsColumnMap.push_back(m);
    ss->grid.push_back(g);

    for (auto r = 0; r < GRID_SIZE_Y; ++r) {
      auto gem = Gem:create();
      auto idx= c<2
        ? getVerticalUnique(c,r) : getVerticalHorizontalUnique(c,r);
      auto t= getGemType(idx);

      gem->setType(t);
      g->set(r,t);

      gem->setPosition(
          c*(TILE_SIZE + GRID_SPACE),
          r*(TILE_SIZE + GRID_SPACE));

      ss->gemsContainer->addChild(gem);
      m->set(r,gem);
      ss->allGems.push_back(gem);
    }
  }
  ss->gridAnimations->animateIntro();
}

//////////////////////////////////////////////////////////////////////////////
//

function GameScene:collapseGrid ()

    local function onGridCollapseComplete (sender)

        local function onMatchedAnimatedOut (sender)
            self:collapseGrid()
        end

        for i = 1, #self.allGems do
            local gem = self.allGems[i]
            local xIndex = math.ceil ((gem:getPositionX() - constants.TILE_SIZE * 0.5) / (constants.TILE_SIZE + constants.GRID_SPACE))
            local yIndex = math.ceil ((gem:getPositionY() - constants.TILE_SIZE * 0.5) / (constants.TILE_SIZE + constants.GRID_SPACE))
            self.gridGemsColumnMap[xIndex][yIndex] = gem
            self.grid[xIndex][yIndex] = gem.type
        end

        if (self.gridController:checkGridMatches () == true) then
           --animate matched games
           if (self.addingCombos == true) then
               if (#self.gridController.matchArray > 3) then self.combos = self.combos + (#self.gridController.matchArray - 3) end
           end
           self.gridAnimations:animateMatches (self.gridController.matchArray, onMatchedAnimatedOut)
           self:showMatchParticle (self.gridController.matchArray)
           self:setGemsScore(#self.gridController.matchArray * constants.POINTS)
           self:playFX("match.wav")
        else
            --no more matches, check for combos
            if (self.combos > 0) then
                --now turn random gems into diamonds
                local diamonds = {}
                local removeGems = {}
                local i = 0

                math.randomseed(os.clock())

                while i < self.combos do

                    i = i + 1

                    local randomGem = nil
                    local randomX,randomY = 0

                    while randomGem == nil do
                        randomX = math.random(1, constants.GRID_SIZE_X)
                        randomY = math.random(1, constants.GRID_SIZE_Y)
                        randomGem = self.gridGemsColumnMap[randomX][randomY]
                        if (randomGem.type == constants.TYPE_GEM_WHITE) then randomGem = nil end
                    end

                    local diamond = self.objectPools:getDiamond()
                    diamond:setPosition(randomGem:getPositionX(), randomGem:getPositionY())

                    local diamondParticle = self.objectPools:getDiamondParticle()
                    diamondParticle:setPosition(randomGem:getPositionX(), randomGem:getPositionY())

                    table.insert(diamonds, diamond)
                    table.insert(removeGems, {x=randomX, y=randomY})

                end
                self:setDiamondScore(#diamonds * constants.DIAMOND_POINTS)
                self.gridAnimations:animateMatches(removeGems, onMatchedAnimatedOut)
                self.gridAnimations:collectDiamonds(diamonds)
                self.combos = 0
                self:playFX("diamond.wav")
            else
                self.gridController.enabled = true
            end
            self.addingCombos = false
        end
    end

    for i = 1, #self.gridController.matchArray do
        self.grid[self.gridController.matchArray[i].x][self.gridController.matchArray[i].y] = -1
    end

    local column = nil
    local newColumn = nil
    local i

    for c = 1, constants.GRID_SIZE_X do
        column = self.grid[c]
        newColumn = {}
        i = 1
        while #newColumn < #column do
            if (#column > i) then
                if (column[i] ~= -1) then
                    --move gem
                    table.insert(newColumn, column[i])
                end
            else
                --create new gem
                table.insert(newColumn, 1, column[i])
            end
            i = i+1
        end
        self.grid[c] = newColumn
    end
    self.gridAnimations:animateCollapse (onGridCollapseComplete)
end

function GameScene:dropSelectedGem ()
    self.selectedGem:setLocalZOrder(constants.Z_GRID)
    self.gridAnimations:resetSelectedGem()
end

function GameScene:getNewGem ()
    return math.random(1, #constants.TYPES)
end

function GameScene:addToScore ()
    for i = 1, #self.gridController.matchArray do
        local position = self.gridController.matchArray[i]
        local gem = self.gridGemsColumnMap[position.x][position.y]
        if (gem.type == constants.TYPE_GEM_WHITE) then
            --got a diamond
        end
    end
end

function GameScene:showMatchParticle (matches)

    for i = 1, #matches do
        local gem = self.gridGemsColumnMap[matches[i].x][matches[i].y]
        local particle = self.objectPools:getMatchParticle()
        particle:setPosition(gem:getPositionX() + self.gemsContainer:getPositionX(), gem:getPositionY()  + self.gemsContainer:getPositionY())
    end

end

function GameScene:setDiamondScore (value)
    self.diamondScore = self.diamondScore  + value
    self.diamondScoreLabel:setString("" .. self.diamondScore)
end

function GameScene:setGemsScore (value)
    self.gemsScore = self.gemsScore  + value
    self.scoreLabel:setString("" .. self.gemsScore)
end

function GameScene:playFX(name)
    local fx = cc.FileUtils:getInstance():fullPathForFilename(name)
    cc.SimpleAudioEngine:getInstance():playEffect(fx)
end

function GameScene:startTimer()

    local timeBarBg = cc.Sprite:create("timeBarBg.png")
    timeBarBg:setPosition(self.middle.x, 40)
    self:addChild(timeBarBg)

    local timeBar = cc.Sprite:create("timeBar.png")
    timeBar:setAnchorPoint(cc.p(0,0.5))
    timeBar:setPosition(self.middle.x - 290, 40)
    self:addChild(timeBar)

    local function tick()
        local scaleNow = timeBar:getScaleX()
        local speed = 0.007
        if (scaleNow - speed > 0) then
            timeBar:setScaleX(scaleNow - speed)
        else
            --GameOver!!!
            timeBar:setScaleX(0)
            cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.schedulerID)
            cc.SimpleAudioEngine:getInstance():stopMusic()
            self.running = false
            -- show game over
            local gameOver = cc.Sprite:create("gameOver.png")
            gameOver:setPosition(self.middle.x, self.middle.y)
            self:addChild(gameOver)
        end
    end

    self.schedulerID = cc.Director:getInstance():getScheduler():scheduleScriptFunc(tick, 1, false)
end

return GameScene
