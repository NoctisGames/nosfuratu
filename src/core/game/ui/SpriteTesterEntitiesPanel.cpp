//
//  SpriteTesterEntitiesPanel.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 11/8/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "SpriteTesterEntitiesPanel.h"

#include "Rectangle.h"
#include "Vector2D.h"
#include "GameConstants.h"
#include "Game.h"
#include "EntityUtils.h"
#include "UnknownEntity.h"
#include "AssetsMapper.h"

SpriteTesterEntitiesPanel::SpriteTesterEntitiesPanel(float x, float y, float width, float height) : PhysicalEntity(x, y, width, height), m_isOpen(false), m_fEntitiesCameraPos(0), m_fEntitiesHeight(0)
{
    m_openButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - 1, height * 0.49081920903955f, 1, 1));
    m_closeButton = std::unique_ptr<Rectangle>(new Rectangle(CAM_WIDTH - width, height * 0.49081920903955f, 1, 1));
    m_touchPointDown = std::unique_ptr<Vector2D>(new Vector2D());
    m_touchPointDown2 = std::unique_ptr<Vector2D>(new Vector2D());
}

void SpriteTesterEntitiesPanel::initForTextureName(std::string textureName)
{
    EntityUtils::cleanUpVectorOfPointers(m_entities);
    
    std::map<std::string, TextureRegion*>& trMap = ASSETS_MAPPER->getTextureRegionMap();
    
    for (std::map<std::string, TextureRegion*>::iterator i = trMap.begin(); i != trMap.end(); i++)
    {
        if (textureName.compare(i->second->getTextureName()) == 0)
        {
            m_entities.push_back(UnknownEntity::create(i->first, i->second->getTextureName()));
        }
    }
    
    std::map<std::string, Animation*>& animMap = ASSETS_MAPPER->getAnimationsMap();
    
    for (std::map<std::string, Animation*>::iterator i = animMap.begin(); i != animMap.end(); i++)
    {
        if (textureName.compare(i->second->getTextureName()) == 0)
        {
            m_entities.push_back(UnknownEntity::create(i->first, i->second->getTextureName()));
        }
    }
    
    float eWidth = m_fWidth * 0.6f;
    float eHeight = m_fHeight / 6;
    float eX = CAM_WIDTH - m_fWidth / 2 + 0.4f;
    float eY = eHeight / 2;
    
    int i = EntityUtils::boxInAll(m_entities, eX, eY, eWidth, eHeight, 0);
    
    m_fEntitiesHeight = fmaxf((i * eHeight), m_fHeight);
    
    if (m_fEntitiesCameraPos > (m_fEntitiesHeight - getHeight()))
    {
        m_fEntitiesCameraPos = m_fEntitiesHeight - getHeight();
    }
}

int SpriteTesterEntitiesPanel::handleTouch(TouchEvent& te, Vector2D& touchPoint, Vector2D& camPos, std::vector<UnknownEntity*>& gameItems, UnknownEntity** lastAddedEntity)
{
    if (m_isOpen && touchPoint.getX() > (CAM_WIDTH - getWidth()))
    {
        switch (te.getTouchType())
        {
            case DOWN:
            {
                if(touchPoint.getX() > m_closeButton->getLowerLeft().getX() + m_closeButton->getWidth())
                {
                    m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                    m_touchPointDown2->set(touchPoint.getX(), touchPoint.getY());
                    
                    return SPRITE_TESTER_ENTITIES_PANEL_RC_HANDLED;
                }
            }
                break;
            case DRAGGED:
                m_fEntitiesCameraPos += (m_touchPointDown->getY() - touchPoint.getY()) * 3;
                if (m_fEntitiesCameraPos < 0)
                {
                    m_fEntitiesCameraPos = 0;
                }
                else if (m_fEntitiesCameraPos > (m_fEntitiesHeight - getHeight()))
                {
                    m_fEntitiesCameraPos = m_fEntitiesHeight - getHeight();
                }
                
                m_touchPointDown->set(touchPoint.getX(), touchPoint.getY());
                
                return SPRITE_TESTER_ENTITIES_PANEL_RC_HANDLED;
            case UP:
            {
                if (OverlapTester::isPointInRectangle(touchPoint, *m_closeButton))
                {
                    m_position->setX(CAM_WIDTH + (3.76608187134503f - 1.894736842105263f) / 2);
                    
                    m_isOpen = false;
                    
                    return SPRITE_TESTER_ENTITIES_PANEL_RC_HANDLED;
                }
                else if (touchPoint.getY() < m_touchPointDown2->getY() + 0.25f && touchPoint.getY() > m_touchPointDown2->getY() - 0.25f)
                {
                    touchPoint.add(0, m_fEntitiesCameraPos);
                    
                    float x = camPos.getX() + ZOOMED_OUT_CAM_WIDTH / 2;
                    float y = GAME_HEIGHT / 2;
                    
                    if (isTouchingEntityForPlacement(m_entities, gameItems, x, y, lastAddedEntity, touchPoint))
                    {
                        return SPRITE_TESTER_ENTITIES_PANEL_RC_ENTITY_ADDED;
                    }
                }
            }
        }
    }
    else
    {
		if (OverlapTester::isPointInRectangle(touchPoint, *m_openButton))
		{
			if (te.getTouchType() == UP)
			{
				m_position->setX(CAM_WIDTH - getWidth() / 2);

				m_isOpen = true;
			}

			return SPRITE_TESTER_ENTITIES_PANEL_RC_HANDLED;
		}
    }
    
    return SPRITE_TESTER_ENTITIES_PANEL_RC_UNHANDLED;
}

bool SpriteTesterEntitiesPanel::isTouchingEntityForPlacement(std::vector<UnknownEntity*>& items, std::vector<UnknownEntity*>& gameItems, float x, float y, UnknownEntity** lastAddedEntity, Vector2D& touchPoint)
{
    int retVal = -1;
    int index = 0;
    for (typename std::vector<UnknownEntity*>::iterator i = items.begin(); i != items.end(); i++, index++)
    {
        UnknownEntity* item = *i;
        float width = item->getWidth();
        float height = item->getHeight();
        float x = item->getPosition().getX() - width / 2;
        float y = item->getPosition().getY() - height / 2;
        
        Rectangle tempBounds = Rectangle(x, y, width, height);
        if (OverlapTester::isPointInRectangle(touchPoint, tempBounds))
        {
            retVal = index;
            break;
        }
    }
    
    if (retVal != -1)
    {
        UnknownEntity* pT = UnknownEntity::create(items.at(index)->getAssetId(), items.at(index)->getTextureName(), x, y);
        gameItems.push_back(pT);
        
        *lastAddedEntity = pT;
    }
    
    return retVal != -1;
}

std::vector<UnknownEntity *>& SpriteTesterEntitiesPanel::getEntities()
{
    return m_entities;
}

float SpriteTesterEntitiesPanel::getEntitiesCameraPos()
{
    return m_fEntitiesCameraPos;
}

bool SpriteTesterEntitiesPanel::isOpen()
{
    return m_isOpen;
}

RTTI_IMPL(SpriteTesterEntitiesPanel, PhysicalEntity);
