//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Assets.h"
#include "TextureRegion.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "ResourceConstants.h"

Assets * Assets::getInstance()
{
    static Assets *assets = new Assets();
    return assets;
}

TextureRegion& Assets::getBackground()
{
    static TextureRegion background = TextureRegion(0, 256, 1024, 576, TEXTURE_SIZE_1024x1024, TEXTURE_SIZE_1024x1024);
    return background;
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

int Assets::getKeyFrameNumber(float stateTime, float cycleTime, std::vector<float> &frameDurations)
{
    if (stateTime > cycleTime && cycleTime > 0)
    {
        while (stateTime > cycleTime)
        {
            stateTime -= cycleTime;
        }
    }

    for (unsigned int i = 0; i < frameDurations.size(); i++)
    {
        if (stateTime < frameDurations.at(i))
        {
            return i;
        }

        stateTime -= frameDurations.at(i);
    }

    return 0;
}

Assets::Assets()
{
    // Hide Constructor for Singleton
}