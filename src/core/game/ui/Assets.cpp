//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Assets.h"
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
#include "UpwardSpike.h"
#include "LogVerticalTall.h"
#include "Rock.h"
#include "GroundType.h"
#include "Game.h"
#include "BackgroundSky.h"
#include "BackgroundTrees.h"
#include "BackgroundCave.h"

#define TEXTURE_SIZE_2048 2048
#define PIXEL_WIDTH_FOR_GAME 684.0f
#define PIXEL_HEIGHT_FOR_GAME 1154.0f

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::get(BackgroundSky& backgroundSky)
{
    static TextureRegion tr = TextureRegion(0, 0, PIXEL_WIDTH_FOR_GAME, 769, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundSky.getX(), backgroundSky.getY(), PIXEL_WIDTH_FOR_GAME, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundTrees& backgroundTrees)
{
    static TextureRegion tr = TextureRegion(0, 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundTrees.getX(), 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundCave& backgroundCave)
{
    static TextureRegion tr = TextureRegion(0, 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundCave.getX(), 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Tree& tree)
{
    static TextureRegion type1 = TextureRegion(0, 628, 270, 346, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion type2 = TextureRegion(274, 628, 480, 439, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion type3 = TextureRegion(758, 628, 339, 411, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (tree.getTreeType())
    {
        case TREE_ONE:
            return type1;
        case TREE_TWO:
            return type2;
        case TREE_THREE:
        default:
            return type3;
    }
}

TextureRegion& Assets::get(CaveSkeleton& caveSkeleton)
{
    static TextureRegion type1 = TextureRegion(1100, 628, 125, 76, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion type2 = TextureRegion(1228, 628, 131, 102, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion type3 = TextureRegion(1362, 628, 131, 102, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (caveSkeleton.getCaveSkeletonType())
    {
        case CAVE_SKELETON_ONE:
            return type1;
        case CAVE_SKELETON_TWO:
            return type2;
        case CAVE_SKELETON_THREE:
        default:
            return type3;
    }
}

TextureRegion& Assets::get(Ground& ground)
{
    static TextureRegion GROUND_GRASS_WITH_CAVE_LARGE = TextureRegion(0, 0, 2048, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_LARGE = TextureRegion(0, 116, 2048, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITHOUT_CAVE_LARGE = TextureRegion(0, 322, 2048, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_GRASS_WITHOUT_CAVE_END_LEFT = TextureRegion(0, 763, 65, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITHOUT_CAVE_MEDIUM = TextureRegion(65, 763, 1025, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITHOUT_CAVE_SMALL = TextureRegion(1090, 763, 512, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITHOUT_CAVE_END_RIGHT = TextureRegion(1602, 763, 69, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_GRASS_WITH_CAVE_END_LEFT = TextureRegion(0, 1204, 74, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITH_CAVE_MEDIUM = TextureRegion(74, 1204, 1025, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITH_CAVE_SMALL = TextureRegion(1099, 1204, 513, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_GRASS_WITH_CAVE_END_RIGHT = TextureRegion(1612, 1204, 69, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_CAVE_END_LEFT = TextureRegion(0, 1320, 67, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_MEDIUM = TextureRegion(67, 1320, 1025, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_SMALL = TextureRegion(1092, 1320, 513, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_END_RIGHT = TextureRegion(1605, 1320, 66, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_CAVE_RAISED_END_LEFT = TextureRegion(0, 0, 94, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_RAISED_MEDIUM = TextureRegion(94, 0, 1025, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_RAISED_SMALL = TextureRegion(1119, 0, 513, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_CAVE_RAISED_END_RIGHT = TextureRegion(1632, 0, 94, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_CAVE_RAISED_LARGE = TextureRegion(0, 274, 2048, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (ground.getGroundType())
    {
        case GroundType::GROUND_GRASS_WITH_CAVE_LARGE:
            return GROUND_GRASS_WITH_CAVE_LARGE;
        case GroundType::GROUND_CAVE_LARGE:
            return GROUND_CAVE_LARGE;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_LARGE:
            return GROUND_GRASS_WITHOUT_CAVE_LARGE;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_LEFT:
            return GROUND_GRASS_WITHOUT_CAVE_END_LEFT;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_MEDIUM:
            return GROUND_GRASS_WITHOUT_CAVE_MEDIUM;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_SMALL:
            return GROUND_GRASS_WITHOUT_CAVE_SMALL;
        case GroundType::GROUND_GRASS_WITHOUT_CAVE_END_RIGHT:
            return GROUND_GRASS_WITHOUT_CAVE_END_RIGHT;
        case GroundType::GROUND_GRASS_WITH_CAVE_END_LEFT:
            return GROUND_GRASS_WITH_CAVE_END_LEFT;
        case GroundType::GROUND_GRASS_WITH_CAVE_MEDIUM:
            return GROUND_GRASS_WITH_CAVE_MEDIUM;
        case GroundType::GROUND_GRASS_WITH_CAVE_SMALL:
            return GROUND_GRASS_WITH_CAVE_SMALL;
        case GroundType::GROUND_GRASS_WITH_CAVE_END_RIGHT:
            return GROUND_GRASS_WITH_CAVE_END_RIGHT;
        case GroundType::GROUND_CAVE_END_LEFT:
            return GROUND_CAVE_END_LEFT;
        case GroundType::GROUND_CAVE_MEDIUM:
            return GROUND_CAVE_MEDIUM;
        case GroundType::GROUND_CAVE_SMALL:
            return GROUND_CAVE_SMALL;
        case GroundType::GROUND_CAVE_END_RIGHT:
            return GROUND_CAVE_END_RIGHT;
        case GroundType::GROUND_CAVE_RAISED_END_LEFT:
            return GROUND_CAVE_RAISED_END_LEFT;
        case GroundType::GROUND_CAVE_RAISED_MEDIUM:
            return GROUND_CAVE_RAISED_MEDIUM;
        case GroundType::GROUND_CAVE_RAISED_SMALL:
            return GROUND_CAVE_RAISED_SMALL;
        case GroundType::GROUND_CAVE_RAISED_END_RIGHT:
            return GROUND_CAVE_RAISED_END_RIGHT;
        case GroundType::GROUND_CAVE_RAISED_LARGE:
        default:
            return GROUND_CAVE_RAISED_LARGE;
    }
}

TextureRegion& Assets::get(LogVerticalTall& logVerticalTall)
{
    static TextureRegion tr = TextureRegion(1896, 824, 71, 91, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LogVerticalShort& logVerticalShort)
{
    static TextureRegion tr = TextureRegion(1852, 1086, 65, 52, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Thorns& thorns)
{
    static TextureRegion tr = TextureRegion(1686, 1248, 122, 63, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Stump& stump)
{
    static TextureRegion tr = TextureRegion(1680, 1086, 168, 143, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(SideSpike& sideSpike)
{
    static TextureRegion tr = TextureRegion(1786, 1342, 48, 18, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(UpwardSpike& upwardSpike)
{
    static TextureRegion TR_UPWARD_SPIKE_METAL_GRASS = TextureRegion(1686, 1316, 28, 44, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion TR_UPWARD_SPIKE_WOOD_GRASS = TextureRegion(1718, 1316, 41, 46, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion TR_UPWARD_SPIKE_METAL_CAVE = TextureRegion(1764, 1316, 18, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (upwardSpike.getUpwardSpikeType())
    {
        case UPWARD_SPIKE_METAL_GRASS:
            return TR_UPWARD_SPIKE_METAL_GRASS;
        case UPWARD_SPIKE_WOOD_GRASS:
            return TR_UPWARD_SPIKE_WOOD_GRASS;
        case UPWARD_SPIKE_METAL_CAVE:
        default:
            return TR_UPWARD_SPIKE_METAL_CAVE;
    }
}

TextureRegion& Assets::get(JumpSpring& jumpSpring)
{
    static Animation grassSpring = Animation(1852, 1142, 43, 43, 2048, 43, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 4);
    static Animation caveSpring = Animation(1824, 1256, 52, 66, 2048, 66, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 4);
    
    switch (jumpSpring.getJumpSpringType())
    {
        case JUMP_SPRING_IN_GRASS:
            return grassSpring.getTextureRegion(jumpSpring.getStateTime());
        case JUMP_SPRING_IN_CAVE:
        default:
            return caveSpring.getTextureRegion(jumpSpring.getStateTime());
    }
}

TextureRegion& Assets::get(Rock& rock)
{
    static Animation anim = Animation(0, 1528, 192, 184, 2048, 184, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 6);
    static TextureRegion tr = TextureRegion(1152, 1528, 192, 184, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    if (rock.isBlowingUp())
    {
        return anim.getTextureRegion(rock.getStateTime());
    }
    else if(rock.isCracked())
    {
        return anim.getTextureRegion(0);
    }
    else
    {
        return tr;
    }
}

TextureRegion& Assets::get(GroundPlatform& platform)
{
    static TextureRegion GROUND_PLATFORM_GRASS_DEFAULT = TextureRegion(1680, 768, 117, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_PLATFORM_GRASS_END_LEFT = TextureRegion(1800, 768, 30, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_PLATFORM_GRASS_CENTER = TextureRegion(1830, 768, 116, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_PLATFORM_GRASS_END_RIGHT = TextureRegion(1946, 768, 30, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion GROUND_PLATFORM_CAVE_END_LEFT = TextureRegion(1680, 824, 34, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_PLATFORM_CAVE_CENTER = TextureRegion(1714, 824, 116, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion GROUND_PLATFORM_CAVE_END_RIGHT = TextureRegion(1830, 824, 33, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (platform.getGroundPlatformType())
    {
        case GroundPlatformType::GROUND_PLATFORM_GRASS_DEFAULT:
            return GROUND_PLATFORM_GRASS_DEFAULT;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_LEFT:
            return GROUND_PLATFORM_GRASS_END_LEFT;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_CENTER:
            return GROUND_PLATFORM_GRASS_CENTER;
        case GroundPlatformType::GROUND_PLATFORM_GRASS_END_RIGHT:
            return GROUND_PLATFORM_GRASS_END_RIGHT;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_LEFT:
            return GROUND_PLATFORM_CAVE_END_LEFT;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_CENTER:
            return GROUND_PLATFORM_CAVE_CENTER;
        case GroundPlatformType::GROUND_PLATFORM_CAVE_END_RIGHT:
        default:
            return GROUND_PLATFORM_CAVE_END_RIGHT;
    }
}

TextureRegion& Assets::get(EndSign& endSign)
{
    static TextureRegion tr = TextureRegion(1990, 768, 34, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Carrot& carrot)
{
    static TextureRegion tr = TextureRegion(1680, 878, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(GoldenCarrot& goldenCarrot)
{
    static Animation anim = Animation(0, 1716, 104, 98, 2048, 98, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 6, 0.68f, 0.06f, 0.06f, 0.06f, 0.06f, 0.06f);
    
    return anim.getTextureRegion(goldenCarrot.getStateTime());
}

TextureRegion& Assets::get(DustCloud& dustCloud)
{
    static Animation anim = Animation(0, 552, 115, 60, 2048, 60, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.08f, 5);
    
    return anim.getTextureRegion(dustCloud.getStateTime());
}

TextureRegion& Assets::get(Jon &jon)
{
    static Animation jonRunningAnim = Animation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.07f, 10);
    static Animation jonJumpingAnim = Animation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.09f, 7);
    static Animation jonDoubleJumpingAnim = Animation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.07f, 9);
    static Animation jonFallingAnim = Animation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 3);
    static Animation jonLandingAnim = Animation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 4);
    static Animation jonSpinningBackFistAnimation = Animation(0, 0, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 8);
    
    switch (jon.getAbilityState())
    {
        case ABILITY_SPINNING_BACK_FIST:
            return jonSpinningBackFistAnimation.getTextureRegion(jon.getAbilityStateTime());
        case ABILITY_NONE:
        default:
            break;
    }
    
    switch (jon.getActionState())
    {
        case ACTION_JUMPING:
            return jonJumpingAnim.getTextureRegion(jon.getActionStateTime());
        case ACTION_DOUBLE_JUMPING:
            return jonDoubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
        case ACTION_NONE:
        default:
            break;
    }
    
    if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
    {
        if (jon.isLanding())
        {
            return jonLandingAnim.getTextureRegion(jon.getStateTime());
        }
    }
    else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
    {
        if (jon.isFalling())
        {
            return jonFallingAnim.getTextureRegion(jon.getStateTime());
        }
    }
    
    return jonRunningAnim.getTextureRegion(jon.getStateTime());
}

void Assets::setMusicId(short musicId)
{
    m_sMusicId = musicId;
}

short Assets::getMusicId()
{
    return m_sMusicId;
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

Assets::Assets()
{
    // Hide Constructor for Singleton
}