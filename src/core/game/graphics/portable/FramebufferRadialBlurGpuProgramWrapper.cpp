//
//  FramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "FramebufferRadialBlurGpuProgramWrapper.h"

#include <assert.h>

FramebufferRadialBlurGpuProgramWrapper::FramebufferRadialBlurGpuProgramWrapper() : m_fDirection(0.0f)
{
    // Empty
}

FramebufferRadialBlurGpuProgramWrapper::~FramebufferRadialBlurGpuProgramWrapper()
{
    // Override
}

void FramebufferRadialBlurGpuProgramWrapper::configure(float direction)
{
    assert(direction <= 1.0f && direction >= 0.0f);
    
    m_fDirection = direction;
}
