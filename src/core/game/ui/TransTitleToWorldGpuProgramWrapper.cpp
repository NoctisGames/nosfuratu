//
//  TransTitleToWorldGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#include "TransTitleToWorldGpuProgramWrapper.h"

TransTitleToWorldGpuProgramWrapper::TransTitleToWorldGpuProgramWrapper() : m_to(nullptr), m_fProgress(0)
{
    // Empty
}

void TransTitleToWorldGpuProgramWrapper::configure(TextureWrapper* to, float progress)
{
    m_to = to;
    m_fProgress = progress;
}