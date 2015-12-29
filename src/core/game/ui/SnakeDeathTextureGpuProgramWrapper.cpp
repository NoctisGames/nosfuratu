//
//  SnakeDeathTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeDeathTextureGpuProgramWrapper.h"

SnakeDeathTextureGpuProgramWrapper::SnakeDeathTextureGpuProgramWrapper() : m_fColorAdditive(0.0f)
{
    // Empty
}

void SnakeDeathTextureGpuProgramWrapper::setColorAdditive(float colorAdditive)
{
    m_fColorAdditive = colorAdditive;
}