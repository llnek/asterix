//
//  TLCMyScene.m
//  CutTheVerlet
//
//  Created by Tammy Coron on 4/10/14.
//  Copyright (c) 2014 Tammy Coron. All rights reserved.
//

#import "TLCMyScene.h"

#import "TLCAppDelegate.h"
#import "TLCGameData.h"

#import "TLCRope.h"

#define lengthOfRope1 24
#define lengthOfRope2 18
#define lengthOfRope3 15

#define prizeIsDynamicsOnStart YES
#define canCutMultipleRopesAtOnce NO

@import AVFoundation;

@interface TLCMyScene() <SKPhysicsContactDelegate, TLCRopeDelegate>

@property (nonatomic, strong) SKNode *worldNode;
@property (nonatomic, strong) SKSpriteNode *background;
@property (nonatomic, strong) SKSpriteNode *ground;

@property (nonatomic, strong) SKSpriteNode *player;

@property (nonatomic, strong) SKSpriteNode *treeLeft;
@property (nonatomic, strong) SKSpriteNode *treeRight;

@property (nonatomic, strong) SKSpriteNode *prize;

@property (nonatomic, strong) NSMutableArray *ropes;

@property (nonatomic, assign) CGPoint touchStartPoint;
@property (nonatomic, assign) CGPoint touchEndPoint;
@property (nonatomic, assign) BOOL touchMoving;

@property (nonatomic, assign) BOOL scoredPoint;
@property (nonatomic, assign) BOOL hitGround;

@property (nonatomic, strong) SKAction *soundCutAction;
@property (nonatomic, strong) SKAction *soundSplashAction;
@property (nonatomic, strong) SKAction *soundNomNomNomAction;

@end

@implementation TLCMyScene

#pragma mark -
#pragma mark Init Methods

-(instancetype)initWithSize:(CGSize)size
{
    if (self = [super initWithSize:size]) {
        /* Setup your scene here */
        
        self.worldNode = [SKNode node];
        [self addChild:self.worldNode];
        
        self.worldNode.scene.physicsWorld.contactDelegate = self;
        self.worldNode.scene.physicsWorld.gravity = CGVectorMake(0.0,-9.8);
        self.worldNode.scene.physicsWorld.speed = 1.0;
        
        [self setupSounds];
        [self setupGameData];
        
        [self setupBackground];
        [self setupTrees];
        [self setupPlayer];
        
        [self setupRopes];
    }
    
    return self;
}

- (void)setupGameData
{
    /*
     In production, you can use a plist, or something, that contains the setup data. With the name, you'd use the index number from the array.
     Here, we're just harcoding some percentages and stuff to make it a little easier. DO NOT do this in production.
     */
    
    self.ropes = [NSMutableArray array];

    TLCGameData *rope1 = [[TLCGameData alloc] init];
    rope1.name = 0;
    rope1.ropeLocation = CGPointMake(self.size.width *.12, self.size.height * .94);
    rope1.ropeLength = lengthOfRope1;
    rope1.isPrimaryRope = YES;
    [self.ropes addObject:rope1];

    TLCGameData *rope2 = [[TLCGameData alloc] init];
    rope2.name = 1;
    rope2.ropeLocation = CGPointMake(self.size.width *.85, self.size.height * .90);
    rope2.ropeLength = lengthOfRope2;
    rope2.isPrimaryRope = NO;
    [self.ropes addObject:rope2];

    TLCGameData *rope3 = [[TLCGameData alloc] init];
    rope3.name = 2;
    rope3.ropeLocation = CGPointMake(self.size.width *.86, self.size.height * .76);
    rope3.ropeLength = lengthOfRope3;
    rope3.isPrimaryRope = NO;
    [self.ropes addObject:rope3];
}

#pragma mark -
#pragma mark Code For Sound & Ambiance

- (void)setupSounds
{
    [self playBackgroundMusic:kSoundFileNameForBackgroundMusic];
    
    self.soundCutAction = [SKAction playSoundFileNamed:kSoundFileNameForCutAction waitForCompletion:NO];
    self.soundSplashAction = [SKAction playSoundFileNamed:kSoundFileNameForSplashAction waitForCompletion:NO];
    self.soundNomNomNomAction = [SKAction playSoundFileNamed:kSoundFileNameForBiteAction waitForCompletion:NO];
}

