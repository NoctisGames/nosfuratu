//
//  CutscenePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 8/2/16.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__CutscenePanel__
#define __nosfuratu__CutscenePanel__

#include "PhysicalEntity.h"

#include "Color.h"
#include "GameConstants.h"
#include "CutsceneEffectType.h"
#include "CutscenePanelType.h"

#include "RTTI.h"

class NGRect;

class CutscenePanelOpeningOne;
class CutscenePanelOpeningTwo;
class CutscenePanelOpeningThree;
class CutscenePanelOpeningFour;
class CutscenePanelOpeningFive;
class CutscenePanelOpeningSix;
class CutscenePanelOpeningSeven;
class CutsceneEffect;
class CutsceneEffectShadowOne;
class CutsceneEffectShadowTwo;
class CutsceneEffectPOW;

class CutscenePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    CutscenePanel(CutscenePanelType type, float x = CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH, float height = CAM_HEIGHT);
    
    virtual ~CutscenePanel();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
    
    std::vector<CutsceneEffect*>& getCutsceneEffects();
    
    CutscenePanelType getType();
    
    Color& getColor();
    
    NGRect& getCamBounds();
    
    bool isReadyForNextPanel();
    
protected:
    NGRect* m_camBounds;
    std::vector<CutsceneEffect*> m_cutsceneEffects;
    CutscenePanelType m_type;
    Color m_color;
    bool m_isReadyForNextPanel;
};

class CutscenePanelOpeningOne : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningOne();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
    
private:
    float shadowX;
    float shadowY;
    float shadowW;
    float shadowH;
    bool m_hasSpawnedShadow;
};

class CutscenePanelOpeningTwo : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningTwo(float shadowX, float shadowY, float shadowWidth, float shadowHeight);
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
};

class CutscenePanelOpeningThree : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningThree();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
    
private:
    bool m_hasSpawnedPow;
};

class CutscenePanelOpeningFour : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningFour();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
};

class CutscenePanelOpeningFive : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningFive();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
    
private:
    bool m_hasSpawnedPow;
};

class CutscenePanelOpeningSix : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningSix();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
};

class CutscenePanelOpeningSeven : public CutscenePanel
{
    RTTI_DECL;
    
public:
    CutscenePanelOpeningSeven();
    
    virtual void update(float deltaTime);
    
    virtual CutscenePanel* getNextPanel();
};

class CutsceneEffect : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    static CutsceneEffect* create(CutsceneEffectType type);
    
    CutsceneEffect(float x, float y, float width, float height, CutsceneEffectType type);
    
    CutsceneEffectType getType();
    
    Color& getColor();
    
private:
    CutsceneEffectType m_type;
    Color m_color;
};

class CutsceneEffectShadowOne : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectShadowOne();
    
    virtual void update(float deltaTime);
};

class CutsceneEffectShadowTwo : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectShadowTwo();
    
    virtual void update(float deltaTime);
};

class CutsceneEffectPOW : public CutsceneEffect
{
    RTTI_DECL;
    
public:
    CutsceneEffectPOW();
    
    virtual void update(float deltaTime);
};

#endif /* defined(__nosfuratu__CutscenePanel__) */
