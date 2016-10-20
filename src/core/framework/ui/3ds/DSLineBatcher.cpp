//
//  DSLineBatcher.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/25/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "DSLineBatcher.h"
#include "macros.h"
#include "Line.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "DummyGpuProgramWrapper.h"

#include <sf2d.h>

#define LINE_THICKNESS CAM_WIDTH / 1000.0f

DSLineBatcher::DSLineBatcher(gfxScreen_t screen, int screenWidth, int screenHeight) : m_screen(screen), m_iScreenWidth(screenWidth), m_iScreenHeight(screenHeight)
{
    m_iNumLines = 0;
}

void DSLineBatcher::beginBatch()
{
    m_lines.clear();
    m_iNumLines = 0;
}

void DSLineBatcher::endBatch()
{
    endBatch(*DummyGpuProgramWrapper::getInstance());
}

void DSLineBatcher::endBatch(GpuProgramWrapper &gpuProgramWrapper)
{
    if (m_iNumLines > 0)
    {
        for (std::vector<LINE>::iterator i = m_lines.begin(); i != m_lines.end(); ++itr)
        {
            LINE l = *i;

            sf2d_draw_line(l.oX, l.oY, l.eX, l.eY, LINE_THICKNESS, RGBA8((int) (l.r * 255), (int) (l.g * 255), (int) (l.b * 255), (int) (l.a * 255)));
        }
    }
}

void DSLineBatcher::renderLine(float originX, float originY, float endX, float endY, Color &c)
{
    LINE l = {originX, CAM_HEIGHT - originY, endX, CAM_HEIGHT - endY, c.red, c.green, c.blue, c.alpha};
    m_lines.push_back(l);

    m_iNumLines++;
}