//
//  Assets.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Assets__
#define __nosfuratu__Assets__

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

#include <vector>

class TextureRegion;
class Jon;
class GamePlatform;
class Carrot;
class Ground;

class Assets
{
public:
	static Assets * getInstance();
    
    static TextureRegion& getSky(Jon& jon);
    
    static TextureRegion& getBackgroundTrees(Jon& jon);
    
    static TextureRegion& getMidgroundTrees(Jon& jon);
    
    static TextureRegion& getForeground(Ground& ground);
    
    static TextureRegion& getPlatform(GamePlatform& platform);
    
    static TextureRegion& getCarrot(Carrot& carrot);
    
    static TextureRegion& getJon(Jon& jon);
    
    void setMusicId(short musicId);
    
    short getMusicId();
    
    short getFirstSoundId();
    
    void addSoundIdToPlayQueue(short soundId);
    
    void eraseFirstSoundId();

private:
    std::vector<short> m_sSoundIds;
    short m_sMusicId;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Assets();
    Assets(const Assets&);
    Assets& operator=(const Assets&);
};

#endif /* defined(__nosfuratu__Assets__) */