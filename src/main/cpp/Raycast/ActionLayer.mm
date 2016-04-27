//
//  ActionLayer.mm
//  Raycast
//
//  Created by Ray Wenderlich on 8/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ActionLayer.h"
#import "SimpleAudioEngine.h"
#import "MonsterData.h"
#import "RaysCastCallback.h"
#import "SimpleContactListener.h"

#define MOVE_POINTS_PER_SECOND 80.0

@implementation ActionLayer

+ (id)scene {
    CCScene *scene = [CCScene node];
    
    HUDLayer *hud = [HUDLayer node];
    [scene addChild:hud z:1];
    
    ActionLayer *layer = [[[ActionLayer alloc] initWithHUD:hud] autorelease];
    [scene addChild:layer];
    
    return scene;
}

- (void)setupWorld {    
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    bool doSleep = false; 
    _world = new b2World(b2Vec2(0,0), doSleep);
    _contactListener = new SimpleContactListener(self);
    _world->SetContactListener(_contactListener);
}

- (void)setupLevelHelper {
    _lhelper = [[LevelHelperLoader alloc] initWithContentOfFile:@"TestLevel"];
    [_lhelper addObjectsToWorld:_world cocos2dLayer:self];
    [_lhelper createWorldBoundaries:_world];
    [_lhelper createGravity:_world];
    _hero = [_lhelper spriteWithUniqueName:@"hero"];
    NSAssert(_hero!=nil, @"Couldn't find hero");
    _heroBody = [_lhelper bodyWithUniqueName:@"hero"];
    NSAssert(_heroBody!=nil, @"Couldn't find hero body");
    NSArray *monsters = [_lhelper spritesWithTag:MONSTER];
    for (CCSprite *monster in monsters) {
        MonsterData *data = [[[MonsterData alloc] init] autorelease];
        [LevelHelperLoader setCustomValue:data withKey:@"data" onSprite:monster];
    }    
}

- (void)setupDebugDraw {    
    _debugDraw = new GLESDebugDraw([_lhelper pixelsToMeterRatio] *
                                   [[CCDirector sharedDirector] contentScaleFactor]);
    _world->SetDebugDraw(_debugDraw);
    _debugDraw->SetFlags(b2DebugDraw::e_shapeBit | 
                         b2DebugDraw::e_jointBit);
}

- (void)setupAudio {
    [[SimpleAudioEngine sharedEngine] playBackgroundMusic:@"Raycast.m4a"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"ground.wav"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"laser.wav"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"wing.wav"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"whine.wav"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"lose.wav"];
    [[SimpleAudioEngine sharedEngine] preloadEffect:@"win.wav"];
}

- (void)updateLives {
    [_hud setStatusString:[NSString stringWithFormat:@"Lives: %d", _lives]];
}


- (id)initWithHUD:(HUDLayer *)hud
{
    if ((self = [super init])) {
        _hud = hud;
        // Rest of method...
        [self setupWorld];
        [self setupLevelHelper];
        [self setupDebugDraw];
        [self setupAudio];
        [self scheduleUpdate];
        self.isTouchEnabled = YES;
        _lives = 3;
        [self updateLives];
    }
    return self;
}

-(void)setViewpointCenter:(CGPoint) position {
    
    CGSize winSize = [[CCDirector sharedDirector] winSize];
    CGRect worldRect = [_lhelper gameWorldSize];
    
    int x = MAX(position.x, worldRect.origin.x + winSize.width / 2);
    int y = MAX(position.y, worldRect.origin.y + winSize.height / 2);
    x = MIN(x, (worldRect.origin.x + worldRect.size.width) - winSize.width / 2);
    y = MIN(y, (worldRect.origin.y + worldRect.size.height) - winSize.height/2);
    CGPoint actualPosition = ccp(x, y);
    
    CGPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CGPoint viewPoint = ccpSub(centerOfView, actualPosition);
    
    self.position = viewPoint;
    
}

- (void)loseGame {
    [_hero runAction:[CCSequence actions:
                      [CCScaleBy actionWithDuration:0.35 scale:2.0],
                      [CCDelayTime actionWithDuration:0.75],
                      [CCScaleTo actionWithDuration:0.35 scale:0],
                      nil]];
    [_hero runAction:[CCRepeatForever actionWithAction:
                      [CCRotateBy actionWithDuration:0.5 angle:360]]]; 
    _gameOver = YES;
    [[SimpleAudioEngine sharedEngine] playEffect:@"lose.wav"];
    [_hud showRestartMenu:NO];
}