- (void)playBackgroundMusic:(NSString *)filename
{
    NSError *error;
    NSURL *backgroundMusicURL = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
    
    /* Option One : great for when the BG Music Changes Per Scene */
    
    /*
     self.backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
     self.backgroundMusicPlayer.numberOfLoops = -1;
     self.backgroundMusicPlayer.volume = 1.0;
     [self.backgroundMusicPlayer prepareToPlay];
     
     [self.backgroundMusicPlayer play];
     */
    
    /* Option Two (App Delegate) : great for when the BG Music remains constant */
    
    TLCAppDelegate *appDelegate = (TLCAppDelegate *)[[UIApplication sharedApplication] delegate];
    
    if (!appDelegate.backgroundMusicPlayer) { // not yet initialized, go ahead and set it up
        appDelegate.backgroundMusicPlayer = nil;
        appDelegate.backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
        appDelegate.backgroundMusicPlayer.numberOfLoops = -1;
        appDelegate.backgroundMusicPlayer.volume = 1.0;
        [appDelegate.backgroundMusicPlayer prepareToPlay];
    }
    
    if (!appDelegate.backgroundMusicPlayer.isPlaying) { // is it currently playing? if not, play music
        [appDelegate.backgroundMusicPlayer play];
    }
}

#pragma mark -
#pragma mark Setup Methods

#pragma mark - Background, Foreground and Ropes

- (void)setupBackground
{
    self.background = [SKSpriteNode spriteNodeWithImageNamed:@"background"];
    self.background.anchorPoint = CGPointMake(0.5, 1);
    self.background.position = CGPointMake(self.size.width/2, self.size.height);
    self.background.zPosition = LayerBackground;

    [self.worldNode addChild:self.background];

    self.ground = [SKSpriteNode spriteNodeWithImageNamed:@"ground"];
    self.ground.anchorPoint = CGPointMake(0.5, 1);
    self.ground.position = CGPointMake(self.size.width/2, self.background.frame.origin.y);
    self.ground.zPosition = LayerBackground;

    [self.worldNode addChild:self.ground];

    SKSpriteNode *water = [SKSpriteNode spriteNodeWithImageNamed:@"water"];
    water.anchorPoint = CGPointMake(0.5, 1);
    water.position = CGPointMake(self.size.width/2, self.ground.frame.origin.y + 10);
    water.zPosition = LayerBackground;

    // making the size a little shorter so the prize will look like its landed in the water
    CGSize bodySize = CGSizeMake(water.frame.size.width, water.frame.size.height -100);
    
    water.physicsBody = [SKPhysicsBody bodyWithRectangleOfSize:bodySize];
    water.physicsBody.dynamic = NO;
    
    water.physicsBody.categoryBitMask = EntityCategoryGround;
    water.physicsBody.collisionBitMask = EntityCategoryPrize;
    water.physicsBody.contactTestBitMask =  EntityCategoryPrize;
    
    [self.worldNode addChild:water];
}

- (void)setupTrees
{
    self.treeLeft = [SKSpriteNode spriteNodeWithImageNamed:@"treeLeft"];
    self.treeLeft.anchorPoint = CGPointMake(0.5, 1);
    self.treeLeft.position = CGPointMake(self.size.width * .20, self.size.height);
    self.treeLeft.zPosition = LayerForeground;

    [self.worldNode addChild:self.treeLeft];

    self.treeRight = [SKSpriteNode spriteNodeWithImageNamed:@"treeRight"];
    self.treeRight.anchorPoint = CGPointMake(0.5, 1);
    self.treeRight.position = CGPointMake(self.size.width * .86, self.size.height);
    self.treeRight.zPosition = LayerForeground;

    [self.worldNode addChild:self.treeRight];
}

- (void)setupRopes
{
    // get ropes data
    for (int i = 0; i < [self.ropes count]; i++) {
        TLCGameData *currentRecord = [self.ropes objectAtIndex:i];
        
        // init ropes
        TLCRope *rope = [self addRopeAtPosition:currentRecord.ropeLocation withLength:currentRecord.ropeLength withName:[NSString stringWithFormat:@"%i", i]];
        
        // add to world and setup physics
        [self.worldNode addChild:rope];
        [rope addRopePhysics];
        
        // setup the prize physics and select up a primary rope
        if (currentRecord.isPrimaryRope) {
            [self setupPrizeUsingPrimaryRope:rope];
        }
        
        // connect the other end of the rope to the prize
        [self attachNode:self.prize toRope:rope];
    }
    
    // reset prize position and set if dynamic; depends on your game play
    self.prize.position = CGPointMake(self.size.width * .50, self.size.height * .80);
    self.prize.physicsBody.dynamic = prizeIsDynamicsOnStart;
}

