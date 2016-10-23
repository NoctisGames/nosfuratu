//
//  AppleSoundManager.m
//  nosfuratu
//
//  Created by Stephen Gowen on 10/23/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#import "AppleSoundManager.h"

#ifdef NG_IOS
#import "SuperpoweredIOSAudioIO.h"
#elif defined NG_MAC
#import "SuperpoweredOSXAudioIO.h"
#endif

#include "SuperpoweredSoundManager.h"

@interface AppleSoundManager ()
{
    SuperpoweredSoundManager* _superpoweredSoundManager;
    NSMutableArray *_audioSystems;
    int _soundIndexCounter;
}

@end

@implementation AppleSoundManager

#ifdef NG_IOS
static bool audioProcessingMusic(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processMusic(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound1(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound1(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound2(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound2(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound3(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound3(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound4(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound4(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound5(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound5(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound6(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound6(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound7(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound7(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound8(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound8(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound9(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound9(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound10(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound10(buffers, (unsigned int)numberOfSamples);
}

static bool audioProcessingSound11(void *clientData, float **buffers, unsigned int inputChannels, unsigned int outputChannels, unsigned int numberOfSamples, unsigned int samplerate, uint64_t hostTime)
{
    return ((SuperpoweredSoundManager *)clientData)->processSound11(buffers, (unsigned int)numberOfSamples);
}
#endif

- (instancetype)init
{
    self = [super init];
    
    if (self)
    {
        int sampleRate = 44100;
        _superpoweredSoundManager = new SuperpoweredSoundManager(sampleRate);
        
        _audioSystems = [[NSMutableArray alloc] init];
        
#ifdef NG_IOS
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingMusic clientdata:_superpoweredSoundManager]];
         
         [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound1 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound2 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound3 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound4 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound5 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound6 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound7 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound8 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound9 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound10 clientdata:_superpoweredSoundManager]];
        [_audioSystems addObject:[[SuperpoweredIOSAudioIO alloc] initWithDelegate:(id<SuperpoweredIOSAudioIODelegate>)self preferredBufferSize:12 preferredMinimumSamplerate:sampleRate audioSessionCategory:AVAudioSessionCategoryPlayback channels:2 audioProcessingCallback:audioProcessingSound11 clientdata:_superpoweredSoundManager]];
        
        for (SuperpoweredIOSAudioIO *audioSystem in _audioSystems)
        {
            [audioSystem start];
        }
#elif defined NG_MAC
        [_audioSystems addObject:[[SuperpoweredOSXAudioIO alloc] initWithDelegate:(id<SuperpoweredOSXAudioIODelegate>)self preferredBufferSizeMs:12 numberOfChannels:2 enableInput:true enableOutput:true]];
        
        for (SuperpoweredOSXAudioIO *audioSystem in _audioSystems)
        {
            [audioSystem start];
        }
#endif
        
        _soundIndexCounter = 0;
    }
    
    return self;
}

- (void)loadSound:(NSString *)path
{
    const char *bundlePath = [[[NSBundle mainBundle] pathForResource:path ofType:@"mp3"] fileSystemRepresentation];
    
    _superpoweredSoundManager->loadSound(_soundIndexCounter++, bundlePath);
}

- (void)playSound:(int)rawResourceId volume:(float)volume isLooping:(bool)isLooping
{
    _superpoweredSoundManager->playSound(rawResourceId, volume, isLooping);
}

- (void)stopSound:(int)rawResourceId
{
    _superpoweredSoundManager->stopSound(rawResourceId);
}

- (void)loadAndPlayMusic:(NSString *)path volume:(float)volume isLooping:(bool)isLooping
{
    const char *bundlePath = [[[NSBundle mainBundle] pathForResource:path ofType:@"mp3"] fileSystemRepresentation];
    
    _superpoweredSoundManager->loadMusic(1337, bundlePath);
    _superpoweredSoundManager->playMusic(1337, volume, isLooping);
}

- (void)setMusicVolume:(float)volume
{
    _superpoweredSoundManager->setMusicVolume(volume);
}

- (void)resumeMusic
{
    _superpoweredSoundManager->resumeMusic();
}

- (void)pauseMusic
{
    _superpoweredSoundManager->pauseMusic();
}

#ifdef NG_IOS
- (void)interruptionStarted {}
- (void)interruptionEnded {}
- (void)recordPermissionRefused {}
- (void)mapChannels:(multiOutputChannelMap *)outputMap inputMap:(multiInputChannelMap *)inputMap externalAudioDeviceName:(NSString *)externalAudioDeviceName outputsAndInputs:(NSString *)outputsAndInputs {}
#elif defined NG_MAC
- (bool)audioProcessingCallback:(float **)inputBuffers inputChannels:(unsigned int)inputChannels outputBuffers:(float **)outputBuffers outputChannels:(unsigned int)outputChannels numberOfSamples:(unsigned int)numberOfSamples samplerate:(unsigned int)samplerate hostTime:(UInt64)hostTime
{
    return _superpoweredSoundManager->processMusic(inputBuffers, (unsigned int)numberOfSamples);
}
#endif

@end
