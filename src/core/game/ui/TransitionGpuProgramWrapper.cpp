//
//  TransitionGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "TransitionGpuProgramWrapper.h"

TransitionGpuProgramWrapper::TransitionGpuProgramWrapper() : m_to(nullptr), m_fProgress(0)
{
    // Empty
}

void TransitionGpuProgramWrapper::configure(TextureWrapper* to, float progress)
{
    m_to = to;
    m_fProgress = progress;
}