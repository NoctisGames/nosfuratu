//
//  Direct3DSinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/18/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DSinWaveTextureGpuProgramWrapper__
#define __gowengamedev__Direct3DSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
	Direct3DSinWaveTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__gowengamedev__Direct3DSinWaveTextureGpuProgramWrapper__) */
