//
//  Direct3DCircleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 11/16/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DCircleBatcher__
#define __noctisgames__Direct3DCircleBatcher__

#include "CircleBatcher.h"

class Direct3DCircleBatcher : public CircleBatcher
{
public:
	Direct3DCircleBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void renderCircle(Circle &circle, Color &c);

	virtual void renderPartialCircle(Circle &circle, int arcDegrees, Color &c);

	virtual void renderCircle(Circle &circle, Color &c, GpuProgramWrapper &gpuProgramWrapper);

	virtual void renderPartialCircle(Circle &circle, int arcDegrees, Color &c, GpuProgramWrapper &gpuProgramWrapper);

protected:
	virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;

	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a);
};

#endif /* defined(__noctisgames__Direct3DCircleBatcher__) */
