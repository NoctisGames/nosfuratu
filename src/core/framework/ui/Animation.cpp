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

Animation::Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames, ...) : m_fCycleTime(0), m_iFirstLoopingFrame(0), m_looping(looping)
{
	loadTextureRegions(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, numFrames);

	va_list arguments;

	va_start(arguments, numFrames);

	for (int i = 0; i < numFrames; i++)
	{
		float f = va_arg(arguments, double);
		m_frameTimes.push_back(f);
		m_fCycleTime += f;
	}

	va_end(arguments);
}

Animation::Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame) : m_fCycleTime(0), m_iFirstLoopingFrame(firstLoopingFrame), m_looping(looping)
{
	loadTextureRegions(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, numFrames);

	for (int i = 0; i < numFrames; i++)
	{
		m_frameTimes.push_back(frameTime);
		m_fCycleTime += frameTime;
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
    unsigned int i = 0;
    
	if (stateTime > m_fCycleTime && m_fCycleTime > 0)
	{
		if (m_looping)
		{
            float cycleTime = m_fCycleTime;
            for ( ; i < m_iFirstLoopingFrame; i++)
            {
                cycleTime -= m_frameTimes.at(i);
            }
            
			while (stateTime > cycleTime)
			{
				stateTime -= cycleTime;
			}
		}
		else
		{
			return ((int) m_frameTimes.size()) - 1;
		}
	}

	for ( ; i < m_frameTimes.size(); i++)
	{
		float frameTime = m_frameTimes.at(i);

		if (stateTime < frameTime)
		{
            return i;
		}

		stateTime -= frameTime;
	}

	return 0;
}