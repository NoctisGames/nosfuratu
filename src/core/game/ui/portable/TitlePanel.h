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

class BigCloud;
class SmallCloud;
class Lightning;
class NosfuratuLogoLightEffect;
class NosfuratuLogo;
class CastleLightEffect;
class Castle;

class TitlePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    TitlePanel(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    ~TitlePanel();
    
    virtual void update(float deltaTime);
    
    bool isLightningStriking();
    
    BigCloud* getBigCloud();
    SmallCloud* getSmallCloud();
    Lightning* getLightning();
    NosfuratuLogoLightEffect* getNosfuratuLogoLightEffect();
    NosfuratuLogo* getNosfuratuLogo();
    CastleLightEffect* getCastleLightEffect();
    Castle* getCastle();
    
private:
    BigCloud* m_bigCloud;
    SmallCloud* m_smallCloud;
    Lightning* m_lightning;
    NosfuratuLogoLightEffect* m_nosfuratuLogoLightEffect;
    NosfuratuLogo* m_nosfuratuLogo;
    CastleLightEffect* m_castleLightEffect;
    Castle* m_castle;
    float m_fTimeBetweenStrikes;
    bool m_isLightningStriking;
};

class BigCloud : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    BigCloud(float x = CAM_WIDTH * 2, float y = CAM_HEIGHT * 0.58333333333333f, float width = CAM_WIDTH * 0.846875f, float height = CAM_HEIGHT * 0.15f);
    
    virtual void update(float deltaTime);
};

class SmallCloud : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    SmallCloud(float x = CAM_WIDTH * -2, float y = CAM_HEIGHT * 0.42222222222222f, float width = CAM_WIDTH * 0.4953125f, float height = CAM_HEIGHT * 0.13055555555556f);
    
    virtual void update(float deltaTime);
};

class Lightning : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Lightning(float x = CAM_WIDTH * 0.7046875f, float y = CAM_HEIGHT * 0.54166666666667f, float width = CAM_WIDTH * 0.2765625f, float height = CAM_HEIGHT * 0.49444444444444f);
};

class NosfuratuLogoLightEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    NosfuratuLogoLightEffect(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.68888888888889f, float width = CAM_WIDTH * 0.6828125f, float height = CAM_HEIGHT * 0.60277777777778f);
};

class NosfuratuLogo : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    NosfuratuLogo(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.68888888888889f, float width = CAM_WIDTH * 0.6828125f, float height = CAM_HEIGHT * 0.60277777777778f);
};

class CastleLightEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    CastleLightEffect(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.5f / 2, float width = CAM_WIDTH * 0.2875f, float height = CAM_HEIGHT * 0.51944444444444f);
};

class Castle : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    Castle(float x = CAM_WIDTH / 2, float y = CAM_HEIGHT * 0.5f / 2, float width = CAM_WIDTH * 0.2875f, float height = CAM_HEIGHT * 0.51944444444444f);
};

#endif /* defined(__nosfuratu__TitlePanel__) */
