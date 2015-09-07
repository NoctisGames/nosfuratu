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
#include "Jon.h"
#include "Platform.h"
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
    tr.init(jon.getPosition().getX() * 8, y, 686, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getBackgroundTrees(Jon& jon)
{
    static TextureRegion tr = TextureRegion(0, 776, 686, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(jon.getPosition().getX() * 16, 776, 686, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getMidgroundTrees(Jon& jon)
{
    static TextureRegion tr = TextureRegion(0, 1292, 686, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(jon.getPosition().getX() * 32, 1292, 686, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::getForeground(Ground& ground)
{
    static Animation groundParts = Animation(0, 1611, 512, 437, 2048, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.0f, 4);
    
    return groundParts.getTextureRegion(ground.getGroundType());
}

TextureRegion& Assets::getPlatform(Platform& platform)
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
    static Animation jonRunningAnim = Animation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 14);
    static Animation jonJumpingAnim = Animation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.1f, 8);
    static Animation jonDoubleJumpingAnim = Animation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 9);
    static Animation jonFallingAnim = Animation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 2);
    
    return jon.isFalling() ? jonFallingAnim.getTextureRegion(jon.getStateTime()) : jon.getNumJumps() == 2 ? jonDoubleJumpingAnim.getTextureRegion(jon.getStateTime()) : jon.getNumJumps() == 1 ? jonJumpingAnim.getTextureRegion(jon.getStateTime()) : jonRunningAnim.getTextureRegion(jon.getStateTime());
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