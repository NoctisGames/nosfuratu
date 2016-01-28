//
//  TransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__TransDeathGpuProgramWrapper__
#define __gowengamedev__TransDeathGpuProgramWrapper__

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

#endif /* defined(__gowengamedev__TransDeathGpuProgramWrapper__) */
