//
//  LevelEditorButton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelEditorButton__
#define __nosfuratu__LevelEditorButton__

#include "PhysicalEntity.h"
#include "GameConstants.h"

class LevelEditorButton : public PhysicalEntity
{
public:
    LevelEditorButton(float x = CAM_WIDTH - 1, float y = CAM_HEIGHT - 0.6456424857638f / 2, float width = 2, float height = 0.6456424857638f);
};

#endif /* defined(__nosfuratu__LevelEditorButton__) */
