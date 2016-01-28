//
//  TransTitleToWorldGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__TransTitleToWorldGpuProgramWrapper__
#define __gowengamedev__TransTitleToWorldGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "TextureWrapper.h"

class TransTitleToWorldGpuProgramWrapper : public GpuProgramWrapper
{
public:
    TransTitleToWorldGpuProgramWrapper();
    
    void configure(TextureWrapper* to, float progress);
    
protected:
    TextureWrapper* m_to;
    float m_fProgress;
};

#endif /* defined(__gowengamedev__TransTitleToWorldGpuProgramWrapper__) */
