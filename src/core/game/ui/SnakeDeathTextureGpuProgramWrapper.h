//
//  SnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__SnakeDeathTextureGpuProgramWrapper__
#define __gowengamedev__SnakeDeathTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class SnakeDeathTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
    SnakeDeathTextureGpuProgramWrapper();
    
    void setColorAdditive(float colorAdditive);
    
protected:
    float m_fColorAdditive;
};

#endif /* defined(__gowengamedev__SnakeDeathTextureGpuProgramWrapper__) */
