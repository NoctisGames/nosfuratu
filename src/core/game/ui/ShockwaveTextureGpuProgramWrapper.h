//
//  ShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__ShockwaveTextureGpuProgramWrapper__
#define __gowengamedev__ShockwaveTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"

#include <memory>

class ShockwaveTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    ShockwaveTextureGpuProgramWrapper();
    
    void configure(float x, float y, float timeElapsed, bool isTransforming);
    
protected:
    std::unique_ptr<Vector2D> m_center;
    float m_fTimeElapsed;
    bool m_isTransforming;
};

#endif /* defined(__gowengamedev__ShockwaveTextureGpuProgramWrapper__) */
