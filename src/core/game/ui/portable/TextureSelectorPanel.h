//
//  TextureSelectorPanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__TextureSelectorPanel__
#define __nosfuratu__TextureSelectorPanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

#include <vector>

class ScreenEvent;
class Vector2D;
class NGRect;
class TextureWrapper;
class Text;

#define TEXTURE_SELECTOR_PANEL_RC_UNHANDLED -1
#define TEXTURE_SELECTOR_PANEL_RC_HANDLED 0
#define TEXTURE_SELECTOR_PANEL_RC_CONFIRM 1

class TextureSelectorPanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    TextureSelectorPanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH / 3, float height = CAM_HEIGHT / 3);
    
    virtual ~TextureSelectorPanel();
    
    void open();
    
    void close();
    
    int handleTouch(ScreenEvent& te, Vector2D& touchPoint, std::vector<TextureWrapper *>& textureWrappers);
    
    bool isOpen();
    
    Text& getText();
    
    TextureWrapper* getSelectedTexture();
    
private:
    NGRect* m_toggleTextureButton;
    NGRect* m_confirmButton;
    Text* m_text;
    TextureWrapper* m_selectedTextureWrapper;
    int m_iSelectedTextureWrapperIndex;
    bool m_isOpen;
};

#endif /* defined(__nosfuratu__TextureSelectorPanel__) */
