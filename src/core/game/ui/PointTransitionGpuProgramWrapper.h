//
//  PointTransitionGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__PointTransitionGpuProgramWrapper__
#define __noctisgames__PointTransitionGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GpuTextureWrapper.h"

class PointTransitionGpuProgramWrapper : public GpuProgramWrapper
{
public:
    PointTransitionGpuProgramWrapper();
    
    void configure(GpuTextureWrapper* to, float x, float y, float progress);
    
protected:
    std::unique_ptr<Vector2D> m_center;
    GpuTextureWrapper* m_to;
    float m_fProgress;
};

#endif /* defined(__noctisgames__PointTransitionGpuProgramWrapper__) */
