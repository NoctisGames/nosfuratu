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
class Ground;
class UpwardSpike;
class JumpSpring;
class LogVerticalTall;
class Rock;
class GroundPlatform;
class Carrot;
class GoldenCarrot;
class Game;

class Assets
{
public:
	static Assets * getInstance();
    
    static TextureRegion& getBackgroundSky(Jon& jon, Game& game);
    
    static TextureRegion& getBackgroundTrees(Jon& jon, Game& game);
    
    static TextureRegion& getBackgroundCave(Jon& jon, Game& game);
    
    static TextureRegion& getMidgroundTree(Tree& tree);
    
    static TextureRegion& getGround(Ground& ground);
    
    static TextureRegion& getLogVerticalTall();
    
    static TextureRegion& getLogVerticalShort();
    
    static TextureRegion& getThorns();
    
    static TextureRegion& getStump();
    
    static TextureRegion& getSideSpike();
    
    static TextureRegion& getUpwardSpike(UpwardSpike& upwardSpike);
    
    static TextureRegion& getJumpSpring(JumpSpring& jumpSpring);
    
    static TextureRegion& getRock(Rock& rock);
    
    static TextureRegion& getGroundPlatform(GroundPlatform& platform);
    
    static TextureRegion& getEndSign();
    
    static TextureRegion& getCarrot(Carrot& carrot);
    
    static TextureRegion& getGoldenCarrot(GoldenCarrot& goldenCarrot);
    
    static TextureRegion& getDustCloud(DustCloud& dustCloud);
    
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