//
//  FramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/8/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__FramebufferRadialBlurGpuProgramWrapper__
#define __noctisgames__FramebufferRadialBlurGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"

#include <memory>

#define RADIAL_BLUR_DIRECTION_LEFT 0
#define RADIAL_BLUR_DIRECTION_TOP_LEFT 1
#define RADIAL_BLUR_DIRECTION_BOTTOM_LEFT 2

class FramebufferRadialBlurGpuProgramWrapper : public GpuProgramWrapper
{
public:
    FramebufferRadialBlurGpuProgramWrapper();
    
    void configure(int direction);
    
protected:
    int m_iDirection;
};

#endif /* defined(__noctisgames__FramebufferRadialBlurGpuProgramWrapper__) */
