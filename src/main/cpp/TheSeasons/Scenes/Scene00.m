//
//  Scene00.m
//  TheSeasons
//
//  Created by Tammy Coron on 9/15/13.
//  Copyright (c) 2013 Tammy Coron. All rights reserved.
//

#import "Scene00.h"
#import "Scene01.h"
@import AVFoundation;

@implementation Scene00
{
    /* set up your instance variables here */
    AVAudioPlayer *_backgroundMusicPlayer;
    SKSpriteNode *_btnSound;
    BOOL _soundOff;
}

#pragma mark -
#pragma mark Scene Setup and Initialize

-(id)initWithSize:(CGSize)size
{
    if (self = [super initWithSize:size])
    {
      /* add setup here */

      _soundOff = [[NSUserDefaults standardUserDefaults] boolForKey:@"pref_sound"];
      [self playBackgroundMusic:@"title_bgMusic.mp3"];
      
      SKSpriteNode *background = [SKSpriteNode spriteNodeWithImageNamed:@"bg_title_page"];
      background.anchorPoint = CGPointZero;
      background.position = CGPointZero;
      
      [self addChild:background];
      
      [self setUpBookTitle];
      [self setUpSoundButton];
    }
    
    return self;
}

- (void)willMoveFromView:(SKView *)view
{
    
}

#pragma mark -
#pragma mark Additional Scene Setup (sprites and such)

- (void)setUpBookTitle
{
  SKSpriteNode *bookTitle = [SKSpriteNode spriteNodeWithImageNamed:@"title_text"];
  bookTitle.name = @"bookTitle";
  
  bookTitle.position = CGPointMake(425,900);
  [self addChild:bookTitle];
  
  SKAction *actionMoveDown = [SKAction moveToY:600 duration:3.0];
  SKAction *actionMoveUp = [SKAction moveToY:603 duration:0.25];
  SKAction *actionMoveDownFast = [SKAction moveToY:600 duration:0.25];
  
  SKAction *wait = [SKAction waitForDuration:0.75];
  SKAction *showButton = [SKAction runBlock:^{
    
    SKSpriteNode *buttonStart = [SKSpriteNode spriteNodeWithImageNamed:@"button_read"];
    buttonStart.name = @"buttonStart";
    
    buttonStart.position = CGPointMake(425,460);
    [self addChild:buttonStart];
    
    [buttonStart runAction:[SKAction playSoundFileNamed:@"thompsonman_pop.wav" waitForCompletion:NO]];
  }];

  [bookTitle runAction:[SKAction sequence:@[actionMoveDown, actionMoveUp, actionMoveDownFast, wait, showButton]]];
}

- (void)setUpSoundButton
{
  if (_soundOff)
  {
    // NSLog(@"_soundOff");
    
    [_btnSound removeFromParent];
    
    _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_off"];
    _btnSound.position = CGPointMake(980, 38);
    
    [self addChild:_btnSound];
    [_backgroundMusicPlayer stop];
  }
  else
  {
    // NSLog(@"_soundOn");
    
    [_btnSound removeFromParent];
    
    _btnSound = [SKSpriteNode spriteNodeWithImageNamed:@"button_sound_on"];
    _btnSound.position = CGPointMake(980, 38);
    
    [self addChild:_btnSound];
    [_backgroundMusicPlayer play];
  }
}

#pragma mark -
#pragma mark Code For Sound & Ambiance

- (void)playBackgroundMusic:(NSString *)filename
{
  NSError *error;
  NSURL *backgroundMusicURL = [[NSBundle mainBundle] URLForResource:filename withExtension:nil];
  _backgroundMusicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
  _backgroundMusicPlayer.numberOfLoops = -1;
  _backgroundMusicPlayer.volume = 1.0;
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
  SKNode *startButton = [self childNodeWithName:@"buttonStart"];
  
  /* Called when a touch begins */
  for (UITouch *touch in touches)
  {
    CGPoint location = [touch locationInNode:self];
    // NSLog(@"** TOUCH LOCATION ** \nx: %f / y: %f", location.x, location.y);
    
    if([_btnSound containsPoint:location])
    {
      // NSLog(@"xxxxxxxxxxxxxxxxxxx sound toggle");
      
      [self showSoundButtonForTogglePosition:_soundOff];
    }
    else if([startButton containsPoint:location])
    {
      [_backgroundMusicPlayer stop];
      
      // NSLog(@"xxxxxxxxxxxxxxxxxxx touched read button");
      
      Scene01 *scene = [[Scene01 alloc] initWithSize:self.size];
      SKTransition *sceneTransition = [SKTransition fadeWithColor:[UIColor darkGrayColor] duration:1];
      [self.view presentScene:scene transition:sceneTransition];
    }
  }
}

#pragma mark -
#pragma mark Game Loop

-(void)update:(CFTimeInterval)currentTime
{
    
}

@end
