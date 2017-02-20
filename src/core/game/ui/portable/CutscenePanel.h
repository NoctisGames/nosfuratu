//
//  CutscenePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CutscenePanel__
#define __nosfuratu__CutscenePanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "Color.h"
#include "RTTI.h"

#include <memory>

typedef enum
{
    CutsceneEffectType_Shadow_One,
    CutsceneEffectType_Shadow_Two,
    CutsceneEffectType_POW
} CutsceneEffectType;

class CutsceneEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static CutsceneEffect* create(CutsceneEffectType type);
    
    CutsceneEffect(float x, float y, float width, float height, CutsceneEffectType type);
    
    CutsceneEffectType getType();
    
    Color& getColor() { return m_color; }
    
private:
    CutsceneEffectType m_type;
    Color m_color;
};

class CutsceneEffectShadowOne : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectShadowOne() : CutsceneEffect(CAM_WIDTH, 0, CAM_WIDTH * 0.60375f, CAM_HEIGHT * 0.56f, CutsceneEffectType_Shadow_One)
    {
        m_position.add(m_fWidth / 2, 0);
        m_position.sub(0, m_fHeight * 1.68f);
        
        m_velocity.set(-3, 3);
    }
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
        
        m_fWidth += deltaTime * 1.6f;
        m_fHeight += deltaTime * 1.6f;
    }
};

class CutsceneEffectShadowTwo : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectShadowTwo() : CutsceneEffect(CAM_WIDTH / 2, 0, CAM_WIDTH, CAM_HEIGHT * 0.84444444444444f, CutsceneEffectType_Shadow_Two)
    {
        m_velocity.set(-1.2f, CAM_HEIGHT * 0.84444444444444f / 4);
    }
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
        
        m_fWidth += deltaTime * 3;
        m_fHeight += deltaTime * 2;
    }
};

class CutsceneEffectPOW : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectPOW() : CutsceneEffect(CAM_WIDTH * 0.44f, CAM_HEIGHT * 0.56f, CAM_WIDTH * 1.22f, CAM_HEIGHT * 1.56888888888889f, CutsceneEffectType_POW) {}
    
    virtual void update(float deltaTime)
    {
        PhysicalEntity::update(deltaTime);
        
        m_fWidth += deltaTime * 2.4f;
        m_fHeight += deltaTime * 2.4f;
    }
};

typedef enum
{
    CutscenePanelType_Opening_None,
    CutscenePanelType_Opening_One,
    CutscenePanelType_Opening_Two,
    CutscenePanelType_Opening_Three,
    CutscenePanelType_Opening_Four,
    CutscenePanelType_Opening_Five,
    CutscenePanelType_Opening_Six,
    CutscenePanelType_Opening_Seven
} CutscenePanelType;

class CutscenePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    CutscenePanel(CutscenePanelType type, float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
	virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel() { return nullptr; }
    
    std::vector<CutsceneEffect*>& getCutsceneEffects() { return m_cutsceneEffects; }
    
    CutscenePanelType getType() { return m_type; }
    
    Color& getColor() { return m_color; }
    
    NGRect& getCamBounds() { return *m_camBounds; }
    
    bool isReadyForNextPanel() { return m_isReadyForNextPanel; }
    
    void onCleanUp();
    
protected:
    std::unique_ptr<NGRect> m_camBounds;
    std::vector<CutsceneEffect*> m_cutsceneEffects;
    CutscenePanelType m_type;
    Color m_color;
    bool m_isReadyForNextPanel;
};

class CutscenePanelOpeningSeven : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningSeven() : CutscenePanel(CutscenePanelType_Opening_Seven)
    {
        m_color.alpha = 0;
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        if (m_fStateTime > 4.8f)
        {
            m_isReadyForNextPanel = true;
        }
        else if (m_fStateTime > 2)
        {
            m_color.alpha += deltaTime / 2;
            
            if (m_color.alpha > 1)
            {
                m_color.alpha = 1;
            }
        }
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return nullptr;
    }
};

