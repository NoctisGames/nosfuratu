//
//  FramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "FramebufferRadialBlurGpuProgramWrapper.h"

#include <assert.h>

FramebufferRadialBlurGpuProgramWrapper::FramebufferRadialBlurGpuProgramWrapper() : m_fDirection(0.0f)
{
    // Empty
}

void FramebufferRadialBlurGpuProgramWrapper::configure(float direction)
{
    assert(direction <= 1.0f && direction >= 0.0f);
    
    m_fDirection = direction;
}