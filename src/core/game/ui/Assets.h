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
#include "CaveSkeleton.h"
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
#include "BackgroundSky.h"
#include "BackgroundTrees.h"
#include "BackgroundCave.h"
#include "BackButton.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"

#include <vector>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

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
    
    static TextureRegion& get(Hole& hole);
    
    static TextureRegion& get(HoleCover& holeCover);
    
    static TextureRegion& get(CaveExit& caveExit);
    
    static TextureRegion& get(CaveExitCover& caveExitCover);
    
    static TextureRegion& get(LogVerticalTall& logVerticalTall);
    
    static TextureRegion& get(LogVerticalShort& logVerticalShort);
    
    static TextureRegion& get(Thorns& thorns);
    
    static TextureRegion& get(Stump& stump);
    
    static TextureRegion& get(SideSpike& sideSpike);
    
    static TextureRegion& get(UpwardSpike& upwardSpike);
    
    static TextureRegion& get(JumpSpring& jumpSpring);
    
    static TextureRegion& get(Rock& rock);
    
    static TextureRegion& get(CrackedRock& rock);
    
    static TextureRegion& get(GroundPlatform& platform);
    
    static TextureRegion& get(EndSign& endSign);
    
    static TextureRegion& get(Carrot& carrot);
    
    static TextureRegion& get(GoldenCarrot& goldenCarrot);
    
    static TextureRegion& get(Jon& jon);
    
    static TextureRegion& get(DustCloud& dustCloud);
    
    static TextureRegion& get(BackButton& backButton);
    
    static TextureRegion& get(LevelEditorEntitiesPanel& levelEditorEntitiesPanel);
    
    static TextureRegion& get(LevelEditorActionsPanel& levelEditorActionsPanel);
    
    static TextureRegion& get(TrashCan& trashCan);
    
    static TextureRegion& get(LevelSelectorPanel& levelSelectorPanel);
    
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