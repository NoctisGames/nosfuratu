//
//  RendererType.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__RendererType__
#define __nosfuratu__RendererType__

typedef enum
{
	RENDERER_TYPE_NONE,
    RENDERER_TYPE_TITLE,
    RENDERER_TYPE_WORLD_MAP,
    RENDERER_TYPE_LEVEL_EDITOR,
    RENDERER_TYPE_WORLD_1_PRE_LEVEL_10,
    RENDERER_TYPE_WORLD_1_MID_BOSS_PART_1,
    RENDERER_TYPE_WORLD_1_MID_BOSS_PART_2,
    RENDERER_TYPE_WORLD_1_POST_LEVEL_10,
    RENDERER_TYPE_WORLD_1_END_BOSS,
    RENDERER_TYPE_WORLD_2,
    RENDERER_TYPE_WORLD_2_MID_BOSS,
    RENDERER_TYPE_WORLD_2_END_BOSS,
    RENDERER_TYPE_WORLD_3,
    RENDERER_TYPE_WORLD_3_MID_BOSS,
    RENDERER_TYPE_WORLD_3_END_BOSS,
    RENDERER_TYPE_WORLD_4,
    RENDERER_TYPE_WORLD_4_MID_BOSS,
    RENDERER_TYPE_WORLD_4_END_BOSS,
    RENDERER_TYPE_WORLD_5,
    RENDERER_TYPE_WORLD_5_MID_BOSS,
    RENDERER_TYPE_WORLD_5_END_BOSS
} RendererType;

inline RendererType calcRendererTypeFromLevel(int world, int level)
{
	if (world == 1)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_1_END_BOSS;
		}
        else if (level > 10)
        {
            return RENDERER_TYPE_WORLD_1_POST_LEVEL_10;
        }
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_1_MID_BOSS_PART_1;
		}
		else
		{
			return RENDERER_TYPE_WORLD_1_PRE_LEVEL_10;
		}
	}
	else if (world == 2)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_2_END_BOSS;
		}
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_2_MID_BOSS;
		}
		else
		{
			return RENDERER_TYPE_WORLD_2;
		}
	}
	else if (world == 3)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_3_END_BOSS;
		}
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_3_MID_BOSS;
		}
		else
		{
			return RENDERER_TYPE_WORLD_3;
		}
	}
	else if (world == 4)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_4_END_BOSS;
		}
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_4_MID_BOSS;
		}
		else
		{
			return RENDERER_TYPE_WORLD_4;
		}
	}
	else if (world == 5)
	{
		if (level == 21)
		{
			return RENDERER_TYPE_WORLD_5_END_BOSS;
		}
		else if (level == 10)
		{
			return RENDERER_TYPE_WORLD_5_MID_BOSS;
		}
		else
		{
			return RENDERER_TYPE_WORLD_5;
		}
	}

	return RENDERER_TYPE_NONE;
}

#endif /* defined(__nosfuratu__RendererType__) */