- (void)setupPrizeUsingPrimaryRope:(TLCRope *)rope
{
    /*
     There is a really strange known Sprite Kit bug:
     
     - When you set a physics body, if the position of the node is not set beforehand, the body will behave unpredictably.
     - The code below uses the last rope part, of a single rope (of your choosing) to use as it's inital position
     */
    
    self.prize = [SKSpriteNode spriteNodeWithImageNamed:kImageNameForPrize];
    self.prize.name = kNodeNameForPrize;
    self.prize.zPosition = LayerPrize;

    self.prize.anchorPoint = CGPointMake(0.5, 1);

    SKNode *positionOfLastNode = [[rope getRopeNodes] lastObject];
    self.prize.position = CGPointMake(positionOfLastNode.position.x, positionOfLastNode.position.y + self.prize.size.height * .30);

    CGFloat offsetX = self.prize.frame.size.width * self.prize.anchorPoint.x;
    CGFloat offsetY = self.prize.frame.size.height * self.prize.anchorPoint.y;

    CGMutablePathRef path = CGPathCreateMutable();

    CGPathMoveToPoint(path, NULL, 18 - offsetX, 75 - offsetY);
    CGPathAddLineToPoint(path, NULL, 5 - offsetX, 65 - offsetY);
    CGPathAddLineToPoint(path, NULL, 3 - offsetX, 55 - offsetY);
    CGPathAddLineToPoint(path, NULL, 4 - offsetX, 34 - offsetY);
    CGPathAddLineToPoint(path, NULL, 8 - offsetX, 7 - offsetY);
    CGPathAddLineToPoint(path, NULL, 21 - offsetX, 2 - offsetY);
    CGPathAddLineToPoint(path, NULL, 33 - offsetX, 4 - offsetY);
    CGPathAddLineToPoint(path, NULL, 38 - offsetX, 20 - offsetY);
    CGPathAddLineToPoint(path, NULL, 34 - offsetX, 53 - offsetY);
    CGPathAddLineToPoint(path, NULL, 36 - offsetX, 62 - offsetY);

    CGPathCloseSubpath(path);

    self.prize.physicsBody = [SKPhysicsBody bodyWithPolygonFromPath:path];
    self.prize.physicsBody.allowsRotation = YES;
    self.prize.physicsBody.affectedByGravity = YES;
    self.prize.physicsBody.density = 1;
    self.prize.physicsBody.dynamic = NO;

    self.prize.physicsBody.categoryBitMask = EntityCategoryPrize;
    self.prize.physicsBody.collisionBitMask = 0;
    self.prize.physicsBody.contactTestBitMask = EntityCategoryRope;

    [self.prize skt_attachDebugFrameFromPath:path color:[SKColor redColor]];
    CGPathRelease(path);
    
    [self.worldNode addChild:self.prize];
}

- (TLCRope *)addRopeAtPosition:(CGPoint)location withLength:(int)length withName:(NSString *)name
{
    SKSpriteNode *ropeHolder = [SKSpriteNode spriteNodeWithImageNamed:kImageNameForRopeHolder];
    
    ropeHolder.position = location;
    ropeHolder.zPosition = LayerRope;
    
    [self.worldNode addChild:ropeHolder];
    
    CGFloat offsetX = ropeHolder.frame.size.width * ropeHolder.anchorPoint.x;
    CGFloat offsetY = ropeHolder.frame.size.height * ropeHolder.anchorPoint.y;
    
    CGMutablePathRef path = CGPathCreateMutable();
    
    CGPathMoveToPoint(path, NULL, 0 - offsetX, 6 - offsetY);
    CGPathAddLineToPoint(path, NULL, 6 - offsetX, 6 - offsetY);
    CGPathAddLineToPoint(path, NULL, 6 - offsetX, 0 - offsetY);
    CGPathAddLineToPoint(path, NULL, 0 - offsetX, 0 - offsetY);
    
    CGPathCloseSubpath(path);
    
    ropeHolder.physicsBody = [SKPhysicsBody bodyWithPolygonFromPath:path];
    ropeHolder.physicsBody.affectedByGravity = NO;
    ropeHolder.physicsBody.dynamic = NO;
    
    ropeHolder.physicsBody.categoryBitMask = EntityCategoryRopeAttachment;
    ropeHolder.physicsBody.collisionBitMask = 0;
    ropeHolder.physicsBody.contactTestBitMask =  EntityCategoryPrize;
    
    [ropeHolder skt_attachDebugFrameFromPath:path color:[SKColor redColor]];
    CGPathRelease(path);
    
    CGPoint ropeAttachPos = CGPointMake(ropeHolder.position.x, ropeHolder.position.y -8);
    
    TLCRope *rope = [[TLCRope alloc] initWithLength:length usingAttachmentPoint:ropeAttachPos toNode:ropeHolder withName:name withDelegate:self];
    rope.zPosition = LayerRope;
    rope.name = name;
    
    return rope;
}

