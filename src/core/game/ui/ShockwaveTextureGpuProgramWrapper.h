//
//  ShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__ShockwaveTextureGpuProgramWrapper__
#define __noctisgames__ShockwaveTextureGpuProgramWrapper__

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

#endif /* defined(__noctisgames__ShockwaveTextureGpuProgramWrapper__) */
