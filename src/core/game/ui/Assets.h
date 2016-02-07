//
//  Assets.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Assets__
#define __nosfuratu__Assets__

#include "TextureRegion.h"
#include "Animation.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"
#include "Tree.h"
#include "DustCloud.h"
#include "Jon.h"
#include "GroundPlatform.h"
#include "Carrot.h"
#include "Ground.h"
#include "Hole.h"
#include "HoleCover.h"
#include "CaveExit.h"
#include "CaveExitCover.h"
#include "UpwardSpike.h"
#include "LogVerticalTall.h"
#include "Rock.h"
#include "Game.h"
#include "Background.h"
#include "BackButton.h"
#include "LevelEditorButton.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"
#include "SnakeGrunt.h"
#include "SnakeHorned.h"
#include "SnakeSpirit.h"

#include <vector>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

class Assets
{
public:
	static Assets * getInstance();
    
    TextureRegion& get(Background& background);
    
    TextureRegion& get(Tree& tree);
    
    TextureRegion& get(Ground& ground);
    
    TextureRegion& get(Hole& hole);
    
    TextureRegion& get(HoleCover& holeCover);
    
    TextureRegion& get(CaveExit& caveExit);
    
    TextureRegion& get(CaveExitCover& caveExitCover);
    
    TextureRegion& get(LogVerticalTall& logVerticalTall);
    
    TextureRegion& get(LogVerticalShort& logVerticalShort);
    
    TextureRegion& get(Thorns& thorns);
    
    TextureRegion& get(Stump& stump);
    
    TextureRegion& get(SideSpike& sideSpike);
    
    TextureRegion& get(UpwardSpike& upwardSpike);
    
    TextureRegion& get(JumpSpring& jumpSpring);
    
    TextureRegion& get(Rock& rock);
    
    TextureRegion& get(CrackedRock& rock);
    
    TextureRegion& get(GroundPlatform& platform);
    
    TextureRegion& get(EndSign& endSign);
    
    TextureRegion& get(Carrot& carrot);
    
    TextureRegion& get(GoldenCarrot& goldenCarrot);
    
    TextureRegion& get(SnakeGrunt& snakeEnemy);
    
    TextureRegion& get(SnakeHorned& snakeEnemy);
    
    TextureRegion& get(SnakeSpirit& snakeSpirit);
    
    TextureRegion& get(Jon& jon);
    
    TextureRegion& get(DustCloud& dustCloud);
    
    TextureRegion& get(BackButton& backButton);
    
    TextureRegion& get(LevelEditorButton& levelEditorButton);
    
    TextureRegion& get(LevelEditorEntitiesPanel& levelEditorEntitiesPanel);
    
    TextureRegion& get(LevelEditorActionsPanel& levelEditorActionsPanel);
    
    TextureRegion& get(TrashCan& trashCan);
    
    TextureRegion& get(LevelSelectorPanel& levelSelectorPanel);
    
    short getFirstSoundId();
    
    void addSoundIdToPlayQueue(short soundId);
    
    void eraseFirstSoundId();
    
    void setMusicId(short musicId);
    
    short getMusicId();
    
    bool isUsingCompressedTextureSet();
    
    void setUsingCompressedTextureSet(bool isUsingCompressedTextureSet);

private:
    std::vector<short> m_sSoundIds;
    short m_sMusicId;
    bool m_isUsingCompressedTextureSet;
    
    Animation createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames);
    
    Animation createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame = 0);
    
    TextureRegion createTextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);

	void initTextureRegion(TextureRegion& tr, int x, int regionWidth, int textureWidth);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Assets();
    Assets(const Assets&);
    Assets& operator=(const Assets&);
};

#endif /* defined(__nosfuratu__Assets__) */