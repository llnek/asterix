//
//  Scene03.m
//  TheSeasons
//
//  Created by Tammy Coron on 9/13/13.
//  Copyright (c) 2013 Tammy Coron. All rights reserved.
//

#import "Scene03.h"

#import "Scene00.h"
#import "Scene02.h"
#import "Scene04.h"

#import "SKTUtils.h"
                                                                   
@import AVFoundation;

@implementation Scene03
{
    AVAudioPlayer *_backgroundMusicPlayer;
    SKSpriteNode *_btnSound;
    BOOL _soundOff;
    
    SKSpriteNode *_footer;
    SKSpriteNode *_btnLeft;
    SKSpriteNode *_btnRight;
}

#pragma mark -
#pragma mark Scene Setup and Initialize

-(id)initWithSize:(CGSize)size
{
    if (self = [super initWithSize:size])
    {
        /* set up your scene here */
        
        /* set up Sound */
        
        _soundOff = [[NSUserDefaults standardUserDefaults] boolForKey:@"pref_sound"];
        [self playBackgroundMusic:@"pg03_bgMusic.mp3"];
        
        /* add background Image */
        
        SKSpriteNode *background = [SKSpriteNode spriteNodeWithImageNamed:@"bg_pg03"];
        background.anchorPoint = CGPointZero;
        background.position = CGPointZero;
        
        [self addChild:background];
        
        /* additional setup */
        
        [self setUpText];
        [self setUpFooter];
        
        [self setUpMainScene];
        
    }
    return self;
}

- (void)willMoveFromView:(SKView *)view
{
    
}

#pragma mark -
#pragma mark Standard Scene Setup

- (void)setUpText
{
    /* add the story text */
    
    SKSpriteNode *text = [SKSpriteNode spriteNodeWithImageNamed:@"pg03_text"];
    text.position = CGPointMake(690 , 585);
    
    [self addChild:text];
    
    [self readText];
}

- (void)readText
{
    if (![self actionForKey:@"readText"])
    {
        SKAction *readPause = [SKAction waitForDuration:0.25];
        SKAction *readText = [SKAction playSoundFileNamed:@"pg03.wav" waitForCompletion:YES];
        
        SKAction *readSequence = [SKAction sequence:@[readPause, readText]];
        
        [self runAction:readSequence withKey:@"readText"];
    }
    else
    {
        [self removeActionForKey:@"readText"];
    }
}

- (void)setUpFooter
{
    /* add the footer */
    
    _footer = [SKSpriteNode spriteNodeWithImageNamed:@"footer"];
    _footer.position = CGPointMake(self.size.width/2, 38);
    
    [self addChild:_footer];
    
     self.physicsBody = [SKPhysicsBody bodyWithEdgeLoopFromRect:_footer.frame];
    
    /* add the right button */
    
    _btnRight = [SKSpriteNode spriteNodeWithImageNamed:@"button_right"];
    _btnRight.position = CGPointMake(self.size.width/2 + 470, 38);
    
    [self addChild:_btnRight];
    
    /* add the left button */
    
    _btnLeft = [SKSpriteNode spriteNodeWithImageNamed:@"button_left"];
    _btnLeft.position = CGPointMake(self.size.width/2 + 400, 38);
    
    [self addChild:_btnLeft];
    
    /* add the sound button */
    
    if (_soundOff)
    {
        // NSLog(@"_soundOff");
        
        [_btnSound removeFromParent];
        
        _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_off"];
        _btnSound.position = CGPointMake(self.size.width/2 + 330, 38);
        
        [self addChild:_btnSound];
        [_backgroundMusicPlayer stop];
    }
    else
    {
        // NSLog(@"_soundOn");
        
        [_btnSound removeFromParent];
        
        _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_on"];
        _btnSound.position = CGPointMake(self.size.width/2 + 330, 38);
        
        [self addChild:_btnSound];
        [_backgroundMusicPlayer play];
    }
}

#pragma mark -
#pragma mark Additional Scene Setup (sprites and such)

- (void)setUpMainScene
{
  /* add the kid and the cat */
  
  SKSpriteNode *cat = [SKSpriteNode spriteNodeWithImageNamed:@"pg03_kid_cat"];
  cat.anchorPoint = CGPointZero;
  cat.position = CGPointMake(self.size.width/2 - 25, 84);
  
  [self addChild:cat];
  
  /* add Snowflakes */
  
  float duration = 1.25f; // determines how often to create snowflakes
  [self runAction:[SKAction repeatActionForever:[SKAction sequence:@[[SKAction performSelector:@selector(spawnSnowflake) onTarget:self],[SKAction waitForDuration:duration]]]]];
}

