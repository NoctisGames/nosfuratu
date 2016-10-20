//
//  Direct3DSpriteBatcher.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DSpriteBatcher__
#define __noctisgames__Direct3DSpriteBatcher__

#include "SpriteBatcher.h"
#include <vector>

class Direct3DSpriteBatcher : public SpriteBatcher
{
public:
	Direct3DSpriteBatcher(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    
	virtual void beginBatch();

	virtual void endBatch(GpuTextureWrapper &textureWrapper);

	virtual void endBatch(GpuTextureWrapper &textureWrapper, GpuProgramWrapper &gpuProgramWrapper);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
protected:
	virtual void drawSprite(float x, float y, float width, float height, TextureRegion tr);

	virtual void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

#endif /* defined(__noctisgames__Direct3DSpriteBatcher__) */