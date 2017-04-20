//
//  RendererType.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__RendererType__
#define __nosfuratu__RendererType__

typedef enum
{
	RENDERER_TYPE_NONE,
    RENDERER_TYPE_TITLE,
    RENDERER_TYPE_WORLD_MAP,
    RENDERER_TYPE_LEVEL_EDITOR,
    RENDERER_TYPE_WORLD_1_CUTSCENE,
    RENDERER_TYPE_WORLD_1,
    RENDERER_TYPE_WORLD_1_OBJECTS_PART_1,
    RENDERER_TYPE_WORLD_1_MID_BOSS,
    RENDERER_TYPE_WORLD_1_END_BOSS
} RendererType;

inline RendererType calcRendererTypeFromLevel(int world, int level)
{
	if (world == 1)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_1_END_BOSS;
		}
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_1_MID_BOSS;
		}
		else
		{
			return RENDERER_TYPE_WORLD_1;
		}
	}

	return RENDERER_TYPE_NONE;
}

#endif /* defined(__nosfuratu__RendererType__) */