- (void)spawnSnowflake
{
  // here you can even add physics and a shake motion too
  
  CGFloat randomNumber = RandomFloatRange(1, 4);
  CGFloat duration = RandomFloatRange(5, 21);
  
  NSString *snowflakeImageName = [NSString stringWithFormat:@"snowfall0%.0f",randomNumber];
  SKSpriteNode *snowflake = [SKSpriteNode spriteNodeWithImageNamed:snowflakeImageName];
  snowflake.name = @"snowflake";
  
  snowflake.position = CGPointMake(RandomFloatRange(0, self.size.height), self.size.height + self.size.height/2);
  [self addChild:snowflake];
  
  SKAction *actionMove = [SKAction moveTo:CGPointMake(snowflake.position.x + 100, -snowflake.size.height/2) duration:duration];
  
  SKAction *actionRemove = [SKAction removeFromParent];
  [snowflake runAction:[SKAction sequence:@[actionMove, actionRemove]]];
}

- (void)checkCollisions
{
  [self enumerateChildNodesWithName:@"snowflake" usingBlock:^(SKNode *node, BOOL *stop)
  {
    SKSpriteNode *snowflake = (SKSpriteNode *)node;
    CGRect footerFrameWithPadding = CGRectInset(_footer.frame, -25, -25); // set padding around foot frame
    
    if (CGRectIntersectsRect(snowflake.frame, footerFrameWithPadding))
    {
      [snowflake removeFromParent];
    }
  }];
}

#pragma mark -
#pragma mark Code For Sound & Ambiance

- (void)playBackgroundMusic:(NSString *)filename
{
    NSError *error;
    NSURL *backgroundMusicURL = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
    _backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
    _backgroundMusicPlayer.numberOfLoops = -1;
    _backgroundMusicPlayer.volume = .50;
    [_backgroundMusicPlayer prepareToPlay];
}

- (void)showSoundButtonForTogglePosition:(BOOL )togglePosition
{
    // NSLog(@"togglePosition: %i", togglePosition);
    
    if (togglePosition)
    {
        _btnSound.texture = [SKTexture textureWithImageNamed:@"button_sound_on"];
        
        _soundOff = NO;
        [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"pref_sound"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        
        [_backgroundMusicPlayer play];
    }
    else
    {
        _btnSound.texture = [SKTexture textureWithImageNamed:@"button_sound_off"];
        
        _soundOff = YES;
        [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"pref_sound"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        
        [_backgroundMusicPlayer stop];
    }
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    /* Called when a touch begins */
    for (UITouch *touch in touches)
    {
        CGPoint location = [touch locationInNode:self];
      
      
        if([_btnSound containsPoint:location])
        {
            // NSLog(@"xxxxxxxxxxxxxxxxxxx sound toggle");
        
            [self showSoundButtonForTogglePosition:_soundOff];
        }
        else if ([_btnRight containsPoint:location])
        {
            // NSLog(@">>>>>>>>>>>>>>>>> page forward");
        
            if (![self actionForKey:@"readText"]) // do not turn page if reading
            {
                [_backgroundMusicPlayer stop];
          
                SKScene *scene = [[Scene04 alloc] initWithSize:self.size];
                SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
                [self.view presentScene:scene transition:sceneTransition];
            }
        }
        else if ([_btnLeft containsPoint:location])
        {
            // NSLog(@"<<<<<<<<<<<<<<<<<< page backward");
        
            if (![self actionForKey:@"readText"]) // do not turn page if reading
            {
                [_backgroundMusicPlayer stop];
          
                SKScene *scene = [[Scene02 alloc] initWithSize:self.size];
                SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
                [self.view presentScene:scene transition:sceneTransition];
            }
        }
        else if ( location.x >= 29 && location.x <= 285 && location.y >= 6 && location.y <= 68 )
        {
            // NSLog(@">>>>>>>>>>>>>>>>> page title");
        
            if (![self actionForKey:@"readText"]) // do not turn page if reading
            {
                [_backgroundMusicPlayer stop];
          
                SKScene *scene = [[Scene00 alloc] initWithSize:self.size];
                SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
                [self.view presentScene:scene transition:sceneTransition];
            }
        }
    }
}

#pragma mark -
#pragma mark Game Loop

-(void)update:(CFTimeInterval)currentTime
{
  [self checkCollisions];  
}

@end
