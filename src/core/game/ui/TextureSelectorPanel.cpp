//
//  TextureSelectorPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "TextureSelectorPanel.h"

#include "ScreenEvent.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "TextureWrapper.h"
#include "Text.h"

#include "OverlapTester.h"

TextureSelectorPanel::TextureSelectorPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false), m_selectedTextureWrapper(nullptr), m_iSelectedTextureWrapperIndex(-1)
{
    float l = x - width / 2;
    float b = y - height / 2;
    
    m_toggleTextureButton = new NGRect(l + width * 0.07647058823529f, b + height * 0.36274509803922f, width * 0.8f, height * 0.3921568627451f);
    m_confirmButton = new NGRect(l + width * 0.12941176470588f, b + height * 0.07843137254902f, width * 0.74117647058824f, height * 0.23921568627451f);
    
    static float fgWidth = CAM_WIDTH / 100;
    static float fgHeight = fgWidth * 1.171875f;
    
    m_text = new Text("", m_toggleTextureButton->getLeft() + m_toggleTextureButton->getWidth() / 18, m_toggleTextureButton->getBottom() + m_toggleTextureButton->getHeight() / 2, fgWidth, fgHeight, 1, 1, 1, 1);
}

TextureSelectorPanel::~TextureSelectorPanel()
{
    delete m_toggleTextureButton;
    delete m_confirmButton;
    delete m_text;
}

void TextureSelectorPanel::open()
{
    m_isOpen = true;
}

void TextureSelectorPanel::close()
{
    m_isOpen = false;
}

int TextureSelectorPanel::handleTouch(ScreenEvent& te, Vector2D& touchPoint, std::vector<TextureWrapper *>& textureWrappers)
{
    if (te.getType() == ScreenEventType_UP)
    {
        if (OverlapTester::isPointInNGRect(touchPoint, *m_toggleTextureButton))
        {
            m_iSelectedTextureWrapperIndex++;
            
            if (m_iSelectedTextureWrapperIndex >= textureWrappers.size())
            {
                m_iSelectedTextureWrapperIndex = 0;
            }
            
            m_selectedTextureWrapper = textureWrappers.at(m_iSelectedTextureWrapperIndex);
            
            m_text->setText(m_selectedTextureWrapper->name);
            
            return TEXTURE_SELECTOR_PANEL_RC_HANDLED;
        }
        else if (OverlapTester::isPointInNGRect(touchPoint, *m_confirmButton))
        {
            m_isOpen = false;
            return TEXTURE_SELECTOR_PANEL_RC_CONFIRM;
        }
    }
    
    return TEXTURE_SELECTOR_PANEL_RC_UNHANDLED;
}

bool TextureSelectorPanel::isOpen()
{
    return m_isOpen;
}

Text& TextureSelectorPanel::getText()
{
    return *m_text;
}

TextureWrapper* TextureSelectorPanel::getSelectedTexture()
{
    return m_selectedTextureWrapper;
}

RTTI_IMPL(TextureSelectorPanel, PhysicalEntity);
