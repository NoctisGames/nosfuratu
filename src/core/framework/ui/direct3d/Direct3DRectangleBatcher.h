//
//  Direct3DRectangleBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DRectangleBatcher__
#define __gowengamedev__Direct3DRectangleBatcher__

#include "RectangleBatcher.h"

class Direct3DRectangleBatcher : public RectangleBatcher
{
public:
	Direct3DRectangleBatcher(bool isFill);

	virtual void beginBatch();

	virtual void endBatch();

	virtual void endBatch(GpuProgramWrapper &gpuProgramWrapper);

	void renderRectangle(Rectangle &rectangle, Color &color);

	virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
};

#endif /* defined(__gowengamedev__Direct3DRectangleBatcher__) */
