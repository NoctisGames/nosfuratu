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
class DustCloud;
class Tree;
class CaveSkeleton;
class Ground;
class UpwardSpike;
class JumpSpring;
class LogVerticalTall;
class LogVerticalShort;
class Thorns;
class Stump;
class SideSpike;
class Rock;
class GroundPlatform;
class EndSign;
class Carrot;
class GoldenCarrot;
class Game;
class BackgroundSky;
class BackgroundTrees;
class BackgroundCave;

class Assets
{
public:
	static Assets * getInstance();
    
    static TextureRegion& get(BackgroundSky& backgroundSky);
    
    static TextureRegion& get(BackgroundTrees& backgroundTrees);
    
    static TextureRegion& get(BackgroundCave& backgroundCave);
    
    static TextureRegion& get(Tree& tree);
    
    static TextureRegion& get(CaveSkeleton& caveSkeleton);
    
    static TextureRegion& get(Ground& ground);
    
    static TextureRegion& get(LogVerticalTall& logVerticalTall);
    
    static TextureRegion& get(LogVerticalShort& logVerticalShort);
    
    static TextureRegion& get(Thorns& thorns);
    
    static TextureRegion& get(Stump& stump);
    
    static TextureRegion& get(SideSpike& sideSpike);
    
    static TextureRegion& get(UpwardSpike& upwardSpike);
    
    static TextureRegion& get(JumpSpring& jumpSpring);
    
    static TextureRegion& get(Rock& rock);
    
    static TextureRegion& get(GroundPlatform& platform);
    
    static TextureRegion& get(EndSign& endSign);
    
    static TextureRegion& get(Carrot& carrot);
    
    static TextureRegion& get(GoldenCarrot& goldenCarrot);
    
    static TextureRegion& get(DustCloud& dustCloud);
    
    static TextureRegion& get(Jon& jon);
    
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