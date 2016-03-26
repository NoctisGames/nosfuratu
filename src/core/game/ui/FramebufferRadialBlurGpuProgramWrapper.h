//
//  FramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__FramebufferRadialBlurGpuProgramWrapper__
#define __noctisgames__FramebufferRadialBlurGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"

#include <memory>

class FramebufferRadialBlurGpuProgramWrapper : public GpuProgramWrapper
{
public:
    FramebufferRadialBlurGpuProgramWrapper();
    
    void configure(float direction);
    
protected:
    float m_fDirection; // Some value between 1.0 and 0.0, where 1.0 is top and 0.0 is bottom
};

#endif /* defined(__noctisgames__FramebufferRadialBlurGpuProgramWrapper__) */
