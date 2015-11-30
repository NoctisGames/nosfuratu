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
#include "DeviceResources.h"

class Direct3DFrameBufferToScreenGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DFrameBufferToScreenGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__gowengamedev__Direct3DFrameBufferToScreenGpuProgramWrapper__) */
