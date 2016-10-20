//
//  Direct3DRectangleBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DRectangleBatcher__
#define __noctisgames__Direct3DRectangleBatcher__

#include "RectangleBatcher.h"
#include "Direct3DManager.h"

class Direct3DRectangleBatcher : public RectangleBatcher
{
public:
	Direct3DRectangleBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isFill = false);

	virtual void beginBatch();

	virtual void endBatch();

	void renderRectangle(Rectangle &rectangle, Color &color);

	virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__noctisgames__Direct3DRectangleBatcher__) */
