//
//  ShockwaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "ShockwaveTextureGpuProgramWrapper.h"

#include "Vector2D.h"

ShockwaveTextureGpuProgramWrapper::ShockwaveTextureGpuProgramWrapper() : m_center(nullptr), m_fTimeElapsed(0), m_isTransforming(false)
{
    m_center = new Vector2D();
}

ShockwaveTextureGpuProgramWrapper::~ShockwaveTextureGpuProgramWrapper()
{
    delete m_center;
}

void ShockwaveTextureGpuProgramWrapper::configure(float x, float y, float timeElapsed, bool isTransforming)
{
    m_center->set(x, y);
    
    m_fTimeElapsed = timeElapsed;
    m_isTransforming = isTransforming;
}
