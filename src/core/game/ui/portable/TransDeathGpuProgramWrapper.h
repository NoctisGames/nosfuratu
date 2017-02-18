//
//  TransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__TransDeathGpuProgramWrapper__
#define __nosfuratu__TransDeathGpuProgramWrapper__

#include "GpuProgramWrapper.h"

struct GpuTextureWrapper;

class TransDeathGpuProgramWrapper : public GpuProgramWrapper
{
public:
    TransDeathGpuProgramWrapper(bool isTransIn);
    
    virtual ~TransDeathGpuProgramWrapper();
    
    void configure(GpuTextureWrapper* grayMap, float timeElapsed);
    
protected:
    GpuTextureWrapper* m_grayMap;
    float m_fTimeElapsed;
    bool m_isTransIn;
};

#endif /* defined(__nosfuratu__TransDeathGpuProgramWrapper__) */