/* Delegate Method: not really required, just using it here to show how delegates work */
- (void)addJoint:(SKPhysicsJointPin *)joint
{
    [self.worldNode.scene.physicsWorld addJoint:joint];
}

- (void)attachNode:(SKSpriteNode *)node toRope:(TLCRope *)rope
{
    SKNode *previous = [[rope getRopeNodes] lastObject];
    node.position = CGPointMake(previous.position.x, previous.position.y + node.size.height * .40);

    SKSpriteNode *nodeAA = [[rope getRopeNodes] lastObject];

    SKPhysicsJointPin *jointB = [SKPhysicsJointPin jointWithBodyA: previous.physicsBody
                                                            bodyB: node.physicsBody
                                                           anchor: CGPointMake(CGRectGetMidX(nodeAA.frame), CGRectGetMinY(nodeAA.frame))];


    [self.worldNode.scene.physicsWorld addJoint:jointB];
}

#pragma mark - Player

- (void)setupPlayer
{
    self.player = [SKSpriteNode spriteNodeWithImageNamed:kImageNameForPlayerMouthClosed];
    self.player.anchorPoint = CGPointMake(0.5, 1);
    self.player.position = CGPointMake(self.size.width * .75, self.background.frame.origin.y + (self.player.size.height - 5));
    self.player.zPosition = LayerPlayer;

    CGFloat offsetX = self.player.frame.size.width * self.player.anchorPoint.x;
    CGFloat offsetY = self.player.frame.size.height * self.player.anchorPoint.y;

    CGMutablePathRef path = CGPathCreateMutable();
    
    /*
     CGPathMoveToPoint(path, NULL, 43 - offsetX, 72 - offsetY);
     CGPathAddLineToPoint(path, NULL, 10 - offsetX, 46 - offsetY);
     CGPathAddLineToPoint(path, NULL, 11 - offsetX, 5 - offsetY);
     CGPathAddLineToPoint(path, NULL, 73 - offsetX, 4 - offsetY);
     CGPathAddLineToPoint(path, NULL, 99 - offsetX, 22 - offsetY);
     CGPathAddLineToPoint(path, NULL, 55 - offsetX, 69 - offsetY);
     */
    
    // slightly bigger hit area
    
    CGPathMoveToPoint(path, NULL, 47 - offsetX, 77 - offsetY);
    CGPathAddLineToPoint(path, NULL, 5 - offsetX, 51 - offsetY);
    CGPathAddLineToPoint(path, NULL, 7 - offsetX, 2 - offsetY);
    CGPathAddLineToPoint(path, NULL, 78 - offsetX, 2 - offsetY);
    CGPathAddLineToPoint(path, NULL, 102 - offsetX, 21 - offsetY);

    CGPathCloseSubpath(path);

    self.player.physicsBody = [SKPhysicsBody bodyWithPolygonFromPath:path];

    self.player.physicsBody.categoryBitMask = EntityCategoryPlayer;
    self.player.physicsBody.collisionBitMask = 0;
    self.player.physicsBody.contactTestBitMask =  EntityCategoryPrize;

    self.player.physicsBody.dynamic = NO;

    [self.player skt_attachDebugFrameFromPath:path color:[SKColor redColor]];
    CGPathRelease(path);
    
    [self.worldNode addChild:self.player];

    [self animatePlayer];
}

#pragma mark - Animation and Actions for Player

