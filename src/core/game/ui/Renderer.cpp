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
#include "Game.h"
#include "Ground.h"
#include "Platform.h"
#include "Carrot.h"
#include "Jon.h"

Renderer::Renderer()
{
    // TODO
}

void Renderer::render(Game& game)
{
    beginFrame();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    bindToOffscreenFramebuffer();
    
    clearFrameBufferWithColor(0, 0, 0, 1);
    
    /// Render Background
    
    updateCameraToFollowJon(game.getJon(), true);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 19.99913569576492f, CAM_WIDTH, 14.00172860847015f, 0, Assets::getSky(game.getJon()));
    m_spriteBatcher->endBatch(*m_background);
    
    m_spriteBatcher->beginBatch();
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 15.00518582541045f, CAM_WIDTH, 11.8547968885047f, 0, Assets::getBackgroundTrees(game.getJon()));
    m_spriteBatcher->drawSprite(CAM_WIDTH / 2, 10.19792566983569f, CAM_WIDTH, 2.24027657735517f, 0, Assets::getMidgroundTrees(game.getJon()));
    m_spriteBatcher->endBatch(*m_background);
    
    /// Render World
    
    updateCameraToFollowJon(game.getJon(), false);
    
    m_spriteBatcher->beginBatch();
    for (std::vector<Ground>::iterator itr = game.getGrounds().begin(); itr != game.getGrounds().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getForeground(*itr));
    }
    
    for (std::vector<Platform>::iterator itr = game.getPlatforms().begin(); itr != game.getPlatforms().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getPlatform(*itr));
    }
    
    for (std::vector<Carrot>::iterator itr = game.getCarrots().begin(); itr != game.getCarrots().end(); itr++)
    {
        renderPhysicalEntity(*itr, Assets::getCarrot(*itr));
    }
    m_spriteBatcher->endBatch(*m_background);
    
    /// Render Jon
    
    m_spriteBatcher->beginBatch();
    
    renderPhysicalEntity(game.getJon(), Assets::getJon(game.getJon()));
    
    m_spriteBatcher->endBatch(*m_jon);
    
    /// Render everything to the screen
    
    bindToScreenFramebuffer();
    
    m_spriteBatcher->beginBatch();
    
    m_spriteBatcher->drawSprite(0, 0, 2, 2, 0, TextureRegion(0, 0, 1, 1, 1, 1));
    
    m_spriteBatcher->endBatch(*m_framebuffer, getFramebufferToScreenGpuProgramWrapper());
    
    endFrame();
}

#pragma mark private

void Renderer::renderPhysicalEntity(PhysicalEntity &go, TextureRegion tr)
{
    m_spriteBatcher->drawSprite(go.getPosition().getX(), go.getPosition().getY(), go.getWidth(), go.getHeight(), go.getAngle(), tr);
}

void Renderer::updateCameraToFollowJon(Jon& jon, bool isBackground)
{
    static float defaultY = 8.86776145203123f;
    
    float x = jon.getPosition().getX() - CAM_WIDTH / 5;
    float y = jon.getPosition().getY() - 1.2f;
    if(y < 15 && y >= defaultY)
    {
        y = defaultY;
    }
    float top = y + CAM_HEIGHT;
    top = top > GAME_HEIGHT ? GAME_HEIGHT : top;
    y = top - CAM_HEIGHT;
    
    x = isBackground ? 0 : x;
    
    updateMatrix(x, x + CAM_WIDTH, y, top);
}