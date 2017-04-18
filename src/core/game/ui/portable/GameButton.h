//
//  GameButton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/2/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameButton__
#define __nosfuratu__GameButton__

#include "PhysicalEntity.h"

#include "GameButtonType.h"
#include "Color.h"

#include "RTTI.h"

class GameButton : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static GameButton* create(GameButtonType type);
    
    GameButton(float x, float y, float width, float height, GameButtonType type);
    
    virtual void update(float deltaTime);
    
    GameButtonType getType();
    
    Color& getColor();
    
    bool handleClick(Vector2D& touchPoint);
    
    void select();
    
    void deselect();
    
    void click();
    
    bool isSelected();
    
    bool isHidden();
    
    void setHidden(bool isHidden);
    
    bool isAlt();
    
    void setAlt(bool isAlt);
    
private:
    GameButtonType m_type;
    Color m_color;
    float m_fOriginalWidth;
    float m_fOriginalHeight;
    bool m_isSelected;
    bool m_isShrinking;
    bool m_isHidden;
    bool m_isAlt;
};

#endif /* defined(__nosfuratu__GameButton__) */
