//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Assets.h"
#include "CollectibleItem.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "Enemy.h"
#include "Spirit.h"
#include "ForegroundObject.h"

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::get(Background& background)
{
	static TextureRegion upper = createTextureRegion(0, 0, PIXEL_WIDTH_FOR_BACKGROUND, 2048, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
	static TextureRegion mid = createTextureRegion(0, 1024, PIXEL_WIDTH_FOR_BACKGROUND, 1024, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
	static TextureRegion lower = createTextureRegion(0, 320, PIXEL_WIDTH_FOR_BACKGROUND, 1728, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion waterBack = createTextureRegion(0, 168, PIXEL_WIDTH_FOR_BACKGROUND, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion waterFront = createTextureRegion(0, 248, PIXEL_WIDTH_FOR_BACKGROUND, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);

    switch (background.getType())
    {
        case BackgroundType_Upper:
            initTextureRegion(upper, background.getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return upper;
        case BackgroundType_Mid:
            initTextureRegion(mid, background.getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return mid;
        case BackgroundType_Lower:
            initTextureRegion(lower, background.getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return lower;
        case BackgroundType_WaterBack:
            initTextureRegion(waterBack, background.getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return waterBack;
        case BackgroundType_WaterFront:
            initTextureRegion(waterFront, background.getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return waterFront;
    }
    
    assert(false);
}

TextureRegion& Assets::get(Midground& midground)
{
    switch (midground.getType())
    {
        case MidgroundType_TreeOne:
        {
            static TextureRegion tr = createTextureRegion(1154, 3424, 512, 672, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_TreeTwo:
        {
            static TextureRegion tr = createTextureRegion(1682, 3296, 656, 800, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_TreeThree:
        {
            static TextureRegion tr = createTextureRegion(2370, 3248, 928, 848, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnSmall:
        {
            static TextureRegion tr = createTextureRegion(0, 3808, 96, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnMedium:
        {
            static TextureRegion tr = createTextureRegion(832, 3264, 128, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case MidgroundType_DeepCaveColumnBig:
        {
            static TextureRegion tr = createTextureRegion(258, 3264, 256, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Ground& ground)
{
    switch (ground.getType())
    {
        case GroundType_CaveExtraDeepEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 774, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 774, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 774, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 774, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveExtraDeepEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 774, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveDeepEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 960, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 960, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 960, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 960, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveDeepEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 960, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1772, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1772, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1772, 1024, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1772, 2048, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1772, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_CaveRaisedEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1180, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 1180, 512, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 1180, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 1180, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_CaveRaisedEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 1180, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case GroundType_GrassWithCaveEndLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 2252, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveSmall:
        {
            static TextureRegion tr = createTextureRegion(128, 2252, 512, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveMedium:
        {
            static TextureRegion tr = createTextureRegion(640, 2252, 1024, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveLarge:
        {
            static TextureRegion tr = createTextureRegion(1664, 2252, 2048, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case GroundType_GrassWithCaveEndRight:
        {
            static TextureRegion tr = createTextureRegion(3712, 2252, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
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
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGround& exitGround)
{
    switch (exitGround.getType())
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
        {
            static TextureRegion tr = createTextureRegion(0, 860, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ExitGroundType_GrassWithCaveSmallExitEnd:
        {
            static TextureRegion tr = createTextureRegion(2512, 360, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ExitGroundType_CaveSmallExit:
        {
            static TextureRegion tr = createTextureRegion(0, 1488, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ExitGroundType_CaveDeepSmallWaterfall:
        {
            static Animation anim = createAnimation(0, 1876, 512, 448, 1536, 448, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 3);
            return anim.getTextureRegion(exitGround.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGroundCover& exitGroundCover)
{
    switch (exitGroundCover.getType())
    {
        case ExitGroundCoverType_Grass:
        {
            static Animation anim = createAnimation(1024, 860, 512, 224, 3072, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
            return anim.getTextureRegion(exitGroundCover.getStateTime());
        }
        case ExitGroundCoverType_Cave:
        {
            static Animation anim = createAnimation(514, 1488, 512, 384, 2560, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 5);
            return anim.getTextureRegion(exitGroundCover.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Hole& hole)
{
    switch (hole.getType())
    {
        case HoleType_Grass:
        {
            static TextureRegion tr = createTextureRegion(0, 0, 256, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case HoleType_Cave:
        {
            static TextureRegion tr = createTextureRegion(0, 1100, 272, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(HoleCover& holeCover)
{
    switch (holeCover.getType())
    {
        case HoleCoverType_Grass:
        {
            static Animation anim = createAnimation(256, 0, 256, 256, 3072, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 12);
            return anim.getTextureRegion(holeCover.getStateTime());
        }
        case HoleCoverType_Cave:
        {
            static Animation anim = createAnimation(272, 1100, 272, 384, 2720, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(holeCover.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ForegroundObject& foregroundObject)
{
    switch (foregroundObject.getType())
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
            static TextureRegion tr = createTextureRegion(0, 3136, 384, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_RockSmallCracked:
        {
            static Animation anim = createAnimation(384, 3136, 384, 368, 2304, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
            return anim.getTextureRegion(foregroundObject.getStateTime());
        }
            
        case ForegroundObjectType_StumpBig:
        {
            static TextureRegion tr = createTextureRegion(0, 1096, 256, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_StumpSmall:
        {
            static TextureRegion tr = createTextureRegion(0, 1406, 320, 272, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_EndSign:
        {
            static TextureRegion tr = createTextureRegion(0, 1712, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_ThornsLeft:
        {
            static TextureRegion tr = createTextureRegion(0, 1814, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsCenterSmall:
        {
            static TextureRegion tr = createTextureRegion(432, 1814, 176, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsCenterBig:
        {
            static TextureRegion tr = createTextureRegion(80, 1814, 352, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_ThornsRight:
        {
            static TextureRegion tr = createTextureRegion(608, 1814, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_LogVerticalTall:
        {
            static TextureRegion tr = createTextureRegion(0, 1942, 128, 176, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_LogVerticalShort:
        {
            static TextureRegion tr = createTextureRegion(0, 2128, 128, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_JumpSpringLight:
        {
            static Animation anim = createAnimation(96, 3584, 96, 80, 480, 80, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 4);
            return anim.getTextureRegion(foregroundObject.getStateTime());
        }
        case ForegroundObjectType_JumpSpringMedium:
        {
            static Animation anim = createAnimation(0, 3668, 272, 144, 1904, 144, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 7);
            return anim.getTextureRegion(foregroundObject.getStateTime());
        }
        case ForegroundObjectType_JumpSpringHeavy:
        {
            static Animation anim = createAnimation(272, 3816, 272, 224, 1904, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
            return anim.getTextureRegion(foregroundObject.getStateTime());
        }
            
        case ForegroundObjectType_SpikeGrassSingle:
        {
            static TextureRegion tr = createTextureRegion(0, 2244, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeGrassFour:
        {
            static TextureRegion tr = createTextureRegion(96, 2244, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeGrassEight:
        {
            static TextureRegion tr = createTextureRegion(384, 2244, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveSingle:
        {
            static TextureRegion tr = createTextureRegion(0, 2452, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveFour:
        {
            static TextureRegion tr = createTextureRegion(96, 2452, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveEight:
        {
            static TextureRegion tr = createTextureRegion(384, 2452, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
        {
            static TextureRegion tr = createTextureRegion(0, 2348, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingFour:
        {
            static TextureRegion tr = createTextureRegion(96, 2348, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingEight:
        {
            static TextureRegion tr = createTextureRegion(384, 2348, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
            
        case ForegroundObjectType_SpikeWallSingle:
        {
            static TextureRegion tr = createTextureRegion(4000, 0, 96, 64, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeWallFour:
        {
            static TextureRegion tr = createTextureRegion(4000, 704, 96, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
        case ForegroundObjectType_SpikeWallEight:
        {
            static TextureRegion tr = createTextureRegion(4000, 128, 96, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Enemy& enemy)
{
    switch (enemy.getType())
    {
        case EnemyType_Mushroom:
        {
            static Animation anim = createAnimation(0, 0, 128, 128, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 4);
            return anim.getTextureRegion(enemy.getStateTime());
        }
        case EnemyType_MushroomCeiling:
        {
            static Animation anim = createAnimation(0, 200, 128, 128, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 4);
            return anim.getTextureRegion(enemy.getStateTime());
        }
        case EnemyType_SnakeGrunt:
        {
            static Animation anim = createAnimation(0, 332, 128, 96, 512, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 4);
            return anim.getTextureRegion(enemy.getStateTime());
        }
        case EnemyType_Sparrow:
        {
            static Animation anim = createAnimation(0, 692, 160, 160, 1120, 160, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 7);
            return anim.getTextureRegion(enemy.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Spirit& spirit)
{
    switch (spirit.getType())
    {
        case SpiritType_Snake:
        {
            static Animation anim = createAnimation(0, 432, 208, 256, 2080, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
            return anim.getTextureRegion(spirit.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(CollectibleItem& collectibleItem)
{
    switch (collectibleItem.getType())
    {
        case CollectibleItemType_Carrot:
        {
            static Animation anim = createAnimation(0, 2692, 96, 112, 960, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 10);
            return anim.getTextureRegion(collectibleItem.getStateTime());
        }
        case CollectibleItemType_GoldenCarrot:
        {
            static Animation anim = createAnimation(0, 2560, 96, 128, 864, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 9);
            return anim.getTextureRegion(collectibleItem.getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Jon& jon)
{
    if (jon.isTransformingIntoVampire())
    {
        static Animation transformingIntoVampire = createAnimation(2048, 2048, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        if (!transformingIntoVampire.hasFrameTimes())
        {
            transformingIntoVampire.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return transformingIntoVampire.getTextureRegion(jon.getTransformStateTime());
    }
    else if (jon.isRevertingToRabbit())
    {
        static Animation revertingToRabbit = createAnimation(2048, 2816, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        if (!revertingToRabbit.hasFrameTimes())
        {
            revertingToRabbit.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return revertingToRabbit.getTextureRegion(jon.getTransformStateTime());
    }
    
    if (jon.isVampire())
    {
        static Animation deathAnim = createAnimation(0, 2048, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06666666666667f, 15);
        static Animation pushedBackAnim = createAnimation(512, 256, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 6);
        static Animation idleAnim = createAnimation(0, 1792, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.125f, 8);
        static Animation runningAnim = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 10);
        static Animation jumpingAnim = createAnimation(768, 512, 256, 512, 1280, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
        static Animation doubleJumpingAnim = createAnimation(0, 512, 256, 256, 768, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 6);
        static Animation glidingAnim = createAnimation(0, 1024, 256, 256, 512, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 2);
        static Animation fallingAnim = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.066f, 3);
        
        if (jon.getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon.getDyingStateTime());
        }
        
        switch (jon.getAbilityState())
        {
            case ABILITY_GLIDE:
                return glidingAnim.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon.getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon.isLanding())
            {
                return landingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon.isFalling())
            {
                return fallingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        
        if (jon.isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon.getStateTime());
            
            if (keyFrameNumber == 1 && !jon.isRightFoot())
            {
                jon.setRightFoot(true);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon.isRightFoot())
            {
                jon.setRightFoot(false);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon.isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon.getStateTime());
        }
        
        return idleAnim.getTextureRegion(jon.getStateTime());
    }
    else
    {
        static Animation deathAnim = createAnimation(0, 3072, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 10);
        static Animation warmUpAnim = createAnimation(0, 2048, 256, 256, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 28);
        static Animation pushedBackAnim = createAnimation(512, 1024, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 6);
        static Animation idleAnim = createAnimation(0, 1792, 256, 256, 1024, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.25f, 4);
        static Animation runningAnim = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.07f, 10);
        static Animation jumpingAnim = createAnimation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 7);
        static Animation doubleJumpingAnim = createAnimation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 9);
        static Animation fallingAnim = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 4);
        static Animation spinningBackFistAnimation = createAnimation(2048, 0, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 7);
        static Animation burrowAnimation = createAnimation(2048, 256, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 5, 1);
        
        if (jon.getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon.getDyingStateTime());
        }
        
        switch (jon.getAbilityState())
        {
            case ABILITY_SPINNING_BACK_FIST:
                return spinningBackFistAnimation.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_BURROW:
                return burrowAnimation.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon.getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon.isLanding())
            {
                return landingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon.isFalling())
            {
                return fallingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        
        if (jon.isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon.getStateTime());
            
            if (keyFrameNumber == 1 && !jon.isRightFoot())
            {
                jon.setRightFoot(true);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon.isRightFoot())
            {
                jon.setRightFoot(false);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon.isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon.getStateTime());
        }
        
        return warmUpAnim.getTextureRegion(jon.getStateTime());
    }
}

TextureRegion& Assets::get(DustCloud& dustCloud)
{
    static Animation anim = createAnimation(1100, 1643, 115, 60, 575, 60, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.08f, 5);
    
    return anim.getTextureRegion(dustCloud.getStateTime());
}

TextureRegion& Assets::get(BackButton& backButton)
{
    static TextureRegion tr = createTextureRegion(1824, 48, 189, 61, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

TextureRegion& Assets::get(LevelEditorButton& levelEditorButton)
{
    static TextureRegion tr = createTextureRegion(1824, 119, 189, 61, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

TextureRegion& Assets::get(LevelEditorEntitiesPanel& levelEditorEntitiesPanel)
{
    static TextureRegion tr = TextureRegion(0, 2093, 593, 1338, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

TextureRegion& Assets::get(LevelEditorActionsPanel& levelEditorActionsPanel)
{
    static TextureRegion tr = TextureRegion(840, 2093, 593, 1338, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

TextureRegion& Assets::get(TrashCan& trashCan)
{
    static TextureRegion trashCanTr = TextureRegion(0, 3502, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    static TextureRegion trashCanHighlightedTr = TextureRegion(128, 3502, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    
    return trashCan.isHighlighted() ? trashCanHighlightedTr : trashCanTr;
}

TextureRegion& Assets::get(LevelSelectorPanel& levelSelectorPanel)
{
    static TextureRegion tr = TextureRegion(1538, 2060, 510, 510, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    return tr;
}

short Assets::getFirstSoundId()
{
    return m_sSoundIds.size() > 0 ? m_sSoundIds.front() : 0;
}

void Assets::addSoundIdToPlayQueue(short soundId)
{
    if (m_sSoundIds.size() < MAX_SOUNDS_TO_PLAY_PER_FRAME)
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