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
#include "GamePlatform.h"
#include "Carrot.h"
#include "Ground.h"

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::getSky(Jon& jon)
{
    static TextureRegion tr = TextureRegion(0, 0, 686, 769, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    float y = GAME_HEIGHT - jon.getPosition().getY();
    y /= GAME_HEIGHT;
    y *= 169;
    y =  y < 0 ? 0 : y;
    tr.init(jon.getPosition().getX() * 8, y, 686, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getBackgroundTrees(Jon& jon)
{
    static TextureRegion tr = TextureRegion(0, 776, 686, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(jon.getPosition().getX() * 16, 776, 686, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getMidgroundBushes(Jon& jon)
{
    static TextureRegion tr = TextureRegion(0, 1292, 686, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(jon.getPosition().getX() * 32, 1292, 686, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getMidgroundTree(Tree& tree)
{
    static TextureRegion tree1 = TextureRegion(0, 0, 448, 418, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tree2 = TextureRegion(452, 0, 251, 330, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tree3 = TextureRegion(706, 0, 317, 389, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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

TextureRegion& Assets::getForeground(Ground& ground)
{
    static Animation groundParts = Animation(0, 1611, 512, 437, 2048, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.0f, 4);
    
    return groundParts.getTextureRegion(ground.getGroundType());
}

TextureRegion& Assets::getPlatform(GamePlatform& platform)
{
    static TextureRegion tr = TextureRegion(4, 1404, 227, 98, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getCarrot(Carrot& carrot)
{
    static TextureRegion tr = TextureRegion(276, 1404, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getJon(Jon &jon)
{
    static Animation jonRunningAnim = Animation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.07f, 10);
    static Animation jonJumpingAnim = Animation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.12f, 8);
    static Animation jonDoubleJumpingAnim = Animation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.08f, 9);
    static Animation jonFallingAnim = Animation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 2);
    
    float scalar = JON_DEFAULT_MAX_SPEED / jon.getVelocity().getX();
    return jon.isFalling() ? jonFallingAnim.getTextureRegion(jon.getStateTime(), scalar) : jon.getNumJumps() == 2 ? jonDoubleJumpingAnim.getTextureRegion(jon.getStateTime(), scalar) : jon.getNumJumps() == 1 ? jonJumpingAnim.getTextureRegion(jon.getStateTime(), scalar) : jonRunningAnim.getTextureRegion(jon.getStateTime(), scalar);
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