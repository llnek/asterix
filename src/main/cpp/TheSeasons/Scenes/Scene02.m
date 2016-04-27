//
//  Scene02.m
//  TheSeasons
//
//  Created by Tammy Coron on 9/13/13.
//  Copyright (c) 2013 Tammy Coron. All rights reserved.
//

#import "Scene02.h"

#import "Scene00.h"
#import "Scene01.h"
#import "Scene03.h"

@import AVFoundation;

@implementation Scene02
{
    AVAudioPlayer *_backgroundMusicPlayer;
    SKSpriteNode *_btnSound;
    BOOL _soundOff;
    
    SKSpriteNode *_footer;
    SKSpriteNode *_btnLeft;
    SKSpriteNode *_btnRight;
    
    SKSpriteNode *_cat;
    SKAction *_catSound;
}

#pragma mark -
#pragma mark Scene Setup and Initialize

-(id)initWithSize:(CGSize)size
{
    if (self = [super initWithSize:size])
    {
        /* set up your scene here */
        
        /* set up sound */
        
        _soundOff = [[NSUserDefaults standardUserDefaults] boolForKey:@"pref_sound"];
        [self playBackgroundMusic:@"pg02_bgMusic.mp3"];
        
        _catSound = [SKAction playSoundFileNamed:@"cameronmusic_meow.wav" waitForCompletion:NO];
        
        /* add background image */
        
        SKSpriteNode *background = [SKSpriteNode spriteNodeWithImageNamed:@"bg_pg02"];
        background.anchorPoint = CGPointZero;
        background.position = CGPointZero;
        
        [self addChild:background];
        
        /* additional setup */
        
        [self setUpText];
        [self setUpFooter];
        
        /* add the cat (you don't always need a seperate method) */
        
        _cat = [SKSpriteNode spriteNodeWithImageNamed:@"pg02_cat"];
        _cat.anchorPoint = CGPointZero;
        _cat.position = CGPointMake(240, 84);
        
        [self addChild:_cat];
        
        /* make 'em meow! */
        
        SKAction *wait = [SKAction waitForDuration:.86];
        SKAction *catSoundInitial = [SKAction playSoundFileNamed:@"thegertz_meow.wav" waitForCompletion:NO];
        
        [_cat runAction:[SKAction sequence:@[wait, catSoundInitial]]];
    }
    return self;
}

- (void)willMoveFromView:(SKView *)view
{
    /* you can also stop the bg music here */
    
    // [_backgroundMusicPlayer stop];
}

#pragma mark -
#pragma mark Standard Scene Setup

- (void)setUpText
{
    /* add the story text */
    
    SKSpriteNode *text = [SKSpriteNode spriteNodeWithImageNamed:@"pg02_text"];
    text.position = CGPointMake(680 , 530);
    
    [self addChild:text];
    
    [self readText];
}

- (void)readText
{
    if (![self actionForKey:@"readText"])
    {
        SKAction *readPause = [SKAction waitForDuration:0.25];
        SKAction *readText = [SKAction playSoundFileNamed:@"pg02.wav" waitForCompletion:YES];
        
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
#pragma mark Code For Sound & Ambiance

- (void)playCatSound
{
    [_cat runAction:_catSound];
}

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
      
      
        if ( location.x >= 300 && location.x < 480 && location.y >= 100 && location.y <= 350 )
        {
          // NSLog(@"xxxxxxxxxxxxxxxxxxx touched cat");
          
          [self playCatSound];
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
          
            SKScene *scene = [[Scene03 alloc] initWithSize:self.size];
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
          
            SKScene *scene = [[Scene01 alloc] initWithSize:self.size];
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
    /* Called before each frame is rendered */
}

@end