- (void)winGame {
    _gameOver = YES;
    [[SimpleAudioEngine sharedEngine] playEffect:@"win.wav"];
    [_hud showRestartMenu:YES];
}

- (void)updateHero:(ccTime)dt {
    if (_playerVelX != 0) {
        b2Vec2 b2Vel = _heroBody->GetLinearVelocity();
        b2Vel.x = _playerVelX / [_lhelper pixelsToMeterRatio];
        _heroBody->SetLinearVelocity(b2Vel);    
        
        if (_numGroundContacts > 0 && CACurrentMediaTime() - _lastGround > 0.25) {
            _lastGround = CACurrentMediaTime();
            [[SimpleAudioEngine sharedEngine] playEffect:@"ground.wav"];
            if ([_hero numberOfRunningActions] == 0) {
                [_lhelper startAnimationWithUniqueName:@"Walk" onSprite:_hero];
            }
        }
        
    } else if (_playerVelX == 0 && _numGroundContacts > 0) {
        [_lhelper stopAnimationWithUniqueName:@"Walk" onSprite:_hero];
    }
}

- (void)updateMonsters:(ccTime)dt {    
    NSArray *monsters = [_lhelper bodiesWithTag:MONSTER];
    for(NSValue* monsterValue in monsters) {
        b2Body* monsterBody = (b2Body*)[monsterValue pointerValue];
        monsterBody->ApplyForce(-1 * monsterBody->GetMass() * _world->GetGravity(), monsterBody->GetWorldCenter());
        monsterBody->SetAngularVelocity(1.0);    
        
        b2Vec2 eyeOffset = b2Vec2(0, -0.5);
        b2Vec2 eye = monsterBody->GetWorldPoint(eyeOffset);
        b2Vec2 target = eye - monsterBody->GetWorldCenter();
        target.Normalize();
        target *= 20.0;
        target = eye + target;

        CCSprite *monsterSprite = (CCSprite*)monsterBody->GetUserData();
        MonsterData * monsterData = [LevelHelperLoader customValueWithKey:@"data" forSprite:monsterSprite];            
        monsterData.eye = ccp(eye.x * [_lhelper pixelsToMeterRatio], eye.y * [_lhelper pixelsToMeterRatio]);
        monsterData.target = ccp(target.x * [_lhelper pixelsToMeterRatio], target.y * [_lhelper pixelsToMeterRatio]);
        monsterData.canSeePlayer = NO;
        
        RaysCastCallback callback;
        _world->RayCast(&callback, eye, target);

        if (callback.m_fixture) {
            monsterData.target = ccp(callback.m_point.x * [_lhelper pixelsToMeterRatio], callback.m_point.y * [_lhelper pixelsToMeterRatio]);
            if (callback.m_fixture->GetBody() == _heroBody) {    
                monsterData.canSeePlayer = TRUE;
                
                if (CACurrentMediaTime() - monsterData.lastShot > 1.0) {
                    monsterData.lastShot = CACurrentMediaTime();
                    
                    // Create and position laser
                    b2Body *laserBody = [_lhelper newBodyWithUniqueName:@"laserbeam_red" world:_world];
                    CCSprite *laserSprite = (CCSprite *)laserBody->GetUserData();
                    laserSprite.position = monsterData.eye;
                    laserSprite.rotation = monsterSprite.rotation;                        
                    laserBody->SetTransform(b2Vec2(laserSprite.position.x/[_lhelper pixelsToMeterRatio], 
                                                   laserSprite.position.y/[_lhelper pixelsToMeterRatio]), 
                                            CC_DEGREES_TO_RADIANS(-laserSprite.rotation));    
                    
                    // Make laser move
                    b2Vec2 laserVel = callback.m_point - eye;
                    laserVel.Normalize();
                    laserVel *= 4.0;
                    laserBody->SetLinearVelocity(laserVel);
                    
                    [[SimpleAudioEngine sharedEngine] playEffect:@"laser.wav"];
                                        
                }
                
            }
        }
        
    }    
}

- (void)updateViewpoint:(ccTime)dt {
    [self setViewpointCenter:_hero.position];
}

- (void)updateGameOver:(ccTime)dt {
    
    if (_gameOver) return;
    
    CGRect worldRect = [_lhelper gameWorldSize];
    if (_hero.position.x > (worldRect.origin.x + worldRect.size.width) * 0.95) {
        [self winGame];
    }
    
    if (_hero.position.y < 0.5) {
        [self loseGame];
    }
    
}

- (void)updateLevelHelper:(ccTime)dt {
    [_lhelper update:dt];
}

- (void)updateBox2D:(ccTime)dt {
    _world->Step(dt, 1, 1);
    _world->ClearForces();
}

