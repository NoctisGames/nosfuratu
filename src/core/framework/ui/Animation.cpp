//
//  Animation.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "Animation.h"
#include "TextureRegion.h"
#include <stdarg.h>

Animation::Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames, ...) : m_cycleTime(0), m_looping(looping)
{
    loadTextureRegions(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, numFrames);
    
    va_list arguments;
    
    va_start(arguments, numFrames);
    
    for (int i = 0; i < numFrames; i++)
    {
        float f = va_arg(arguments, double);
        m_frameTimes.push_back(f);
        m_cycleTime += f;
    }
    
    va_end(arguments);
}

Animation::Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames) : m_cycleTime(0), m_looping(looping)
{
    loadTextureRegions(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, numFrames);
    
    for (int i = 0; i < numFrames; i++)
    {
        m_frameTimes.push_back(frameTime);
        m_cycleTime += frameTime;
    }
}

TextureRegion& Animation::getTextureRegion(float stateTime)
{
    int keyFrameNumber = getKeyFrameNumber(stateTime);
    
    return getTextureRegion(keyFrameNumber);
}

TextureRegion& Animation::getTextureRegion(int keyFrameNumber)
{
    return m_textureRegions.at(keyFrameNumber);
}

Animation::~Animation()
{
    m_textureRegions.clear();
    m_frameTimes.clear();
}

void Animation::loadTextureRegions(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, int numFrames)
{
    int right = x + animationWidth;
    int bottom = y + animationHeight;
    int numTextureRegionsAdded = 0;
    for (int j = y; j < bottom; j += regionHeight)
    {
        for (int i = x; i < right; i += regionWidth)
        {
            TextureRegion tr = TextureRegion(i, j, regionWidth, regionHeight, textureWidth, textureHeight);
            m_textureRegions.push_back(tr);
            numTextureRegionsAdded++;
            
            if (numTextureRegionsAdded == numFrames)
            {
                return;
            }
        }
    }
}

int Animation::getKeyFrameNumber(float stateTime)
{
    if (stateTime > m_cycleTime && m_cycleTime > 0)
    {
        if (m_looping)
        {
            while (stateTime > m_cycleTime)
            {
                stateTime -= m_cycleTime;
            }
        }
        else
        {
            return m_frameTimes.at(m_frameTimes.size() - 1);
        }
    }
    
    for (unsigned int i = 0; i < m_frameTimes.size(); i++)
    {
        if (stateTime < m_frameTimes.at(i))
        {
            return i;
        }
        
        stateTime -= m_frameTimes.at(i);
    }
    
    return 0;
}