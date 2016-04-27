//
//  HelloWorldLayer.mm
//  TinySeal
//
//  Created by Ray Wenderlich on 4/18/13.
//  Copyright Razeware LLC 2013. All rights reserved.
//

#import "HelloWorldLayer.h"
#import "Terrain.h"
#import "Box2D.h"
#import "Hero.h"
#import "SimpleAudioEngine.h"

@interface HelloWorldLayer() {
    CCSprite *_background;
    Terrain * _terrain;
    
    b2World * _world;
    Hero * _hero;
    
    BOOL _tapDown;
}
@end
 
@implementation HelloWorldLayer
 
+(CCScene *) scene {
	CCScene *scene = [CCScene node];
	HelloWorldLayer *layer = [HelloWorldLayer node];
	[scene addChild: layer];
	return scene;
}

-(CCSprite *)spriteWithColor:(ccColor4F)bgColor textureWidth:(float)textureWidth textureHeight:(float)textureHeight {
 
    // 1: Create new CCRenderTexture
    CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:textureWidth height:textureHeight];
 
    // 2: Call CCRenderTexture:begin
    [rt beginWithClear:bgColor.r g:bgColor.g b:bgColor.b a:bgColor.a];
 
    // 3: Draw into the texture
    
    self.shaderProgram = [[CCShaderCache sharedShaderCache] programForKey:kCCShader_PositionColor];
    CC_NODE_DRAW_SETUP();
     
    // 3: Draw into the texture
    float gradientAlpha = 0.7f;
    CGPoint vertices[4];
    ccColor4F colors[4];
    int nVertices = 0;
     
    vertices[nVertices] = CGPointMake(0, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0 };
    vertices[nVertices] = CGPointMake(textureWidth, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    vertices[nVertices] = CGPointMake(0, textureHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
    vertices[nVertices] = CGPointMake(textureWidth, textureHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
     
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position  | kCCVertexAttribFlag_Color);
     
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
    
    CCSprite *noise = [CCSprite spriteWithFile:@"Noise.png"];
    [noise setBlendFunc:(ccBlendFunc){GL_DST_COLOR, GL_ZERO}];
    noise.position = ccp(textureWidth/2, textureHeight/2);
    [noise visit];
 
    // 4: Call CCRenderTexture:end
    [rt end];
 
    // 5: Create a new Sprite from the texture
    return [CCSprite spriteWithTexture:rt.sprite.texture];
 
}

- (ccColor4F)randomBrightColor {
 
    while (true) {
        float requiredBrightness = 192;
        ccColor4B randomColor = 
            ccc4(arc4random() % 255,
                 arc4random() % 255, 
                 arc4random() % 255, 
                 255);
        if (randomColor.r > requiredBrightness || 
            randomColor.g > requiredBrightness ||
            randomColor.b > requiredBrightness) {
            return ccc4FFromccc4B(randomColor);
        }        
    }
 
}
 
- (void)genBackground {
    
    [_background removeFromParentAndCleanup:YES];
    
    ccColor4F bgColor = [self randomBrightColor];
    _background = [self spriteWithColor:bgColor textureWidth:IS_IPHONE_5 ? 1024:512 textureHeight:512];
    
    CGSize winSize = [CCDirector sharedDirector].winSize;
    _background.position = ccp(winSize.width/2, winSize.height/2);
    ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    [_background.texture setTexParameters:&tp];
    
    [self addChild:_background];
    
    ccColor4F color3 = [self randomBrightColor];
    ccColor4F color4 = [self randomBrightColor];
    CCSprite *stripes = [self stripedSpriteWithColor1:color3 color2:color4
                                         textureWidth:IS_IPHONE_5 ? 1024:512    textureHeight:512 stripes:4];
    ccTexParams tp2 = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
    [stripes.texture setTexParameters:&tp2];
    _terrain.stripes = stripes;
    
}

- (void)setupWorld {
    b2Vec2 gravity = b2Vec2(0.0f, -7.0f);
    bool doSleep = true;
    _world = new b2World(gravity);
    _world->SetAllowSleeping(doSleep);
}

- (void)createTestBodyAtPostition:(CGPoint)position {
    
    b2BodyDef testBodyDef;
    testBodyDef.type = b2_dynamicBody;
    testBodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
    b2Body * testBody = _world->CreateBody(&testBodyDef);
    
    b2CircleShape testBodyShape;
    b2FixtureDef testFixtureDef;
    testBodyShape.m_radius = 25.0/PTM_RATIO;
    testFixtureDef.shape = &testBodyShape;
    testFixtureDef.density = 1.0;
    testFixtureDef.friction = 0.2;
    testFixtureDef.restitution = 0.5;
    testBody->CreateFixture(&testFixtureDef);
    
}

- (void) onEnter {
    [super onEnter];
    
    [self setupWorld];
    _terrain = [[[Terrain alloc] initWithWorld:_world] autorelease];
    [self addChild:_terrain z:1];
    
    [self genBackground];
    [self setTouchEnabled:YES];
    [self scheduleUpdate];
    
    _hero = [[[Hero alloc] initWithWorld:_world] autorelease];
    [_terrain.batchNode addChild:_hero];
    
    [[SimpleAudioEngine sharedEngine] playBackgroundMusic:@"TinySeal.caf" loop:YES];
//    self.scale = 0.25;
}
 
- (void)ccTouchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [self genBackground];
    _tapDown = YES;
    [_hero runForceAnimation];
}

