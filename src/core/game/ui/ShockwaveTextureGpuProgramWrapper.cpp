//
//  ShockwaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "ShockwaveTextureGpuProgramWrapper.h"

ShockwaveTextureGpuProgramWrapper::ShockwaveTextureGpuProgramWrapper() : m_fTimeElapsed(0), m_isTransforming(false)
{
    m_center = std::unique_ptr<Vector2D>(new Vector2D());
}

void ShockwaveTextureGpuProgramWrapper::configure(float x, float y, float timeElapsed, bool isTransforming)
{
    m_center->set(x, y);
    m_fTimeElapsed = timeElapsed;
    m_isTransforming = isTransforming;
}