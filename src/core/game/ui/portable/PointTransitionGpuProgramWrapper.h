//
//  PointTransitionGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__PointTransitionGpuProgramWrapper__
#define __nosfuratu__PointTransitionGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class Vector2D;
struct GpuTextureWrapper;

class PointTransitionGpuProgramWrapper : public GpuProgramWrapper
{
public:
    PointTransitionGpuProgramWrapper();
    
    virtual ~PointTransitionGpuProgramWrapper();
    
    void configure(GpuTextureWrapper* to, float x, float y, float progress);
    
protected:
    Vector2D* m_center;
    GpuTextureWrapper* m_to;
    float m_fProgress;
};

#endif /* defined(__nosfuratu__PointTransitionGpuProgramWrapper__) */
