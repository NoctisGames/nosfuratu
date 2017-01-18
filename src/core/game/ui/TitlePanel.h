//
//  TitlePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/8/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__TitlePanel__
#define __nosfuratu__TitlePanel__

#include "PhysicalEntity.h"
#include "GameConstants.h"
#include "RTTI.h"

class BigCloud : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    BigCloud(float x = CAM_WIDTH * 2, float y = CAM_HEIGHT * 0.58333333333333f, float width = CAM_WIDTH * 0.846875f, float height = CAM_HEIGHT * 0.15f) : PhysicalEntity(x, y, width, height)
    {
        m_velocity->setX(-1.5f);
    }
    
    virtual void update(float deltaTime);
};

class SmallCloud : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SmallCloud(float x = CAM_WIDTH * -2, float y = CAM_HEIGHT * 0.42222222222222f, float width = CAM_WIDTH * 0.4953125f, float height = CAM_HEIGHT * 0.13055555555556f) : PhysicalEntity(x, y, width, height)
    {
        m_velocity->setX(2.5f);
    }
    
    virtual void update(float deltaTime);
};

class Lightning : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Lightning(float x = CAM_WIDTH * 0.7046875f, float y = CAM_HEIGHT * 0.54166666666667f, float width = CAM_WIDTH * 0.2765625f, float height = CAM_HEIGHT * 0.49444444444444f) : PhysicalEntity(x, y, width, height) {}
};

class NosfuratuLogoLightEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    NosfuratuLogoLightEffect(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.68888888888889f, float width = CAM_WIDTH * 0.6828125f, float height = CAM_HEIGHT * 0.60277777777778f) : PhysicalEntity(x, y, width, height) {}
};

class NosfuratuLogo : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    NosfuratuLogo(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.68888888888889f, float width = CAM_WIDTH * 0.6828125f, float height = CAM_HEIGHT * 0.60277777777778f) : PhysicalEntity(x, y, width, height) {}
};

class CastleLightEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    CastleLightEffect(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.5f / 2, float width = CAM_WIDTH * 0.2875f, float height = CAM_HEIGHT * 0.51944444444444f) : PhysicalEntity(x, y, width, height) {}
};

class Castle : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Castle(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.5f / 2, float width = CAM_WIDTH * 0.2875f, float height = CAM_HEIGHT * 0.51944444444444f) : PhysicalEntity(x, y, width, height) {}
};

class TitlePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    TitlePanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual void update(float deltaTime);
    
    bool isLightningStriking();
    
    BigCloud* getBigCloud() { return m_bigCloud.get(); }
    SmallCloud* getSmallCloud() { return m_smallCloud.get(); }
    Lightning* getLightning() { return m_lightning.get(); }
    NosfuratuLogoLightEffect* getNosfuratuLogoLightEffect() { return m_nosfuratuLogoLightEffect.get(); }
    NosfuratuLogo* getNosfuratuLogo() { return m_nosfuratuLogo.get(); }
    CastleLightEffect* getCastleLightEffect() { return m_castleLightEffect.get(); }
    Castle* getCastle() { return m_castle.get(); }
    
private:
    std::unique_ptr<BigCloud> m_bigCloud;
    std::unique_ptr<SmallCloud> m_smallCloud;
    std::unique_ptr<Lightning> m_lightning;
    std::unique_ptr<NosfuratuLogoLightEffect> m_nosfuratuLogoLightEffect;
    std::unique_ptr<NosfuratuLogo> m_nosfuratuLogo;
    std::unique_ptr<CastleLightEffect> m_castleLightEffect;
    std::unique_ptr<Castle> m_castle;
    float m_fTimeBetweenStrikes;
    bool m_isLightningStriking;
};

#endif /* defined(__nosfuratu__TitlePanel__) */
