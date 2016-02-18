//
//  TransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__TransDeathGpuProgramWrapper__
#define __noctisgames__TransDeathGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "TextureWrapper.h"

class TransDeathGpuProgramWrapper : public GpuProgramWrapper
{
public:
    TransDeathGpuProgramWrapper(bool isTransIn);
    
    void configure(TextureWrapper* grayMap, float timeElapsed);
    
protected:
    TextureWrapper* m_grayMap;
    float m_fTimeElapsed;
    bool m_isTransIn;
};

#endif /* defined(__noctisgames__TransDeathGpuProgramWrapper__) */
