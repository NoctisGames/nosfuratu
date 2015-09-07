//
//  Direct3DFrameBufferToScreenGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/7/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DFrameBufferToScreenGpuProgramWrapper__
#define __gowengamedev__Direct3DFrameBufferToScreenGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class Direct3DFrameBufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DFrameBufferToScreenGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();
};

#endif /* defined(__gowengamedev__Direct3DFrameBufferToScreenGpuProgramWrapper__) */
