//
//  Direct3DSpriteBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DSpriteBatcher__
#define __gowengamedev__Direct3DSpriteBatcher__

#include "SpriteBatcher.h"
#include <vector>

class Direct3DSpriteBatcher : public SpriteBatcher
{
public:
	Direct3DSpriteBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    
	virtual void beginBatch();

	virtual void endBatch(TextureWrapper &textureWrapper);

	virtual void endBatch(TextureWrapper &textureWrapper, GpuProgramWrapper &gpuProgramWrapper);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
protected:
	virtual void drawSprite(float x, float y, float width, float height, TextureRegion tr);

	virtual void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__gowengamedev__Direct3DSpriteBatcher__) */