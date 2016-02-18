//
//  SinWaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#include "SinWaveTextureGpuProgramWrapper.h"

SinWaveTextureGpuProgramWrapper::SinWaveTextureGpuProgramWrapper() : m_fOffset(0.0f)
{
    // Empty
}

void SinWaveTextureGpuProgramWrapper::setOffset(float offset)
{
    m_fOffset = offset;
}