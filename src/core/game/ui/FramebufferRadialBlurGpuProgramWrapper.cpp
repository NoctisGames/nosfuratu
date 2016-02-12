//
//  FramebufferRadialBlurGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "FramebufferRadialBlurGpuProgramWrapper.h"

FramebufferRadialBlurGpuProgramWrapper::FramebufferRadialBlurGpuProgramWrapper() : m_iDirection(0)
{
    // Empty
}

void FramebufferRadialBlurGpuProgramWrapper::configure(int direction)
{
    m_iDirection = direction;
}