class CutscenePanelOpeningSix : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningSix() : CutscenePanel(CutscenePanelType_Opening_Six, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3)
    {
        m_color.alpha = 0;

		float camL = CAM_WIDTH / 4;
		float camB = CAM_HEIGHT / 4;
		float camW = CAM_WIDTH / 2;
		float camH = CAM_HEIGHT / 2;

		m_camBounds->getLowerLeft().set(camL, camB);
		m_camBounds->setWidth(camL + camW);
		m_camBounds->setHeight(camB + camH);
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        float camL = CAM_WIDTH / 4 - m_fStateTime / 3.6f * CAM_WIDTH / 4;
        float camB = CAM_HEIGHT / 4 - m_fStateTime / 3.6f * CAM_HEIGHT / 4;
        float camW = CAM_WIDTH / 2 + m_fStateTime / 3.6f * CAM_WIDTH / 2;
        float camH = CAM_HEIGHT / 2 + m_fStateTime / 3.6f * CAM_HEIGHT / 2;
        
        if (m_fStateTime > 4.4f)
        {
            m_isReadyForNextPanel = true;
        }
        
        if (m_fStateTime > 3.6f)
        {
            camL = 0;
            camB = 0;
            camW = CAM_WIDTH;
            camH = CAM_HEIGHT;
            
            m_color.alpha -= deltaTime * 1.25f;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
            }
        }
        else
        {
            m_color.alpha += deltaTime / 2;
            
            if (m_color.alpha > 1)
            {
                m_color.alpha = 1;
            }
        }
        
        m_camBounds->getLowerLeft().set(camL, camB);
        m_camBounds->setWidth(camL + camW);
        m_camBounds->setHeight(camB + camH);
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return new CutscenePanelOpeningSeven();
    }
};

class CutscenePanelOpeningFive : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningFive() : CutscenePanel(CutscenePanelType_Opening_Five)
    {
        m_color.alpha = 0.5f;

		float camL = 0;
		float camB = 0;
		float camW = CAM_WIDTH;
		float camH = CAM_HEIGHT;

		m_camBounds->getLowerLeft().set(camL, camB);
		m_camBounds->setWidth(camL + camW);
		m_camBounds->setHeight(camB + camH);
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        float camL = 0;
        float camB = 0;
        float camW = CAM_WIDTH - m_fStateTime / 3 * CAM_WIDTH * 1 / 3;
        float camH = CAM_HEIGHT - m_fStateTime / 3 * CAM_HEIGHT * 1 / 3;
        
        if (m_fStateTime > 3)
        {
            camL = 0;
            camB = 0;
            camW = CAM_WIDTH * 2 / 3;
            camH = CAM_HEIGHT * 2 / 3;
        }
        else if (m_fStateTime > 2.5f)
        {
            m_isReadyForNextPanel = true;
            
            m_color.alpha -= deltaTime / 2;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
            }
        }
        else
        {
            m_color.alpha += deltaTime / 2;
            
            if (m_color.alpha > 1)
            {
                m_color.alpha = 1;
            }
        }
        
        m_camBounds->getLowerLeft().set(camL, camB);
        m_camBounds->setWidth(camL + camW);
        m_camBounds->setHeight(camB + camH);
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return new CutscenePanelOpeningSix();
    }
    
private:
    bool m_hasSpawnedPow;
};

class CutscenePanelOpeningFour : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningFour() : CutscenePanel(CutscenePanelType_Opening_Four)
	{
		float camL = 0;
		float camB = 0;
		float camW = CAM_WIDTH;
		float camH = CAM_HEIGHT;

		m_camBounds->getLowerLeft().set(camL, camB);
		m_camBounds->setWidth(camL + camW);
		m_camBounds->setHeight(camB + camH);
	}
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        float camL = m_fStateTime / 1.5f * CAM_WIDTH / 16;
        float camB = m_fStateTime / 1.5f * CAM_HEIGHT / 16;
        float camW = CAM_WIDTH - m_fStateTime / 1.5f * CAM_WIDTH * 1 / 8;
        float camH = CAM_HEIGHT - m_fStateTime / 1.5f * CAM_HEIGHT * 1 / 8;
        
        if (m_fStateTime > 1.5f)
        {
            camL = CAM_WIDTH / 16;
            camB = CAM_HEIGHT / 16;
            camW = CAM_WIDTH * 7 / 8;
            camH = CAM_HEIGHT * 7 / 8;
            
            m_color.alpha = 0;
        }
        else if (m_fStateTime > 1.25f)
        {
            m_isReadyForNextPanel = true;
        }
        
        if (m_fStateTime > 0.75f)
        {
            m_color.alpha -= deltaTime * 1.33333333333333f;
            
            if (m_color.alpha < 0)
            {
                m_color.alpha = 0;
            }
        }
        
        m_camBounds->getLowerLeft().set(camL, camB);
        m_camBounds->setWidth(camL + camW);
        m_camBounds->setHeight(camB + camH);
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return new CutscenePanelOpeningFive();
    }
};

