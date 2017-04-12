//
//  PointTransitionGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "PointTransitionGpuProgramWrapper.h"

#include "Vector2D.h"
#include "GpuTextureWrapper.h"

PointTransitionGpuProgramWrapper::PointTransitionGpuProgramWrapper() : m_to(nullptr), m_fProgress(0)
{
    m_center = new Vector2D();
}

PointTransitionGpuProgramWrapper::~PointTransitionGpuProgramWrapper()
{
    delete m_center;
}

void PointTransitionGpuProgramWrapper::configure(GpuTextureWrapper* to, float x, float y, float progress)
{
    m_center->set(x, y);
    m_to = to;
    m_fProgress = progress;
}
