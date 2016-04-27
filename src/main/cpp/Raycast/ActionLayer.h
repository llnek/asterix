//
//  ActionLayer.h
//  Raycast
//
//  Created by Ray Wenderlich on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "cocos2d.h"
#import "Box2D.h"
#import "GLES-Render.h"
#import "LevelHelperLoader.h"
#import "HUDLayer.h"

@interface ActionLayer : CCLayer {
    b2World * _world;
    GLESDebugDraw * _debugDraw;
    LevelHelperLoader * _lhelper;
    double _playerVelX;
    CCSprite * _hero;
    b2Body * _heroBody;
    BOOL _gameOver;
    int _lives;
    b2ContactListener * _contactListener;
    BOOL _invincible;
    double _lastGround;
    int _numGroundContacts;
    HUDLayer * _hud;
}

+ (id)scene;
- (id)initWithHUD:(HUDLayer *)hud;

@end
