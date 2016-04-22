
//
//  PhysicWorldScene.cpp
//  FlyingBird
//
//  Created by cocos on 14-2-9.
//
//

#include "PhysicWorldScene.h"
#include "BackgroundLayer.h"
#include "MainScene.h"
#define PROPES_COUNT 1
#define PROPES_POS 8
#define MAP_0_DOT_COUNT 3

PhysicWorldLayer::PhysicWorldLayer(){
    propsCount = 0;
    isCalledBefore = false;
    isGameOver = false;
    nScore = 0;
    collisionpropesType = 0;
    lastX = 0;
    isScoreAdded = false;
    CCUserDefault::sharedUserDefault()->setBoolForKey("IS_GAMEOVER", false);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("LastScore", 0);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("PROPES_TYPE", 0);
}

PhysicWorldLayer::~PhysicWorldLayer(){
    cpSpaceFree(space);
}

CCScene* PhysicWorldLayer::scene(){
    
    CCScene *scene = CCScene::create();
    PhysicWorldLayer *layer = PhysicWorldLayer::create();
    scene->addChild(layer);
    return scene;
}

bool PhysicWorldLayer::init(){
    
    if (!CCLayer::init()) {
        return false;
    }
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    createPhysicWorld();
    scheduleUpdate();
    schedule(schedule_selector(PhysicWorldLayer::endCollisionEffect), 4);
    schedule(schedule_selector(PhysicWorldLayer::addProps), 6);
    schedule(schedule_selector(PhysicWorldLayer::addMonsters), 4);
    return true;
}

void PhysicWorldLayer::createPhysicWorld(){
    
    space = cpSpaceNew();
    space->gravity = cpv(0, -600);
    
    //open debugDraw
    cocos2d::extension::CCPhysicsDebugNode* debugLayer = cocos2d::extension::CCPhysicsDebugNode::create(space);
	addChild(debugLayer, 100);
    debugLayer->setVisible(false);

    wallBottom = cpSegmentShapeNew(this->space->staticBody,
                                   cpv(0, 64),
                                   cpv(4294967295, 64),
                                   0);
    cpSpaceAddStaticShape(this->space, wallBottom);
    this->wallBottom->collision_type = 3;
    this->wallBottom->sensor = false;
    cpShapeSetUserData(wallBottom, this);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    wallTop = cpSegmentShapeNew(this->space->staticBody,
                                   cpv(0, size.height-10),
                                   cpv(4294967295, size.height-10),
                                   0);
    cpSpaceAddStaticShape(this->space, wallTop);
    this->wallTop->collision_type = 4;
    this->wallTop->sensor = false;
    cpShapeSetUserData(wallBottom, this);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprites.plist");
    spriteSheet = CCSpriteBatchNode::create("sprites.png");
    addChild(spriteSheet, 100);
    
    bird = Bird::create(space);
    spriteSheet->addChild(bird,10);
    
    backgroundLayer = BackgroundLayer::create(spriteSheet,space);
    addChild(backgroundLayer);

    //add Props
    CCPoint point = CCPoint(backgroundLayer->daojuX[1],backgroundLayer->daojuY[1]);
    propSp = PropsSprites::create(space, 1, point);
    spriteSheet->addChild(propSp,10);
}

void PhysicWorldLayer::update(float delta)
{
//    int integerKey = CCUserDefault::sharedUserDefault()->getIntegerForKey("PROPES_TYPE",0);
//    if(5 == integerKey) {
//        if (bird->body->v.x <= 300) {
//            bird->body->v = cpvadd(cpv(100, 0),cpv(bird->body->v.x,0));
//        }
//    }else if(6 == integerKey){
//
//    }
    
    for (int i = 0; i<backgroundLayer->barArray->count(); i++) {
        CCDictionary* bar = (CCDictionary*)backgroundLayer->barArray->objectAtIndex(i);
        int x = (bar->valueForKey("x")->intValue()/2) + backgroundLayer->bgWidth*backgroundLayer->mapIndex;
        
        if(bird->getPositionX()>x){
            
            if (backgroundLayer->mapIndex<1) {
                nScore = i+1+(backgroundLayer->mapIndex)*(backgroundLayer->barArray->count())-2;
            }else {
                nScore = i+1+MAP_0_DOT_COUNT+(backgroundLayer->mapIndex-1)*(backgroundLayer->barArray->count());
            }
            sprintf(strScore, "%d",nScore);
            
            if (lastX < nScore) {
                //CCLOG("Audio Called");
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("getPropes.mp3");
                lastX = nScore;
            }
            
            CCUserDefault::sharedUserDefault()->setIntegerForKey("LastScore", nScore);
            //CCLOG("mapindex:%d arraycount:%d score:%d",backgroundLayer->mapIndex,backgroundLayer->barArray->count(),nScore);
        }
    }
    
    cpSpaceStep(space, delta);
    lastEyeX = bird->getPositionX() - bird->getoffsetPx();
//    if(lastEyeX == 208.0)
//    {
//        backgroundLayer->loadObject(backgroundLayer->getNextMap(), backgroundLayer->getMapIndex() + 1);
//    }
    
    if(true == backgroundLayer->checkAndReload(lastEyeX))
    {
        //CCLog("reload ");
        backgroundLayer->getObjManager()->removeObjectOfMap(backgroundLayer->getMapIndex() - 1);
        backgroundLayer->loadObject(backgroundLayer->getCurMap(), backgroundLayer->getMapIndex());
        backgroundLayer->loadObject(backgroundLayer->getNextMap(), backgroundLayer->getMapIndex() + 1);
    }
    
    CCCamera *camera = this->getCamera();
    float eyeZ = camera->getZEye();
    camera->setEyeXYZ(lastEyeX, 0, eyeZ);
    camera->setCenterXYZ(lastEyeX, 0, 0);
    
    cpSpaceAddCollisionHandler(this->space, 2, 1, collisionBegin, NULL, NULL, NULL, this->bird->body);
    cpSpaceAddCollisionHandler(this->space, 1, 3, collisionBegin, NULL, NULL, NULL, this->bird->body);
    cpSpaceAddCollisionHandler(this->space, 4, 1, getPropesBegin, NULL, NULL, NULL, this->bird->body);
    cpSpaceAddCollisionHandler(this->space, 1, 5, getPropesBegin, NULL, NULL, NULL, this->bird->body);
    //cpSpaceAddCollisionHandler(this->space, 1, 6, getPropesBegin, NULL, NULL, NULL, this->bird->body);
    cpSpaceAddCollisionHandler(this->space, 7, 1, collisionBegin, NULL, NULL, NULL, this->bird->body);
    cpSpaceAddCollisionHandler(this->space, 4, 7, getPropesBegin, NULL, NULL, NULL, NULL);
    cpSpaceAddCollisionHandler(this->space, 2, 7, collisionBegin, NULL, NULL, NULL, NULL);
    cpSpaceAddCollisionHandler(this->space, 5, 7, collisionBegin, NULL, NULL, NULL, NULL);
}

