//
//  Scene01.m
//  TheSeasons
//
//  Created by Tammy Coron on 9/13/13.
//  Copyright (c) 2013 Tammy Coron. All rights reserved.
//

#import "Scene01.h"
#import "Scene00.h"
#import "Scene02.h"
#import "SKTUtils.h"
@import AVFoundation;

@interface Scene01 ()


@end

@implementation Scene01
{
    AVAudioPlayer *_backgroundMusicPlayer;
    SKSpriteNode *_btnSound;
    BOOL _soundOff;
  
    SKSpriteNode *_footer;
    SKSpriteNode *_btnLeft;
    SKSpriteNode *_btnRight;

    SKSpriteNode *_kid;
    SKSpriteNode *_hat;

    BOOL _touchingHat;
    CGPoint _touchPoint;
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
        [self playBackgroundMusic:@"pg01_bgMusic.mp3"];
        
        /* add background image */
        
        SKSpriteNode *background = [SKSpriteNode spriteNodeWithImageNamed:@"bg_pg01"];
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
  SKSpriteNode *text = [SKSpriteNode spriteNodeWithImageNamed:@"pg01_text"];
  text.position = CGPointMake(300 , 530);
  
  [self addChild:text];
  
  [self readText];
}

- (void)readText
{
  if (![self actionForKey:@"readText"])
  {
    SKAction *readPause = [SKAction waitForDuration:0.25];
    SKAction *readText = [SKAction playSoundFileNamed:@"pg01.wav" waitForCompletion:YES];
    
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
  [self setUpMainCharacter];
  [self setUpHat];
}

- (void)setUpMainCharacter
{
  _kid = [SKSpriteNode spriteNodeWithImageNamed:@"pg01_kid"];
  _kid.anchorPoint = CGPointZero;
  _kid.position = CGPointMake(self.size.width/2 - 245, 45);
  
  [self addChild:_kid];
  [self setUpMainCharacterAnimation];
}

- (void)setUpMainCharacterAnimation
{
  NSMutableArray *textures = [NSMutableArray arrayWithCapacity:2];
  
  for (int i = 0; i <= 2; i++)
  {
    NSString *textureName = [NSString stringWithFormat:@"pg01_kid0%d", i];
    SKTexture *texture = [SKTexture textureWithImageNamed:textureName];
    [textures addObject:texture];
  }
  
  CGFloat duration = RandomFloatRange(3, 6);
  
  SKAction *blink = [SKAction animateWithTextures:textures timePerFrame:0.25];
  SKAction *wait = [SKAction waitForDuration:duration];
  
  SKAction *mainCharacterAnimation = [SKAction sequence:@[blink, wait, blink, blink, wait , blink, blink]];
  [_kid runAction: [SKAction repeatActionForever:mainCharacterAnimation]];
}

- (void)setUpHat
{
  SKLabelNode *label = [SKLabelNode labelNodeWithFontNamed:@"Thonburi-Bold"];
  label.text = @"Help Mikey put on his hat!";
  label.fontSize = 20.0;
  label.fontColor = [UIColor yellowColor];
  label.position = CGPointMake(160, 180);
  
  [self addChild:label];
  
  _hat = [SKSpriteNode spriteNodeWithImageNamed:@"pg01_kid_hat"];
  _hat.position = CGPointMake(150, 290);
  _hat.physicsBody = [SKPhysicsBody bodyWithRectangleOfSize:_hat.size];
  _hat.physicsBody.restitution = 0.5;
  
  [self addChild:_hat];
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

#pragma mark -
#pragma mark Touch Events

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    /* Called when a touch begins */
    for (UITouch *touch in touches)
    {
        CGPoint location = [touch locationInNode:self];
        // NSLog(@"** TOUCH LOCATION ** \nx: %f / y: %f", location.x, location.y);
    
      
        if([_hat containsPoint:location])
        {
            // NSLog(@"xxxxxxxxxxxxxxxxxxx touched hat");
            _touchingHat = YES;
            _touchPoint = location;
        
            /* change the physics or the hat is too 'heavy' */
        
            _hat.physicsBody.velocity = CGVectorMake(0, 0);
            _hat.physicsBody.angularVelocity = 0;
            _hat.physicsBody.affectedByGravity = NO;
        }
        else if([_btnSound containsPoint:location])
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
            
                SKScene *scene = [[Scene02 alloc] initWithSize:self.size];
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
            
                SKScene *scene = [[Scene00 alloc] initWithSize:self.size];
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

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  _touchPoint = [[touches anyObject] locationInNode:self];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  if (_touchingHat)
  {
    CGPoint currentPoint = [[touches anyObject] locationInNode:self];
    
    if ( currentPoint.x >= 300 && currentPoint.x <= 550 &&
        currentPoint.y >= 250 && currentPoint.y <= 400 )
    {
      // NSLog(@"Close Enough! Let me do it for you");
      
      currentPoint.x = 420;
      currentPoint.y = 330;
      
      _hat.position = currentPoint;
      
      SKAction *popSound = [SKAction playSoundFileNamed:@"thompsonman_pop.wav" waitForCompletion:NO];
      [_hat runAction:popSound];
    }
    else
      _hat.physicsBody.affectedByGravity = YES;
    
    _touchingHat = NO;
  }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  _touchingHat = NO;
  _hat.physicsBody.affectedByGravity = YES;
}

#pragma mark -
#pragma mark Game Loop

-(void)update:(CFTimeInterval)currentTime
{
  if (_touchingHat)
  {
    _touchPoint.x = Clamp(_touchPoint.x, _hat.size.width / 2, self.size.width - _hat.size.width / 2);
    _touchPoint.y = Clamp(_touchPoint.y,
                          _footer.size.height +  _hat.size.height / 2,
                          self.size.height - _hat.size.height / 2);
    
    _hat.position = _touchPoint;
  }
}

@end
