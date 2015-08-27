//
//  Renderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Renderer.h"
#include "ResourceConstants.h"
#include "SpriteBatcher.h"
#include "TextureRegion.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "Vector2D.h"
#include "Rectangle.h"

Renderer::Renderer()
{
    // TODO
}

void Renderer::renderBackground()
{
    m_spriteBatcher->beginBatch();
    
    static PhysicalEntity go = PhysicalEntity(GAME_WIDTH / 2, GAME_HEIGHT / 2, GAME_WIDTH, GAME_HEIGHT, 0);
    
    renderPhysicalEntity(go, Assets::getBackground());
    
    m_spriteBatcher->endBatch(*m_backgroundTexture);
}

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}