- (void)updateSprites:(ccTime)dt {
    
    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			CCSprite *myActor = (CCSprite*)b->GetUserData();
            
			myActor.position = CGPointMake(b->GetPosition().x * [_lhelper pixelsToMeterRatio], 
                                           b->GetPosition().y * [_lhelper pixelsToMeterRatio]);
			myActor.rotation = -1 * CC_RADIANS_TO_DEGREES(b->GetAngle());
        }
    }
}

- (void)update:(ccTime)dt {
    [self updateHero:dt];
    [self updateMonsters:dt];
    [self updateLevelHelper:dt];
    [self updateBox2D:dt];
    [self updateSprites:dt];
    [self updateViewpoint:dt];
    [self updateGameOver:dt];
}

-(void) draw {  
    
    glClearColor(98.0/255.0, 183.0/255.0, 214.0/255.0, 255.0/255.0);
    glClear(GL_COLOR_BUFFER_BIT);	
    
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    _world->DrawDebugData();
    
    NSArray *monsters = [_lhelper spritesWithTag:MONSTER];
    for (CCSprite *monster in monsters) {
        MonsterData * data = [LevelHelperLoader customValueWithKey:@"data" forSprite:monster];
        if (!data.canSeePlayer) {
            glColor4ub(0, 255, 0, 255);
        } else {
            glColor4ub(255, 0, 0, 255);
        }
        ccDrawLine(data.eye, data.target);
    }
        
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}

- (void)handleTouchAtPoint:(CGPoint)touchLocation {        
    if (touchLocation.x < _hero.position.x) {
        _playerVelX = -MOVE_POINTS_PER_SECOND;
        _hero.flipX = YES;
    } else {
        _playerVelX = MOVE_POINTS_PER_SECOND;
        _hero.flipX = NO;
    }    
}

- (void)ccTouchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {    
    
    if (_gameOver) return;
    
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [self convertTouchToNodeSpace:touch];
    
    [self handleTouchAtPoint:touchLocation];    
    
    if (touch.tapCount > 1) {
        _heroBody->ApplyLinearImpulse(b2Vec2(_playerVelX/[_lhelper pixelsToMeterRatio], 1.25), _heroBody->GetWorldCenter());
        [[SimpleAudioEngine sharedEngine] playEffect:@"wing.wav"];
        [_lhelper startAnimationWithUniqueName:@"Flap" onSprite:_hero];
    }
    
}

- (void)ccTouchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    
    if (_gameOver) return;
    
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [self convertTouchToNodeSpace:touch];
    [self handleTouchAtPoint:touchLocation];
}

- (void)ccTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    _playerVelX = 0;
}

- (void)ccTouchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    _playerVelX = 0;
}

- (void)beginContact:(b2Contact *)contact {
    
    if (_gameOver) return;
    
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    b2Body *bodyA = fixtureA->GetBody();
    b2Body *bodyB = fixtureB->GetBody();
    CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
    CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
    
    if (!_invincible) {
        if ((spriteA == _hero && spriteB.tag == LASER) ||
            (spriteB == _hero && spriteA.tag == LASER)) {
            _lives--;
            [self updateLives];
            [[SimpleAudioEngine sharedEngine] playEffect:@"whine.wav"];
            if (_lives == 0) {
                [self loseGame];
                return;
            }
            _invincible = YES;
            [_hero runAction:
             [CCSequence actions:
              [CCBlink actionWithDuration:1.5 blinks:9],
              [CCCallBlock actionWithBlock:^(void) {
                 _invincible = NO;
             }],
              nil]];
        }
    }    
    
    if ((spriteA == _hero && spriteB.tag == GROUND) ||
        (spriteB == _hero && spriteA.tag == GROUND)) {
        if (_numGroundContacts == 0) {
            [_lhelper stopAnimationWithUniqueName:@"Flap" onSprite:_hero];
        }
        _numGroundContacts++;
    }

}

- (void)endContact:(b2Contact *)contact {
    
    b2Fixture *fixtureA = contact->GetFixtureA();
    b2Fixture *fixtureB = contact->GetFixtureB();
    b2Body *bodyA = fixtureA->GetBody();
    b2Body *bodyB = fixtureB->GetBody();
    CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
    CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
    
    if ((spriteA == _hero && spriteB.tag == GROUND) ||
        (spriteB == _hero && spriteA.tag == GROUND)) {
        _numGroundContacts--;
    }
}

- (void)dealloc {
    _world->SetContactListener(NULL);
    delete _contactListener;
    [_lhelper release];
    _lhelper = nil;
    delete _world;
    [super dealloc];
}

@end
