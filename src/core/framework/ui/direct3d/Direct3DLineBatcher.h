//
//  Direct3DLineBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/16/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DLineBatcher__
#define __gowengamedev__Direct3DLineBatcher__

#include "LineBatcher.h"

class Direct3DLineBatcher : public LineBatcher
{
public:
	Direct3DLineBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    
    virtual void beginBatch();
    
    virtual void endBatch();

	virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);
    
    virtual void renderLine(float originX, float originY, float endX, float endY, Color &color);

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__gowengamedev__Direct3DLineBatcher__) */
