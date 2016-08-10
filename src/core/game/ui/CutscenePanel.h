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

typedef enum
{
    CutsceneEffectType_Shadow_One,
    CutsceneEffectType_Shadow_Two,
    CutsceneEffectType_POW
} CutsceneEffectType;

class CutsceneEffect : public PhysicalEntity
{
public:
    static CutsceneEffect* create(CutsceneEffectType type);
    
    CutsceneEffect(float x, float y, float width, float height, CutsceneEffectType type);
    
    virtual void update(float deltaTime);
    
    CutsceneEffectType getType();
    
private:
    CutsceneEffectType m_type;
};

class CutsceneEffectShadowOne : public CutsceneEffect
{
public:
    CutsceneEffectShadowOne() : CutsceneEffect(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH * 0.60375f, CAM_HEIGHT * 0.56f, CutsceneEffectType_Shadow_One) {}
    
    virtual void update(float deltaTime);
};

class CutsceneEffectShadowTwo : public CutsceneEffect
{
public:
    CutsceneEffectShadowTwo() : CutsceneEffect(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT * 0.84444444444444f, CutsceneEffectType_Shadow_Two) {}
    
    virtual void update(float deltaTime);
};

class CutsceneEffectPOW : public CutsceneEffect
{
public:
    CutsceneEffectPOW() : CutsceneEffect(CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH * 1.22f, CAM_HEIGHT * 1.56888888888889f, CutsceneEffectType_POW) {}
    
    virtual void update(float deltaTime);
};

typedef enum
{
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
public:
    static CutscenePanel* create(CutscenePanelType type);
    
    CutscenePanel(CutscenePanelType type, float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual void update(float deltaTime);
    
    std::vector<CutsceneEffect *>& getCutsceneEffects();
    
    CutscenePanelType getType();
    
private:
    std::vector<CutsceneEffect *> m_cutsceneEffects;
    CutscenePanelType m_type;
};

class CutscenePanelOpeningOne : public CutscenePanel
{
public:
    CutscenePanelOpeningOne() : CutscenePanel(CutscenePanelType_Opening_One, CAM_WIDTH / 2, CAM_HEIGHT * 2 / 3, CAM_WIDTH, CAM_HEIGHT * 4 / 3) {}
    
    virtual void update(float deltaTime);
};

class CutscenePanelOpeningTwo : public CutscenePanel
{
public:
    CutscenePanelOpeningTwo() : CutscenePanel(CutscenePanelType_Opening_Two) {}
};

class CutscenePanelOpeningSix : public CutscenePanel
{
public:
    CutscenePanelOpeningSix() : CutscenePanel(CutscenePanelType_Opening_Six, CAM_WIDTH / 2, CAM_HEIGHT / 2, CAM_WIDTH, CAM_HEIGHT * 4 / 3) {}
    
    virtual void update(float deltaTime);
};

#endif /* defined(__nosfuratu__CutscenePanel__) */
