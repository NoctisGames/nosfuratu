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
#include "ResourceConstants.h"
#include "GameConstants.h"
#include "Tree.h"
#include "Jon.h"
#include "GroundPlatform.h"
#include "Carrot.h"
#include "Ground.h"
#include "LogVerticalTall.h"
#include "GroundType.h"
#include "Game.h"

#define PIXEL_WIDTH_FOR_GAME 684.0f
#define PIXEL_HEIGHT_FOR_GAME 1154.0f

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::getBackgroundSky(Jon& jon, Game& game)
{
    static TextureRegion tr = TextureRegion(0, 0, PIXEL_WIDTH_FOR_GAME, 769, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    float y = GAME_HEIGHT - jon.getPosition().getY();
    y /= GAME_HEIGHT;
    y *= 169;
    y =  y < 0 ? 0 : y;
    
    float x = jon.getPosition().getX() * 6;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (jon.getPosition().getX() > farCamPos)
    {
        x = farCamPos * 6;
    }
    
    tr.init(x, y, PIXEL_WIDTH_FOR_GAME, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getBackgroundTrees(Jon& jon, Game& game)
{
    static TextureRegion tr = TextureRegion(0, 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    float x = jon.getPosition().getX() * 12;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (jon.getPosition().getX() > farCamPos)
    {
        x = farCamPos * 12;
    }
    
    tr.init(x, 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getBackgroundCave(Jon& jon, Game& game)
{
    static TextureRegion tr = TextureRegion(0, 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    float x = jon.getPosition().getX() * 24;
    float farRight = game.getFarRight();
    float farCamPos = farRight - CAM_WIDTH + JON_STARTING_X;
    if (jon.getPosition().getX() > farCamPos)
    {
        x = farCamPos * 24;
    }
    
    tr.init(x, 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getMidgroundTree(Tree& tree)
{
    static TextureRegion tree1 = TextureRegion(0, 0, 270, 346, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tree2 = TextureRegion(274, 0, 480, 439, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tree3 = TextureRegion(758, 0, 339, 411, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (tree.getTreeType())
    {
        case 0:
            return tree1;
        case 1:
            return tree2;
        case 2:
            return tree3;
        default:
            return tree3;
    }
}

TextureRegion& Assets::getGround(Ground& ground)
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

TextureRegion& Assets::getLogVerticalTall(LogVerticalTall& logVerticalTall)
{
    static TextureRegion tr = TextureRegion(1896, 824, 71, 91, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getGroundPlatform(GroundPlatform& platform)
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

TextureRegion& Assets::getEndSign()
{
    static TextureRegion tr = TextureRegion(1990, 768, 34, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getCarrot(Carrot& carrot)
{
    static TextureRegion tr = TextureRegion(1680, 878, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getGoldenCarrot(GoldenCarrot& goldenCarrot)
{
    static TextureRegion tr = TextureRegion(1786, 878, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getJon(Jon &jon)
{
    static Animation jonRunningAnim = Animation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.07f, 10);
    static Animation jonJumpingAnim = Animation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 8);
    static Animation jonDoubleJumpingAnim = Animation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 9);
    static Animation jonFallingAnim = Animation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 3);
    static Animation jonLandingAnim = Animation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 4);
    static Animation jonSpinningBackFistAnimation = Animation(0, 0, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 8);
    
    float scalar = JON_DEFAULT_MAX_SPEED / jon.getVelocity().getX();
    
    switch (jon.getAbilityState())
    {
        case ABILITY_SPINNING_BACK_FIST:
            return jonSpinningBackFistAnimation.getTextureRegion(jon.getAbilityStateTime(), scalar);
        case ABILITY_NONE:
        default:
            break;
    }
    
    switch (jon.getActionState())
    {
        case ACTION_JUMPING:
            return jonJumpingAnim.getTextureRegion(jon.getActionStateTime(), scalar);
        case ACTION_DOUBLE_JUMPING:
            return jonDoubleJumpingAnim.getTextureRegion(jon.getActionStateTime(), scalar);
        case ACTION_NONE:
        default:
            break;
    }
    
    if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
    {
        if (jon.isLanding())
        {
            return jonLandingAnim.getTextureRegion(jon.getStateTime(), scalar);
        }
    }
    else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
    {
        if (jon.isFalling())
        {
            return jonFallingAnim.getTextureRegion(jon.getStateTime(), scalar);
        }
    }
    
    return jonRunningAnim.getTextureRegion(jon.getStateTime(), scalar);
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