- (void)animatePlayer
{
    NSMutableArray *textures = [NSMutableArray arrayWithCapacity:1];

    for (int i = 0; i <= 1; i++) {
        NSString *textureName = [NSString stringWithFormat:@"%@0%d", kImageNameForPlayerBaseImage, i];
        SKTexture *texture = [SKTexture textureWithImageNamed:textureName];
        [textures addObject:texture];
    }

    CGFloat duration = RandomFloatRange(2, 4);

    SKAction *move = [SKAction animateWithTextures:textures timePerFrame:0.25];
    SKAction *wait = [SKAction waitForDuration:duration];
    SKAction *rest = [SKAction setTexture:[textures objectAtIndex:0]];

    SKAction *animatePlayer = [SKAction sequence:@[wait, move, wait, rest]];
    [self.player runAction: [SKAction repeatActionForever:animatePlayer]];
}

- (void)nomnomnomActionWithDelay:(float)duration
{
    [self.player removeAllActions];

    SKAction *openMouth = [SKAction setTexture:[SKTexture textureWithImageNamed:kImageNameForPlayerMouthOpen]];
    SKAction *wait = [SKAction waitForDuration:duration];
    SKAction *closeMouth = [SKAction setTexture:[SKTexture textureWithImageNamed:kImageNameForPlayerMouthClosed]];

    SKAction *nomnomnomAnimation = [SKAction sequence:@[openMouth, wait, closeMouth]];

    [self.player runAction: [SKAction repeatAction:nomnomnomAnimation count:1]];

    if (!self.scoredPoint) {
        [self animatePlayer];
    }
}

#pragma mark -
#pragma mark Touch Methods

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.touchMoving = NO;

    for (UITouch *touch in touches) {
        self.touchStartPoint = [touch locationInNode:self];
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches) {
        if (touches.count == 1 && self.touchMoving) {
            self.touchEndPoint = [touch locationInNode:self];
            
            if (canCutMultipleRopesAtOnce) {
                /* allow multiple ropes to be cut */
                
                [self.worldNode.scene.physicsWorld enumerateBodiesAlongRayStart:self.touchStartPoint end:self.touchEndPoint usingBlock:^(SKPhysicsBody *body, CGPoint point, CGVector normal, BOOL *stop)
                 {
                     [self checkRopeCutWithBody:body];
                 }];
            }
            else {
                /* allow only one rope to be cut */
                
                SKPhysicsBody *body = [self.worldNode.scene.physicsWorld bodyAlongRayStart:self.touchStartPoint end:self.touchEndPoint];
                [self checkRopeCutWithBody:body];
            }
        }
    }
    
    self.touchMoving = NO;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (touches.count == 1) {
        for (UITouch *touch in touches) {
            NSString *particlePath = [[NSBundle mainBundle] pathForResource:@"TLCParticle" ofType:@"sks"];
            SKEmitterNode *emitter = [NSKeyedUnarchiver unarchiveObjectWithFile:particlePath];
            emitter.position = [touch locationInNode:self];
            emitter.zPosition = LayerRope;
            emitter.name = @"emitter";
            
            [self.worldNode addChild:emitter];
            
            self.touchMoving = YES;
        }
    }
}

#pragma mark -
#pragma mark Game Play and Update Methods

#pragma mark - Update

- (void)update:(CFTimeInterval)currentTime
{
    [self checkForScore];
    
    [self checkForGroundHit];
    
    [self checkForPrize];
}

#pragma mark - Methods for Game Play

- (void)checkForPrize
{
    [self.worldNode enumerateChildNodesWithName:kNodeNameForPrize usingBlock:^(SKNode *node, BOOL *stop)
     {
         if (node.position.y <= 0) {
             [node removeFromParent];
             self.hitGround = YES;
         }
     }];
}

