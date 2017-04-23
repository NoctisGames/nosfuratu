//
//  ShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ShockwaveTextureGpuProgramWrapper__
#define __nosfuratu__ShockwaveTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class Vector2D;

class ShockwaveTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    ShockwaveTextureGpuProgramWrapper();
    
    virtual ~ShockwaveTextureGpuProgramWrapper();
    
    void configure(float x, float y, float timeElapsed, bool isTransforming);
    
protected:
    Vector2D* m_center;
    float m_fTimeElapsed;
    bool m_isTransforming;
};

#endif /* defined(__nosfuratu__ShockwaveTextureGpuProgramWrapper__) */