void PhysicWorldLayer::postStepRemove(cpSpace *space, cpShape *shape, void *param)
{
    //CCLOG("check");
    int integerKey = CCUserDefault::sharedUserDefault()->getIntegerForKey("PROPES_TYPE",0);
    cpBody* body = (cpBody*)param;
    CCPhysicsSprite* sprite = (CCPhysicsSprite*)shape->data;
    switch (shape->collision_type) {
        case 1:
            //CCLOG("Get pipe");
            if (5 == integerKey) {
                sprite->setOpacity(100);
                propsCount++;
            }
//            else if (6 == integerKey){
//      
//            }
            else{
                sprite->getCPBody()->a = 180;
                sprite->getCPBody()->v = cpv(0, -500);
            }
            break;
        case 3:
            //CCLOG("Get ground");
            body->a = 180;
            CCUserDefault::sharedUserDefault()->setBoolForKey("IS_GAMEOVER", true);
            break;
        case 4:
            body->a = 180;
            body->v = cpv(0, -500);
            break;
        case 0:
        case 5:
        //case 6:
        //case 7:
        //case 8:
            //CCLOG("GET propes Type:%lu",shape->collision_type);
            CCUserDefault::sharedUserDefault()->setIntegerForKey("PROPES_TYPE", shape->collision_type);
            cpSpaceRemoveStaticShape(space, shape);
            //cpSpaceRemoveShape(space, shape);
            cpShapeFree(shape);
            cpBodyFree(sprite->getCPBody());
            sprite->removeFromParentAndCleanup(true);
            break;
        default:
            break;
    }
}

int PhysicWorldLayer::getPropesBegin(cpArbiter *arb, cpSpace *space, void *param)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)PhysicWorldLayer::postStepRemove, b, NULL);
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)PhysicWorldLayer::postStepRemove, a, param);
    return 0;
}

int PhysicWorldLayer::collisionBegin(cpArbiter *arb, cpSpace *space, void *param)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
    cpSpaceAddPostStepCallback(space, (cpPostStepFunc)PhysicWorldLayer::postStepRemove, b, param);
    return 0;
}

void PhysicWorldLayer::addProps(float dt){
    int type = CCRANDOM_0_1()*PROPES_COUNT+1;
    int posRand = CCRANDOM_0_1()*backgroundLayer->daojuArray->count()+1;
    
    CCPoint point = CCPoint(backgroundLayer->daojuX[posRand]+backgroundLayer->bgWidth*(backgroundLayer->mapIndex+1),backgroundLayer->daojuY[posRand]);
    propSp = PropsSprites::create(space, type, point);
    spriteSheet->addChild(propSp,10);
}

void PhysicWorldLayer::addMonsters(float dt){
    int score = CCUserDefault::sharedUserDefault()->getIntegerForKey("LastScore", 0);
    if (score >= 10) {
        monster = Monster::create(this->space);
        monster->body->p = cpv(bird->body->p.x+backgroundLayer->bgWidth/2, 80);
        spriteSheet->addChild(monster,10);
    }
}

bool PhysicWorldLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("IS_GAMEOVER", false)==false) {
         CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("jump.mp3");
    }
    cpVect point = cpvsub(cpv(100,0), cpBodyGetVel(bird->body));
    if( point.y >= 0 ){
        cpBodyApplyImpulse(bird->body, cpv(0, 400), cpv(0, 0));
    }
    return true;
}
void PhysicWorldLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}
void PhysicWorldLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
void PhysicWorldLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
}

void PhysicWorldLayer::endCollisionEffect(float dt){
    CCLOG("Props Count:%d",propsCount);
    //bird->body->v = cpv(100, 0);
    //CCUserDefault::sharedUserDefault()->setIntegerForKey("PROPES_TYPE", 0);
    //bird->setOpacity(255);
    //if (propsCount < 2) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("PROPES_TYPE", 0);
        bird->setOpacity(255);
    //}else{
    //    propsCount--;
    //}
}
