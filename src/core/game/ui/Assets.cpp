//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Assets.h"
#include "CollectibleItem.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "Enemy.h"
#include "EnemySpirit.h"
#include "ForegroundObject.h"
#include "TitlePanel.h"
#include "WorldMapPanel.h"
#include "GameScreenWorldMap.h"
#include "Game.h"

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::get(TitlePanel* panel)
{
    if (panel->isLightningStriking())
    {
        static Animation anim = Animation(0, 0, 1280, 720, 3840, 1448, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 6, 0, 8);
        return anim.getTextureRegion(panel->getStateTime());
    }
    else
    {
        static Animation anim = Animation(0, 1456, 1280, 720, 2560, 720, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
        return anim.getTextureRegion(panel->getStateTime());
    }
}

TextureRegion& Assets::get(WorldMapPanel* panel)
{
    static TextureRegion tr = TextureRegion(0, 192, 1024, 576, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LevelThumbnail* thumbnail)
{
    if (thumbnail->isCompleted())
    {
        static TextureRegion tr = TextureRegion(68, 772, 68, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
        return tr;
    }
    else
    {
        static TextureRegion tr = TextureRegion(0, 772, 68, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
        return tr;
    }
}

TextureRegion& Assets::get(Background* background)
{
	static TextureRegion upper = createTextureRegion(0, 0, PIXEL_WIDTH_FOR_BACKGROUND, 2048, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
	static TextureRegion mid = createTextureRegion(0, 1024, PIXEL_WIDTH_FOR_BACKGROUND, 1024, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
	static TextureRegion lower = createTextureRegion(0, 320, PIXEL_WIDTH_FOR_BACKGROUND, 1728, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion waterBack = createTextureRegion(0, 168, PIXEL_WIDTH_FOR_BACKGROUND, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion waterFront = createTextureRegion(0, 248, PIXEL_WIDTH_FOR_BACKGROUND, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);

    switch (background->getType())
    {
        case BackgroundType_Upper:
            initTextureRegion(upper, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return upper;
        case BackgroundType_Mid:
            initTextureRegion(mid, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return mid;
        case BackgroundType_Lower:
            initTextureRegion(lower, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return lower;
        case BackgroundType_WaterBack:
            initTextureRegion(waterBack, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return waterBack;
        case BackgroundType_WaterFront:
            initTextureRegion(waterFront, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return waterFront;
    }
    
    assert(false);
}

TextureRegion& Assets::get(Midground* midground)
{
    switch (midground->getType())
    {
        case MidgroundType_TreeOne:
        {
            static TextureRegion tr = createTextureRegion(1940, 224, 512, 672, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_TreeTwo:
        {
            static TextureRegion tr = createTextureRegion(2468, 96, 656, 800, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_TreeThree:
        {
            static TextureRegion tr = createTextureRegion(3156, 48, 928, 848, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnSmall:
        {
            static TextureRegion tr = createTextureRegion(3052, 1444, 96, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnMedium:
        {
            static TextureRegion tr = createTextureRegion(3884, 900, 128, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnBig:
        {
            static TextureRegion tr = createTextureRegion(3310, 900, 256, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Ground* ground)
{
    switch (ground->getType())
    {
        case GroundType_CaveExtraDeepEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1132, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1132, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1132, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1132, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1132, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveDeepEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1264, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1264, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1264, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1264, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1264, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1912, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1912, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1912, 1024, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1912, 2048, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1912, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveRaisedEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1396, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1396, 512, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1396, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1396, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1396, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_GrassWithCaveEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 2300, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 2300, 512, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 2300, 1024, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 2300, 2048, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 2300, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_GrassWithoutCaveEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 2496, 128, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithoutCaveSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 2496, 512, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithoutCaveMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 2496, 1024, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithoutCaveLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 2496, 2048, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithoutCaveEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 2496, 128, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_GrassPitSmall:
        {
            static TextureRegion tr = createTextureRegion(2162, 880, 640, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassPitMedium:
        {
            static TextureRegion tr = createTextureRegion(2898, 880, 1168, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassPitLarge:
        {
            static TextureRegion tr = createTextureRegion(0, 880, 2048, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassPitExtraLarge:
        {
            static TextureRegion tr = createTextureRegion(0, 2496, 4096, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGround* exitGround)
{
    switch (exitGround->getType())
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
        {
            static TextureRegion tr = createTextureRegion(0, 0, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ExitGroundType_GrassWithCaveSmallExitEnd:
        {
            static TextureRegion tr = createTextureRegion(3500, 228, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ExitGroundType_CaveSmallExit:
        {
            static TextureRegion tr = createTextureRegion(0, 228, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ExitGroundType_CaveDeepSmallWaterfall:
        {
            static Animation anim = createAnimation(0, 616, 512, 448, 1536, 448, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 3);
            return anim.getTextureRegion(exitGround->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGroundCover* exitGroundCover)
{
    switch (exitGroundCover->getType())
    {
        case ExitGroundCoverType_Grass:
        {
            static Animation anim = createAnimation(1024, 0, 512, 224, 3072, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.08333333333333f, 6);
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
        case ExitGroundCoverType_Cave:
        {
            static Animation anim = createAnimation(514, 228, 512, 384, 2560, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 5);
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Hole* hole)
{
    switch (hole->getType())
    {
        case HoleType_GrassTileLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1400, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case HoleType_GrassTileCenter:
        {
            static TextureRegion tr = createTextureRegion(0, 1772, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case HoleType_GrassTileRight:
        {
            static TextureRegion tr = createTextureRegion(0, 2144, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case HoleType_Grass:
        {
            static TextureRegion tr = createTextureRegion(0, 2528, 256, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case HoleType_Cave:
        {
            static TextureRegion tr = createTextureRegion(0, 2788, 272, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(HoleCover* holeCover)
{
    switch (holeCover->getType())
    {
        case HoleCoverType_GrassTileLeft:
        {
            static Animation anim = createAnimation(256, 1400, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileCenter:
        {
            static Animation anim = createAnimation(256, 1772, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileRight:
        {
            static Animation anim = createAnimation(256, 2144, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Grass:
        {
            static Animation anim = createAnimation(256, 2528, 256, 256, 3072, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04166666666667f, 12);
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Cave:
        {
            static Animation anim = createAnimation(272, 2788, 272, 384, 2720, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(holeCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ForegroundObject* foregroundObject)
{
    switch (foregroundObject->getType())
    {
        case ForegroundObjectType_GrassPlatformLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_GrassPlatformCenter:
        {
            static TextureRegion tr = createTextureRegion(64, 0, 224, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_GrassPlatformRight:
        {
            static TextureRegion tr = createTextureRegion(288, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_CavePlatformLeft:
        {
            static TextureRegion tr = createTextureRegion(378, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_CavePlatformCenter:
        {
            static TextureRegion tr = createTextureRegion(442, 0, 224, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_CavePlatformRight:
        {
            static TextureRegion tr = createTextureRegion(666, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_RockLarge:
        {
            static TextureRegion tr = createTextureRegion(0, 130, 704, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_RockMedium:
        {
            static TextureRegion tr = createTextureRegion(0, 686, 384, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_RockSmall:
        {
            static TextureRegion tr = createTextureRegion(0, 3236, 384, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_RockSmallCracked:
        {
            static Animation anim = createAnimation(384, 3236, 384, 368, 2304, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_StumpBig:
        {
            static TextureRegion tr = createTextureRegion(388, 696, 256, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_StumpSmall:
        {
            static TextureRegion tr = createTextureRegion(0, 1080, 320, 272, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_EndSign:
        {
            static TextureRegion tr = createTextureRegion(700, 686, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_ThornsLeft:
        {
            static TextureRegion tr = createTextureRegion(1156, 1196, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsCenterSmall:
        {
            static TextureRegion tr = createTextureRegion(1588, 1196, 176, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsCenterBig:
        {
            static TextureRegion tr = createTextureRegion(1236, 1196, 352, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsRight:
        {
            static TextureRegion tr = createTextureRegion(1764, 1196, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_LogVerticalTall:
        {
            static TextureRegion tr = createTextureRegion(452, 1004, 128, 176, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_LogVerticalShort:
        {
            static TextureRegion tr = createTextureRegion(452, 1190, 128, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_JumpSpringLight:
        {
            static Animation anim = createAnimation(2126, 3960, 96, 80, 480, 80, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 4);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringMedium:
        {
            static Animation anim = createAnimation(0, 3668, 272, 144, 1904, 144, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05142857142857, 7);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringHeavy:
        {
            static Animation anim = createAnimation(272, 3816, 272, 224, 1904, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 6);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_SpikeGrassSingle:
        {
            static TextureRegion tr = createTextureRegion(1904, 900, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeGrassFour:
        {
            static TextureRegion tr = createTextureRegion(2000, 900, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeGrassEight:
        {
            static TextureRegion tr = createTextureRegion(2288, 900, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveSingle:
        {
            static TextureRegion tr = createTextureRegion(1904, 1108, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveFour:
        {
            static TextureRegion tr = createTextureRegion(2000, 1108, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveEight:
        {
            static TextureRegion tr = createTextureRegion(2288, 1108, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
        {
            static TextureRegion tr = createTextureRegion(1904, 1004, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingFour:
        {
            static TextureRegion tr = createTextureRegion(2000, 1004, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingEight:
        {
            static TextureRegion tr = createTextureRegion(2288, 1004, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeWallSingle:
        {
            static TextureRegion tr = createTextureRegion(4000, 3140, 96, 64, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeWallFour:
        {
            static TextureRegion tr = createTextureRegion(4000, 3824, 96, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeWallEight:
        {
            static TextureRegion tr = createTextureRegion(4000, 3248, 96, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeStar:
        {
            static TextureRegion tr = createTextureRegion(890, 1162, 224, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_VerticalSaw:
        {
            static Animation anim = createAnimation(3590, 2556, 240, 528, 480, 528, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 2);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_GiantTree:
        {
            static TextureRegion tr = createTextureRegion(832, 0, 1088, 1040, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_GiantShakingTree:
        {
            static Animation anim = createAnimation(832, 0, 1088, 1040, 3264, 3120, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 7);
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_GiantPerchTree:
        {
            static TextureRegion tr = createTextureRegion(3008, 2080, 1088, 1040, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeTower:
        {
            static TextureRegion tr = createTextureRegion(2892, 3200, 512, 896, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeTowerBg:
        {
            static TextureRegion tr = createTextureRegion(3408, 3200, 512, 896, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(CountHissWithMina* countHissWithMina)
{
    static Animation anim = createAnimation(2048, 3072, 512, 512, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 7);
    return anim.getTextureRegion(countHissWithMina->getStateTime());
}

TextureRegion& Assets::get(Enemy* enemy)
{
    switch (enemy->getType())
    {
        case EnemyType_MushroomGround:
        {
            static Animation anim = createAnimation(0, 0, 112, 128, 448, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.12f, 4);
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_MushroomCeiling:
        {
            static Animation anim = createAnimation(0, 132, 112, 128, 448, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.12f, 4);
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_SnakeGrunt:
        {
            static Animation anim = createAnimation(0, 332, 128, 96, 512, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 4);
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_Sparrow:
        {
            static Animation anim = createAnimation(0, 692, 160, 160, 1120, 160, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 7);
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_Toad:
        {
            Toad* toad = dynamic_cast<Toad *>(enemy);
            
            if (toad->isDead())
            {
                if (toad->isDeadPart1())
                {
                    if (toad->isJonVampire())
                    {
                        // Vampire Part 1
                        static Animation anim = createAnimation(1024, 2144, 512, 256, 1024, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 2);
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                    else
                    {
                        // Rabbit Part 1
                        static Animation anim = createAnimation(0, 1888, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 5);
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                }
                else
                {
                    // Part 2
                    static Animation anim = createAnimation(2048, 1888, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 7);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (toad->isEating())
            {
                if (toad->isJonVampire())
                {
                    static Animation anim = createAnimation(2048, 1372, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 8);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = createAnimation(0, 1372, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 8);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else
            {
                static Animation anim = createAnimation(492, 0, 512, 256, 3584, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.15f, 7);
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_Fox:
        {
            Fox* fox = dynamic_cast<Fox *>(enemy);
            
            if (fox->isDead())
            {
                static Animation anim = createAnimation(0, 2664, 256, 512, 2560, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (fox->isHitting())
            {
                if (fox->isLeft())
                {
                    static Animation anim = createAnimation(2080, 260, 256, 256, 1792, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 11);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = createAnimation(0, 2404, 256, 256, 2816, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 11);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (fox->isBeingHit())
            {
                static Animation anim = createAnimation(3072, 2404, 256, 256, 768, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                if (fox->isLeft())
                {
                    static Animation anim = createAnimation(3104, 516, 256, 256, 768, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.15f, 3);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = createAnimation(3104, 772, 256, 256, 768, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.15f, 3);
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(EnemySpirit* spirit)
{
    switch (spirit->getType())
    {
        case EnemySpiritType_Snake:
        {
            static Animation anim = createAnimation(0, 432, 208, 256, 2080, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_Sparrow:
        {
            static Animation anim = createAnimation(0, 856, 256, 512, 3072, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04166666666667f, 12);
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_None:
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& Assets::get(CollectibleItem* collectibleItem)
{
    switch (collectibleItem->getType())
    {
        case CollectibleItemType_Carrot:
        {
            static Animation anim = createAnimation(884, 1054, 96, 112, 960, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 10);
            return anim.getTextureRegion(collectibleItem->getStateTime());
        }
        case CollectibleItemType_GoldenCarrot:
        {
            static Animation anim = createAnimation(1904, 1216, 96, 128, 864, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 9);
            return anim.getTextureRegion(collectibleItem->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Jon* jon)
{
    if (jon->isTransformingIntoVampire())
    {
        static Animation transformingIntoVampire = createAnimation(2048, 2048, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        if (!transformingIntoVampire.hasFrameTimes())
        {
            transformingIntoVampire.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return transformingIntoVampire.getTextureRegion(jon->getTransformStateTime());
    }
    else if (jon->isRevertingToRabbit())
    {
        static Animation revertingToRabbit = createAnimation(2048, 2816, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        if (!revertingToRabbit.hasFrameTimes())
        {
            revertingToRabbit.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return revertingToRabbit.getTextureRegion(jon->getTransformStateTime());
    }
    
    if (jon->isVampire())
    {
        static Animation deathAnim = createAnimation(0, 2048, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06666666666667f, 15);
        static Animation pushedBackAnim = createAnimation(512, 256, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 6);
        static Animation idleAnim = createAnimation(0, 1792, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.125f, 8);
        static Animation runningAnim = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.056f, 10);
        static Animation jumpingAnim = createAnimation(768, 512, 256, 512, 1280, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
        static Animation doubleJumpingAnim = createAnimation(0, 512, 256, 256, 768, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 6);
        static Animation glidingAnim = createAnimation(0, 1024, 256, 256, 512, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 2);
        static Animation fallingAnim = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.066f, 3);
        
        if (jon->getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon->getDyingStateTime());
        }
        
        switch (jon->getAbilityState())
        {
            case ABILITY_GLIDE:
                return glidingAnim.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon->getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon->getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon->isLanding())
            {
                return landingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        else if (jon->getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon->isFalling())
            {
                return fallingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        
        if (jon->isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon->getStateTime());
            
            if (keyFrameNumber == 1 && !jon->isRightFoot())
            {
                jon->setRightFoot(true);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon->isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon->getStateTime());
        }
        
        return idleAnim.getTextureRegion(jon->getStateTime());
    }
    else
    {
        static Animation deathAnim = createAnimation(0, 3072, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 10);
        static Animation warmUpAnim = createAnimation(0, 2048, 256, 256, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 28);
        static Animation pushedBackAnim = createAnimation(512, 1024, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 6);
        static Animation idleAnim = createAnimation(0, 1792, 256, 256, 1024, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.25f, 4);
        static Animation runningAnim = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.064f, 10);
        static Animation jumpingAnim = createAnimation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 7);
        static Animation doubleJumpingAnim = createAnimation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 9);
        static Animation fallingAnim = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 4);
        static Animation spinningBackFistAnimation = createAnimation(2048, 0, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 7);
        static Animation burrowAnimation = createAnimation(2048, 256, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 5, 1);
        static Animation stompAnimation = createAnimation(2048, 1024, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 10, 8);
        static Animation landingRollAnimation = createAnimation(2816, 1280, 256, 256, 1280, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04f, 5);
        
        if (jon->getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon->getDyingStateTime());
        }
        
        switch (jon->getAbilityState())
        {
            case ABILITY_SPINNING_BACK_FIST:
                return spinningBackFistAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_BURROW:
                return burrowAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_STOMP:
                return stompAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon->getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon->getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon->isLanding())
            {
                if (jon->isRollLanding())
                {
                    return landingRollAnimation.getTextureRegion(jon->getStateTime());
                }
                
                return landingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        else if (jon->getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon->isFalling())
            {
                return fallingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        
        if (jon->isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon->getStateTime());
            
            if (keyFrameNumber == 1 && !jon->isRightFoot())
            {
                jon->setRightFoot(true);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon->isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon->getStateTime());
        }
        
        return warmUpAnim.getTextureRegion(jon->getStateTime());
    }
}

TextureRegion& Assets::get(DustCloud* dustCloud)
{
    static Animation anim = createAnimation(1100, 1644, 116, 60, 580, 60, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.08f, 5);
    
    return anim.getTextureRegion(dustCloud->getStateTime());
}

TextureRegion& Assets::get(MidBossOwl* owl)
{
    switch (owl->getState())
    {
        case MidBossOwlState_Sleeping:
        {
            static Animation anim = createAnimation(0, 0, 702, 572, 702, 2288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 4);
            
            return anim.getTextureRegion(owl->getStateTime());
        }
            break;
        case MidBossOwlState_Awakening:
        {
            static Animation anim = createAnimation(0, 3300, 702, 572, 1404, 572, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 2);
            
            return anim.getTextureRegion(owl->getStateTime());
        }
            break;
        case MidBossOwlState_Screeching:
        {
            static Animation anim = createAnimation(1404, 3300, 702, 572, 1404, 572, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
            
            return anim.getTextureRegion(owl->getStateTime());
        }
            break;
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& Assets::get(BackButton* backButton)
{
    static TextureRegion tr = TextureRegion(800, 748, 190, 62, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);
    return tr;
}

TextureRegion& Assets::get(LevelEditorButton* levelEditorButton)
{
    static TextureRegion tr = TextureRegion(2848, 2644, 190, 62, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

TextureRegion& Assets::get(LevelEditorEntitiesPanel* levelEditorEntitiesPanel)
{
    static TextureRegion tr = TextureRegion(0, 46, 592, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LevelEditorActionsPanel* levelEditorActionsPanel)
{
    static TextureRegion tr = TextureRegion(840, 46, 592, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(TrashCan* trashCan)
{
    static TextureRegion trashCanTr = TextureRegion(0, 1454, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion trashCanHighlightedTr = TextureRegion(128, 1454, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    return trashCan->isHighlighted() ? trashCanHighlightedTr : trashCanTr;
}

TextureRegion& Assets::get(LevelSelectorPanel* panel)
{
    static TextureRegion tr = TextureRegion(1538, 12, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(OffsetPanel* panel)
{
    static TextureRegion tr = TextureRegion(1538, 542, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(ConfirmResetPanel* panel)
{
    static TextureRegion tr = TextureRegion(938, 1484, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(ConfirmExitPanel* panel)
{
    static TextureRegion tr = TextureRegion(1538, 1072, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

short Assets::getFirstSoundId()
{
    return m_sSoundIds.size() > 0 ? m_sSoundIds.front() : 0;
}

void Assets::addSoundIdToPlayQueue(short soundId)
{
    if (soundId > NO_SOUND && m_sSoundIds.size() < MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::forceAddSoundIdToPlayQueue(short soundId)
{
    if (soundId > NO_SOUND)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::eraseFirstSoundId()
{
    if (m_sSoundIds.size() > 0)
    {
        m_sSoundIds.erase(m_sSoundIds.begin());
    }
}

void Assets::setMusicId(short musicId)
{
    m_sMusicId = musicId;
}

short Assets::getMusicId()
{
    return m_sMusicId;
}

bool Assets::isUsingCompressedTextureSet()
{
    return m_isUsingCompressedTextureSet;
}

void Assets::setUsingCompressedTextureSet(bool isUsingCompressedTextureSet)
{
    m_isUsingCompressedTextureSet = isUsingCompressedTextureSet;
}

Animation Assets::createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames)
{
    if (m_isUsingCompressedTextureSet)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        animationWidth /= 2.0;
        animationHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
    }
    
    return Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, numFrames);
}

Animation Assets::createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame)
{
    if (m_isUsingCompressedTextureSet)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        animationWidth /= 2.0;
        animationHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
    }
    
    return Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, frameTime, numFrames, firstLoopingFrame);
}

TextureRegion Assets::createTextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
{
    if (m_isUsingCompressedTextureSet)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
    }
    
    return TextureRegion(x, y, regionWidth, regionHeight, textureWidth, textureHeight);
}

void Assets::initTextureRegion(TextureRegion& tr, int x, int regionWidth, int textureWidth)
{
	if (m_isUsingCompressedTextureSet)
	{
		x /= 2.0;
		regionWidth /= 2.0;
		textureWidth /= 2.0;
	}

	tr.init(x, regionWidth, textureWidth);
}

Assets::Assets() : m_sMusicId(0), m_isUsingCompressedTextureSet(false)
{
    // Hide Constructor for Singleton
}