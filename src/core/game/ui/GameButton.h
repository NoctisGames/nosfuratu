//
//  GameButton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameButton__
#define __nosfuratu__GameButton__

#include "PhysicalEntity.h"

#include "Color.h"

typedef enum
{
    GameButtonType_BackToTitle,
    GameButtonType_Leaderboards,
    GameButtonType_ViewOpeningCutscene,
    GameButtonType_BackToLevelSelect,
    GameButtonType_LevelEditor
} GameButtonType;

class GameButton : public PhysicalEntity
{
public:
    static GameButton* create(GameButtonType type);
    
    GameButton(float x, float y, float width, float height, GameButtonType type);
    
    GameButtonType getType();
    
    Color& getColor() { return m_color; }
    
private:
    GameButtonType m_type;
    Color m_color;
};

#endif /* defined(__nosfuratu__GameButton__) */