-(void)ccTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    _tapDown = NO;
    [_hero runNormalAnimation];
}

- (void)ccTouchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    _tapDown = NO;
    [_hero runNormalAnimation];
}

-(CCSprite *)stripedSpriteWithColor1:(ccColor4F)c1 color2:(ccColor4F)c2 textureWidth:(float)textureWidth
   textureHeight:(float)textureHeight stripes:(int)nStripes {
 
    // 1: Create new CCRenderTexture
    CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:textureWidth height:textureHeight];
 
    // 2: Call CCRenderTexture:begin
    [rt beginWithClear:c1.r g:c1.g b:c1.b a:c1.a];
 
    // 3: Draw into the texture
 
    // Layer 1: Stripes
    CGPoint vertices[nStripes*6];
    ccColor4F colors[nStripes*6];
 
    int nVertices = 0;
    float x1 = -textureHeight;
    float x2;
    float y1 = textureHeight;
    float y2 = 0;
    float dx = textureWidth / nStripes * 2;
    float stripeWidth = dx/2;
    for (int i=0; i<nStripes; i++) {
        x2 = x1 + textureHeight;
 
        vertices[nVertices] = CGPointMake(x1, y1);
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
 
        vertices[nVertices] = CGPointMake(x1+stripeWidth, y1);
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
 
        vertices[nVertices] = CGPointMake(x2, y2);
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
 
        vertices[nVertices] = vertices[nVertices-2];
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
 
        vertices[nVertices] = vertices[nVertices-2];
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
 
        vertices[nVertices] = CGPointMake(x2+stripeWidth, y2);
        colors[nVertices++] = (ccColor4F){c2.r, c2.g, c2.b, c2.a};
        x1 += dx;
    }
 
    self.shaderProgram =
    [[CCShaderCache sharedShaderCache] programForKey:kCCShader_PositionColor];
 
    // Layer 2: Noise
    CCSprite *noise = [CCSprite spriteWithFile:@"Noise.png"];
    [noise setBlendFunc:(ccBlendFunc){GL_DST_COLOR, GL_ZERO}];
    noise.position = ccp(textureWidth/2, textureHeight/2);
    [noise visit];
 
    // Layer 3: Stripes
    CC_NODE_DRAW_SETUP();
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);
    
    float gradientAlpha = 0.7;
    
    nVertices = 0;
    
    vertices[nVertices] = CGPointMake(0, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    vertices[nVertices] = CGPointMake(textureWidth, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    vertices[nVertices] = CGPointMake(0, textureHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
    
    vertices[nVertices] = CGPointMake(textureWidth, textureHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
    
    // layer 3: top highlight
    float borderHeight = textureHeight/16;
    float borderAlpha = 0.3f;
    nVertices = 0;
    
    vertices[nVertices] = CGPointMake(0, 0);
    colors[nVertices++] = (ccColor4F){1, 1, 1, borderAlpha};
    
    vertices[nVertices] = CGPointMake(textureWidth, 0);
    colors[nVertices++] = (ccColor4F){1, 1, 1, borderAlpha};
    
    vertices[nVertices] = CGPointMake(0, borderHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    vertices[nVertices] = CGPointMake(textureWidth, borderHeight);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
    
    // 4: Call CCRenderTexture:end
    [rt end];
 
    // 5: Create a new Sprite from the texture
    return [CCSprite spriteWithTexture:rt.sprite.texture];
}

 
-(id) init {
	if((self=[super init])) {
	}
	return self;
}

- (void)update:(ccTime)dt {
    
    if (_tapDown) {
        if (!_hero.awake) {
            [_hero wake];
            _tapDown = NO;
        } else {
            [_hero dive];
        }
    }else {
        [_hero nodive];
    }
    [_hero limitVelocity];
    
    static double UPDATE_INTERVAL = 1.0f/60.0f;
    static double MAX_CYCLES_PER_FRAME = 5;
    static double timeAccumulator = 0;
    
    timeAccumulator += dt;
    if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
        timeAccumulator = UPDATE_INTERVAL;
    }
    
    int32 velocityIterations = 3;
    int32 positionIterations = 2;
    while (timeAccumulator >= UPDATE_INTERVAL) {
        timeAccumulator -= UPDATE_INTERVAL;
        _world->Step(UPDATE_INTERVAL,
                     velocityIterations, positionIterations);
        _world->ClearForces();
        
    }
    
//    float PIXELS_PER_SECOND = 100;
//    static float offset = 0;
//    offset += PIXELS_PER_SECOND * dt;
    [_hero update];
    
    CGSize winSize = [CCDirector sharedDirector].winSize;
    float scale = (winSize.height*3/4) / _hero.position.y;
    if (scale > 1) scale = 1;
    _terrain.scale = scale;
    
    float offset = _hero.position.x;
    
    CGSize textureSize = _background.textureRect.size;
    [_background setTextureRect:CGRectMake(offset * 0.7, 0, textureSize.width, textureSize.height)];
    [_terrain setOffsetX:offset];
    
}
@end