class CutscenePanelOpeningThree : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningThree() : CutscenePanel(CutscenePanelType_Opening_Three), m_hasSpawnedPow(false)
    {
        m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_Two));
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        if (m_fStateTime > 2)
        {
            m_isReadyForNextPanel = true;
        }
        else if (m_fStateTime > 1 && !m_hasSpawnedPow)
        {
            m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_POW));
            
            m_hasSpawnedPow = true;
        }
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return new CutscenePanelOpeningFour();
    }
    
private:
    bool m_hasSpawnedPow;
};

class CutscenePanelOpeningTwo : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningTwo(float shadowX, float shadowY, float shadowWidth, float shadowHeight) : CutscenePanel(CutscenePanelType_Opening_Two)
    {
        m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_One));
        
        m_cutsceneEffects.at(0)->getPosition().set(shadowX, shadowY);
        m_cutsceneEffects.at(0)->setWidth(shadowWidth);
        m_cutsceneEffects.at(0)->setHeight(shadowHeight);
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        if (m_fStateTime > 0.8f)
        {
            m_isReadyForNextPanel = true;
        }
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        return new CutscenePanelOpeningThree();
    }
};

class CutscenePanelOpeningOne : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningOne() : CutscenePanel(CutscenePanelType_Opening_One, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3), shadowX(0), shadowY(0), shadowW(0), shadowH(0), m_hasSpawnedShadow(false)
    {
		float baseCamB = CAM_HEIGHT * 0.83333333333333f;

		float camL = 0;
		float camB = baseCamB;
		float camW = CAM_WIDTH / 2;
		float camH = CAM_HEIGHT / 2;

		m_camBounds->getLowerLeft().set(camL, camB);
		m_camBounds->setWidth(camL + camW);
		m_camBounds->setHeight(camB + camH);
        
        m_color.alpha = 0;
    }
    
    virtual void update(float deltaTime)
    {
        CutscenePanel::update(deltaTime);
        
        m_color.alpha += deltaTime;
        
        if (m_color.alpha > 1)
        {
            m_color.alpha = 1;
        }
        
        float baseCamB = CAM_HEIGHT * 0.83333333333333f;
        
        float camL = 0;
        float camB = baseCamB - m_fStateTime / 6 * baseCamB;
        float camW = CAM_WIDTH / 2 * m_fStateTime / 6 + CAM_WIDTH / 2;
        float camH = CAM_HEIGHT / 2 * m_fStateTime / 6 + CAM_HEIGHT / 2;
        
        if (m_fStateTime > 6.5f)
        {
            if (m_cutsceneEffects.size() > 0)
            {
                shadowX = m_cutsceneEffects.at(0)->getPosition().getX();
                shadowY = m_cutsceneEffects.at(0)->getPosition().getY();
                shadowW = m_cutsceneEffects.at(0)->getWidth();
                shadowH = m_cutsceneEffects.at(0)->getHeight();
            }
            
            m_isReadyForNextPanel = true;
        }
        
        if (m_fStateTime > 6)
        {
            camL = 0;
            camB = 0;
            camW = CAM_WIDTH;
            camH = CAM_HEIGHT;
        }
        else if (m_fStateTime > 4.5f && !m_hasSpawnedShadow)
        {
            m_cutsceneEffects.push_back(CutsceneEffect::create(CutsceneEffectType_Shadow_One));
            
            m_hasSpawnedShadow = true;
        }
        
        m_camBounds->getLowerLeft().set(camL, camB);
        m_camBounds->setWidth(camL + camW);
        m_camBounds->setHeight(camB + camH);
    }
    
    virtual CutscenePanel* getNextPanel()
    {
        CutscenePanelOpeningTwo* ret = new CutscenePanelOpeningTwo(shadowX, shadowY, shadowW, shadowH);
        
        return ret;
    }
    
private:
    float shadowX;
    float shadowY;
    float shadowW;
    float shadowH;
    bool m_hasSpawnedShadow;
};

#endif /* defined(__nosfuratu__CutscenePanel__) */