- (void)checkForScore
{
    if (self.scoredPoint) {
        self.scoredPoint = NO;
        [self runAction:self.soundNomNomNomAction];
        
        SKAction *shrink = [SKAction scaleTo:0 duration:0.08];
        SKAction *removeNode = [SKAction removeFromParent];
        
        SKAction *sequence = [SKAction sequence:@[shrink, removeNode]];
        [self.prize runAction: sequence];
        
        /* Various kinds of scene transitions */
        
        // SKTransition *sceneTransition = [SKTransition doorsOpenHorizontalWithDuration:1.0];
        // SKTransition *sceneTransition = [SKTransition doorsOpenVerticalWithDuration:1.0];
        
        // SKTransition *sceneTransition = [SKTransition doorsCloseHorizontalWithDuration:1.0];
        // SKTransition *sceneTransition = [SKTransition doorsCloseVerticalWithDuration:1.0];
        
        // SKTransition *sceneTransition = [SKTransition flipHorizontalWithDuration:1.0];
        // SKTransition *sceneTransition = [SKTransition flipVerticalWithDuration:1.0];
        
        // SKTransition *sceneTransition = [SKTransition moveInWithDirection:SKTransitionDirectionLeft duration:1.0];
        // SKTransition *sceneTransition = [SKTransition pushWithDirection:SKTransitionDirectionRight duration:1.0];
        // SKTransition *sceneTransition = [SKTransition revealWithDirection:SKTransitionDirectionDown duration:1.0];
        
        // SKTransitionDirectionLeft, SKTransitionDirectionRight, SKTransitionDirectionUp, SKTransitionDirectionDown
        
        // SKTransition *sceneTransition = [SKTransition crossFadeWithDuration:1.0];
        SKTransition *sceneTransition = [SKTransition doorwayWithDuration:1.0];
        
        // SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1.0];
        // SKTransition *sceneTransition = [SKTransition fadeWithDuration:1.0];
        
        [self performSelector:@selector(switchToNewGameWithTransition:) withObject:sceneTransition afterDelay:1.0];
    }
}

- (void)checkForGroundHit
{
    if (self.hitGround) {
        self.hitGround = NO;
        [self runAction:self.soundSplashAction];
        
        SKAction *shrink = [SKAction scaleTo:0 duration:0.08];
        SKAction *removeNode = [SKAction removeFromParent];
        
        SKAction *sequence = [SKAction sequence:@[shrink, removeNode]];
        [self.prize runAction: sequence];
        
        SKTransition *sceneTransition = [SKTransition fadeWithDuration:1.0];
        [self performSelector:@selector(switchToNewGameWithTransition:) withObject:sceneTransition afterDelay:1.0];
    }
}

- (void)checkRopeCutWithBody:(SKPhysicsBody *)body
{
    SKNode *node = body.node;
    if (body) {
        self.prize.physicsBody.affectedByGravity = YES;
        self.prize.physicsBody.dynamic = YES;
        
        [self runAction:self.soundCutAction];
        [self nomnomnomActionWithDelay:1];
        
        [self.worldNode enumerateChildNodesWithName:node.name usingBlock:^(SKNode *node, BOOL *stop)
         {
             // start cheater fix
             
             if ([node.name isEqualToString:kNodeNameForPrize]) {
                 return;
             }
             
             // end cheater fix
             
             for (SKPhysicsJoint *joint in body.joints) {
                 [self.worldNode.scene.physicsWorld removeJoint:joint];
             }
             
             SKSpriteNode *ropePart = (SKSpriteNode *)node;
             
             SKAction *fadeAway = [SKAction fadeOutWithDuration:0.25];
             SKAction *removeNode = [SKAction removeFromParent];
             
             SKAction *sequence = [SKAction sequence:@[fadeAway, removeNode]];
             [ropePart runAction: sequence];
         }];
    }
}

#pragma mark - Start a New Game

- (void)switchToNewGameWithTransition:(SKTransition *)transition
{
    // [_backgroundMusicPlayer stop]; // use if using option one, from above
    
    SKView *skView = (SKView *)self.view;
    
    TLCMyScene *scene = [[TLCMyScene alloc] initWithSize:self.size];
    [skView presentScene:scene transition:transition];
}

#pragma mark - Collision Detection

- (void)didBeginContact:(SKPhysicsContact *)contact
{
    SKPhysicsBody *other = (contact.bodyA.categoryBitMask == EntityCategoryPrize ? contact.bodyB : contact.bodyA);
    if (other.categoryBitMask == EntityCategoryPlayer) {
        if (!self.hitGround) {
            NSLog(@"scoredPoint");
            self.scoredPoint = YES;
            
            [self nomnomnomActionWithDelay:.15];
        }
        
        return;
    }
    
    else if (other.categoryBitMask == EntityCategoryGround) {
        if (!self.scoredPoint) {
            NSLog(@"hitGround");
            self.hitGround = YES;
            return;
        }
    }
}

@end
