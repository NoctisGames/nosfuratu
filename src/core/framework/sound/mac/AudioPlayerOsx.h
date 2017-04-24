//
//  AudioPlayerOsx.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 4/24/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef AUDIOPLAYEROSX_H
#define AUDIOPLAYEROSX_H

#include "AudioPlayer.h"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

class AudioPlayerOsx : public AudioPlayer
{
public:
    ~AudioPlayerOsx();
    static const int kNumberBuffers = 3;                              // 1
    struct AQPlayerState {
        AudioStreamBasicDescription   mDataFormat;                    // 2
        AudioQueueRef                 mQueue;                         // 3
        AudioQueueBufferRef           mBuffers[kNumberBuffers];       // 4
        AudioFileID                   mAudioFile;                     // 5
        UInt32                        bufferByteSize;                 // 6
        SInt64                        mCurrentPacket;                 // 7
        UInt32                        mNumPacketsToRead;              // 8
        AudioStreamPacketDescription  *mPacketDescs;                  // 9
        bool                          mIsRunning;                     // 10
        AudioPlayerOsx* m_player;
    };
    static void HandleOutputBuffer (
        void                *aqData,
        AudioQueueRef       inAQ,
        AudioQueueBufferRef inBuffer
    );
    bool load(CFURLRef url);

public:
    //Super();
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;
    double duration() const;
    double progress() const;
    bool play();
    void stop();
    void pause();//todo
    void seek(double sec);
    static AudioPlayerOsx* file(const char *fn);

    void setVolume(int);
    int getVolume() const;
    void mute();
    void unmute();
    bool isMuted();

    void setBalance(int LR);
    int getBalance() const;

    const static int FILETYPE_COUNT;
    const static char* FILETYPES[];
    
    bool m_isLooping;
    
    void setLooping(bool isLooping) { m_isLooping = isLooping; }

private:
    AudioPlayerOsx() : volume_level(100), _isMuted(false), m_isLooping(false) {}
    void primeBuffer();
    void seekToPacket(uint64_t packet);
    static void checkStatus_(OSStatus status, const char* file, int line);

    static void deriveBufferSize(
            AudioStreamBasicDescription &ASBDesc,                            // 1
            UInt32                      maxPacketSize,                       // 2
            Float64                     seconds,                             // 3
            UInt32                      *outBufferSize,                      // 4
            UInt32                      *outNumPacketsToRead                 // 5
        );
    double timeBase;
    AQPlayerState aqData;
    bool paoosay, _isMuted;
    int volume_level;
};

#endif // AUDIOPLAYEROSX_H
