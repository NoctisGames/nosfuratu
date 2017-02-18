//
//  MainAssets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainAssets.h"

#include "Assets.h"
#include "TextureRegion.h"
#include "Animation.h"
#include "Vector2D.h"
#include "NGRect.h"
#include "GameConstants.h"
#include "Background.h"
#include "Midground.h"
#include "Ground.h"
#include "ExitGround.h"
#include "Hole.h"
#include "DustCloud.h"
#include "Jon.h"
#include "MidBossOwl.h"
#include "GameButton.h"
#include "TrashCan.h"
#include "LevelSelectorPanel.h"
#include "OffsetPanel.h"
#include "ConfirmResetPanel.h"
#include "ConfirmExitPanel.h"
#include "CollectibleItem.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "Enemy.h"
#include "EnemySpirit.h"
#include "ForegroundObject.h"
#include "TitlePanel.h"
#include "CutscenePanel.h"
#include "WorldMapPanel.h"
#include "MainScreenWorldMap.h"
#include "Game.h"
#include "BatPanel.h"
#include "MainScreen.h"
#include "EndBossSnake.h"
#include "TextureSelectorPanel.h"
#include "ForegroundCoverObject.h"
#include "SoundManager.h"

MainAssets* MainAssets::getInstance()
{
    static MainAssets *instance = new MainAssets();
    return instance;
}

TextureRegion& MainAssets::get(TitlePanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("TitlePanel_Background");
    return tr;
}

TextureRegion& MainAssets::get(BigCloud* bigCloud)
{
    static TextureRegion tr = ASSETS->findTextureRegion("TitlePanel_Big_Cloud");
    return tr;
}

TextureRegion& MainAssets::get(SmallCloud* smallCloud)
{
    static TextureRegion tr = ASSETS->findTextureRegion("TitlePanel_Small_Cloud");
    return tr;
}

TextureRegion& MainAssets::get(Lightning* lightning)
{
    static Animation anim = ASSETS->findAnimation("TitlePanel_Lightning");
    return anim.getTextureRegion(lightning->getStateTime());
}

TextureRegion& MainAssets::get(NosfuratuLogoLightEffect* nosfuratuLogoLightEffect)
{
    static Animation anim = ASSETS->findAnimation("TitlePanel_Logo_Light_Effect");
    return anim.getTextureRegion(nosfuratuLogoLightEffect->getStateTime());
}

TextureRegion& MainAssets::get(NosfuratuLogo* nosfuratuLogo)
{
    static TextureRegion tr = ASSETS->findTextureRegion("TitlePanel_Logo");
    return tr;
}

TextureRegion& MainAssets::get(CastleLightEffect* castleLightEffect)
{
    static Animation anim = ASSETS->findAnimation("TitlePanel_Castle_Light_Effect");
    return anim.getTextureRegion(castleLightEffect->getStateTime());
}

TextureRegion& MainAssets::get(Castle* castle)
{
    static Animation anim = ASSETS->findAnimation("TitlePanel_Castle");
    return anim.getTextureRegion(castle->getStateTime());
}

TextureRegion& MainAssets::get(CutsceneEffect* effect)
{
    switch (effect->getType())
    {
        case CutsceneEffectType_Shadow_One:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutsceneEffectType_Shadow_One");
            return tr;
        }
        case CutsceneEffectType_Shadow_Two:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutsceneEffectType_Shadow_Two");
            return tr;
        }
        case CutsceneEffectType_POW:
        {
            static Animation anim = ASSETS->findAnimation("CutsceneEffectType_POW");
            return anim.getTextureRegion(effect->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(CutscenePanel* panel)
{
    switch (panel->getType())
    {
        case CutscenePanelType_Opening_One:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_One");
            return tr;
        }
        case CutscenePanelType_Opening_Two:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Two");
            return tr;
        }
        case CutscenePanelType_Opening_Three:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Three");
            return tr;
        }
        case CutscenePanelType_Opening_Four:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Four");
            return tr;
        }
        case CutscenePanelType_Opening_Five:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Five");
            return tr;
        }
        case CutscenePanelType_Opening_Six:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Six");
            return tr;
        }
        case CutscenePanelType_Opening_Seven:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("CutscenePanelType_Opening_Seven");
            return tr;
        }
        case CutscenePanelType_Opening_None:
        default:
            assert(false);
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(WorldMapPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("WorldMapPanel");
    return tr;
}

TextureRegion& MainAssets::get(AbilitySlot* abilitySlot)
{
    if (abilitySlot->isUnlocking())
    {
        static Animation anim = ASSETS->findAnimation("AbilitySlot_Unlocking");
        return anim.getTextureRegion(abilitySlot->getStateTime());
    }
    else if (abilitySlot->isUnlocked())
    {
        switch (abilitySlot->getType())
        {
            case AbilitySlotType_Drill:
            {
                static Animation anim = ASSETS->findAnimation("AbilitySlotType_Drill");
                return anim.getTextureRegion(abilitySlot->getStateTime());
            }
            case AbilitySlotType_Dash:
            {
                static Animation anim = ASSETS->findAnimation("AbilitySlotType_Dash");
                return anim.getTextureRegion(abilitySlot->getStateTime());
            }
        }
    }
    
    static TextureRegion tr = ASSETS->findTextureRegion("AbilitySlot_Locked");
    return tr;
}

TextureRegion& MainAssets::get(LevelThumbnail* thumbnail)
{
    if (thumbnail->isBoss())
    {
        BossLevelThumbnail *bossLevelThumbnail = reinterpret_cast<BossLevelThumbnail *>(thumbnail);
        
		if (bossLevelThumbnail->isUnlocking())
        {
            static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Unlocking");
            return anim.getTextureRegion(thumbnail->getStateTime());
        }
        else if (bossLevelThumbnail->isUnlocked())
        {
            if (thumbnail->isSelected())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Selected_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Selected");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isSelecting())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Selecting_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Selecting");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isClearing())
            {
                static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Clearing");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else if (thumbnail->isCleared())
            {
                static TextureRegion tr = ASSETS->findTextureRegion("BossLevelThumbnail_Cleared");
                return tr;
            }
            else
            {
                static TextureRegion tr = ASSETS->findTextureRegion("BossLevelThumbnail_Unlocked");
                return tr;
            }
        }
        else
        {
            if (bossLevelThumbnail->isJawMoving())
            {
                static Animation anim = ASSETS->findAnimation("BossLevelThumbnail_Locked_Jaw_Moving");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else
            {
                static TextureRegion tr = ASSETS->findTextureRegion("BossLevelThumbnail_Locked");
                return tr;
            }
        }
    }
    else
    {
        if (thumbnail->isPlayable())
        {
            if (thumbnail->isSelected())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = ASSETS->findAnimation("NormalLevelThumbnail_Selected_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("NormalLevelThumbnail_Selected");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isSelecting())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = ASSETS->findAnimation("NormalLevelThumbnail_Selecting_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("NormalLevelThumbnail_Selecting");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isClearing())
            {
                static Animation anim = ASSETS->findAnimation("NormalLevelThumbnail_Clearing");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else if (thumbnail->isCleared())
            {
                static TextureRegion tr = ASSETS->findTextureRegion("NormalLevelThumbnail_Cleared");
                return tr;
            }
            else
            {
                static TextureRegion tr = ASSETS->findTextureRegion("NormalLevelThumbnail_Playable");
                return tr;
            }
        }
        else
        {
            static TextureRegion tr = ASSETS->findTextureRegion("NormalLevelThumbnail_Unplayable");
            return tr;
        }
    }
}

TextureRegion& MainAssets::get(GoldenCarrotsMarker* goldenCarrotsMarker)
{
    switch (goldenCarrotsMarker->getNumGoldenCarrots())
    {
        case 0:
        {
            static Animation anim = ASSETS->findAnimation("GoldenCarrotsMarker_0");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 1:
        {
            static Animation anim = ASSETS->findAnimation("GoldenCarrotsMarker_1");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 2:
        {
            static Animation anim = ASSETS->findAnimation("GoldenCarrotsMarker_2");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 3:
        {
            static Animation anim = ASSETS->findAnimation("GoldenCarrotsMarker_3");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 4:
        {
            static Animation anim = ASSETS->findAnimation("GoldenCarrotsMarker_4");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(SpendGoldenCarrotsBubble* spendGoldenCarrotsBubble)
{
    if (spendGoldenCarrotsBubble->userHasEnoughGoldenCats())
    {
        static Animation anim = ASSETS->findAnimation("SpendGoldenCarrotsBubble_Has_Enough");
        return anim.getTextureRegion(spendGoldenCarrotsBubble->getStateTime());
    }
    
    static Animation anim = ASSETS->findAnimation("SpendGoldenCarrotsBubble_Not_Enough");
    return anim.getTextureRegion(spendGoldenCarrotsBubble->getStateTime());
}

TextureRegion& MainAssets::get(Background* background)
{
    switch (background->getType())
    {
        case BackgroundType_Upper:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Upper");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Mid_Hills:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Mid_Hills");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Lower_Innermost:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Lower_Innermost");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Mid_Trees:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Mid_Trees");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Lower_Inner:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Lower_Inner");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Lower_Top:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Lower_Top");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_Lower_Bottom:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_Lower_Bottom");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_WaterBack:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_WaterBack");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
        case BackgroundType_WaterFront:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("BackgroundType_WaterFront");
            ASSETS->initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, 2048);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(Midground* midground)
{
    switch (midground->getType())
    {
        case MidgroundType_TreeOne:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_TreeOne");
            return tr;
        }
        case MidgroundType_TreeTwo:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_TreeTwo");
            return tr;
        }
        case MidgroundType_TreeThree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_TreeThree");
            return tr;
        }
        case MidgroundType_DeepCaveColumnSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_DeepCaveColumnSmall");
            return tr;
        }
        case MidgroundType_DeepCaveColumnMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_DeepCaveColumnMedium");
            return tr;
        }
        case MidgroundType_DeepCaveColumnBig:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_DeepCaveColumnBig");
            return tr;
        }
        case MidgroundType_Plant:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Plant");
            return tr;
        }
        case MidgroundType_Metal_Tower_Section:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Metal_Tower_Section");
            return tr;
        }
        case MidgroundType_Billboard_Count_Hiss:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Billboard_Count_Hiss");
            return tr;
        }
        case MidgroundType_Billboard_Slag_Town:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Billboard_Slag_Town");
            return tr;
        }
        case MidgroundType_Billboard_Jon_Wanted:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Billboard_Jon_Wanted");
            return tr;
        }
        case MidgroundType_Hill:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Hill");
            return tr;
        }
        case MidgroundType_Train_Car:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Train_Car");
            return tr;
        }
        case MidgroundType_Short_Stump:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Short_Stump");
            return tr;
        }
        case MidgroundType_Tall_Stump:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Tall_Stump");
            return tr;
        }
        case MidgroundType_Thin_Tall_Tree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Thin_Tall_Tree");
            return tr;
        }
        case MidgroundType_Thin_Short_Tree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Thin_Short_Tree");
            return tr;
        }
        case MidgroundType_Stone_Square:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Stone_Square");
            return tr;
        }
        case MidgroundType_Stone_Diamond:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Stone_Diamond");
            return tr;
        }
        case MidgroundType_Wall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Wall");
            return tr;
        }
        case MidgroundType_Roof:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("MidgroundType_Roof");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(Ground* ground)
{
    switch (ground->getType())
    {
        case GroundType_CaveExtraDeepEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveExtraDeepEndLeft");
            return tr;
        }
        case GroundType_CaveExtraDeepSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveExtraDeepSmall");
            return tr;
        }
        case GroundType_CaveExtraDeepMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveExtraDeepMedium");
            return tr;
        }
        case GroundType_CaveExtraDeepLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveExtraDeepLarge");
            return tr;
        }
        case GroundType_CaveExtraDeepEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveExtraDeepEndRight");
            return tr;
        }
            
        case GroundType_CaveDeepEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveDeepEndLeft");
            return tr;
        }
        case GroundType_CaveDeepSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveDeepSmall");
            return tr;
        }
        case GroundType_CaveDeepMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveDeepMedium");
            return tr;
        }
        case GroundType_CaveDeepLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveDeepLarge");
            return tr;
        }
        case GroundType_CaveDeepEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveDeepEndRight");
            return tr;
        }
            
        case GroundType_CaveEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveEndLeft");
            return tr;
        }
        case GroundType_CaveSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveSmall");
            return tr;
        }
        case GroundType_CaveMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveMedium");
            return tr;
        }
        case GroundType_CaveLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveLarge");
            return tr;
        }
        case GroundType_CaveEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveEndRight");
            return tr;
        }
            
        case GroundType_CaveRaisedEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveRaisedEndLeft");
            return tr;
        }
        case GroundType_CaveRaisedSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveRaisedSmall");
            return tr;
        }
        case GroundType_CaveRaisedMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveRaisedMedium");
            return tr;
        }
        case GroundType_CaveRaisedLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveRaisedLarge");
            return tr;
        }
        case GroundType_CaveRaisedEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_CaveRaisedEndRight");
            return tr;
        }
            
        case GroundType_GrassWithCaveEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithCaveEndLeft");
            return tr;
        }
        case GroundType_GrassWithCaveSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithCaveSmall");
            return tr;
        }
        case GroundType_GrassWithCaveMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithCaveMedium");
            return tr;
        }
        case GroundType_GrassWithCaveLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithCaveLarge");
            return tr;
        }
        case GroundType_GrassWithCaveEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithCaveEndRight");
            return tr;
        }
            
        case GroundType_GrassWithoutCaveEndLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithoutCaveEndLeft");
            return tr;
        }
        case GroundType_GrassWithoutCaveSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithoutCaveSmall");
            return tr;
        }
        case GroundType_GrassWithoutCaveMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithoutCaveMedium");
            return tr;
        }
        case GroundType_GrassWithoutCaveLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithoutCaveLarge");
            return tr;
        }
        case GroundType_GrassWithoutCaveEndRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassWithoutCaveEndRight");
            return tr;
        }
            
        case GroundType_GrassPitSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassPitSmall");
            return tr;
        }
        case GroundType_GrassPitMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassPitMedium");
            return tr;
        }
        case GroundType_GrassPitLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassPitLarge");
            return tr;
        }
        case GroundType_GrassPitExtraLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GroundType_GrassPitExtraLarge");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(ExitGround* exitGround)
{
    switch (exitGround->getType())
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ExitGroundType_GrassWithCaveSmallExitMid");
            return tr;
        }
        case ExitGroundType_GrassWithCaveSmallExitEnd:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ExitGroundType_GrassWithCaveSmallExitEnd");
            return tr;
        }
        case ExitGroundType_CaveSmallExit:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ExitGroundType_CaveSmallExit");
            return tr;
        }
            
        case ExitGroundType_CaveDeepSmallWaterfall:
        {
            static Animation anim = ASSETS->findAnimation("ExitGroundType_CaveDeepSmallWaterfall");
            return anim.getTextureRegion(exitGround->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(ExitGroundCover* exitGroundCover)
{
    switch (exitGroundCover->getType())
    {
        case ExitGroundCoverType_Grass:
        {
            static Animation anim = ASSETS->findAnimation("ExitGroundCoverType_Grass");
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
        case ExitGroundCoverType_Cave:
        {
            static Animation anim = ASSETS->findAnimation("ExitGroundCoverType_Cave");
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(Hole* hole)
{
    switch (hole->getType())
    {
        case HoleType_GrassTileLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("HoleType_GrassTileLeft");
            return tr;
        }
        case HoleType_GrassTileCenter:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("HoleType_GrassTileCenter");
            return tr;
        }
        case HoleType_GrassTileRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("HoleType_GrassTileRight");
            return tr;
        }
        case HoleType_Grass:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("HoleType_Grass");
            return tr;
        }
        case HoleType_Cave:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("HoleType_Cave");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(HoleCover* holeCover)
{
    switch (holeCover->getType())
    {
        case HoleCoverType_GrassTileLeft:
        {
            static Animation anim = ASSETS->findAnimation("HoleCoverType_GrassTileLeft");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileCenter:
        {
            static Animation anim = ASSETS->findAnimation("HoleCoverType_GrassTileCenter");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileRight:
        {
            static Animation anim = ASSETS->findAnimation("HoleCoverType_GrassTileRight");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Grass:
        {
            static Animation anim = ASSETS->findAnimation("HoleCoverType_Grass");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Cave:
        {
            static Animation anim = ASSETS->findAnimation("HoleCoverType_Cave");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(ForegroundObject* foregroundObject)
{
    switch (foregroundObject->getType())
    {
        case ForegroundObjectType_GrassPlatformLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GrassPlatformLeft");
            return tr;
        }
        case ForegroundObjectType_GrassPlatformCenter:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GrassPlatformCenter");
            return tr;
        }
        case ForegroundObjectType_GrassPlatformRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GrassPlatformRight");
            return tr;
        }
            
        case ForegroundObjectType_CavePlatformLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_CavePlatformLeft");
            return tr;
        }
        case ForegroundObjectType_CavePlatformCenter:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_CavePlatformCenter");
            return tr;
        }
        case ForegroundObjectType_CavePlatformRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_CavePlatformRight");
            return tr;
        }
            
        case ForegroundObjectType_RockLarge:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_RockLarge");
            return tr;
        }
        case ForegroundObjectType_RockMedium:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_RockMedium");
            return tr;
        }
        case ForegroundObjectType_RockSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_RockSmall");
            return tr;
        }
        case ForegroundObjectType_RockSmallCracked:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_RockSmallCracked");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_StumpBig:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_StumpBig");
            return tr;
        }
        case ForegroundObjectType_StumpSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_StumpSmall");
            return tr;
        }
            
        case ForegroundObjectType_EndSign:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_EndSign");
            return tr;
        }
            
        case ForegroundObjectType_ThornsLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_ThornsLeft");
            return tr;
        }
        case ForegroundObjectType_ThornsCenterSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_ThornsCenterSmall");
            return tr;
        }
        case ForegroundObjectType_ThornsCenterBig:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_ThornsCenterBig");
            return tr;
        }
        case ForegroundObjectType_ThornsRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_ThornsRight");
            return tr;
        }
            
        case ForegroundObjectType_LogVerticalTall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_LogVerticalTall");
            return tr;
        }
        case ForegroundObjectType_LogVerticalShort:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_LogVerticalShort");
            return tr;
        }
            
        case ForegroundObjectType_JumpSpringLightFlush:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_JumpSpringLightFlush");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringLight:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_JumpSpringLight");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringMedium:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_JumpSpringMedium");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringHeavy:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_JumpSpringHeavy");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_SpikeGrassSingle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeGrassSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeGrassFour:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeGrassFour");
            return tr;
        }
        case ForegroundObjectType_SpikeGrassEight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeGrassEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveSingle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveFour:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveFour");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveEight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveCeilingSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingFour:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveCeilingFour");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingEight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeCaveCeilingEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeWallSingle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeWallSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeWallFour:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeWallFour");
            return tr;
        }
        case ForegroundObjectType_SpikeWallEight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeWallEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeStar:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeStar");
            return tr;
        }
            
        case ForegroundObjectType_VerticalSaw:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_VerticalSaw");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_GiantTree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GiantTree");
            return tr;
        }
        case ForegroundObjectType_GiantShakingTree:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_GiantShakingTree");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_GiantPerchTree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GiantPerchTree");
            return tr;
        }
            
        case ForegroundObjectType_SpikeTower:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeTower");
            return tr;
        }
        case ForegroundObjectType_SpikeTowerBg:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikeTowerBg");
            return tr;
        }
            
        case ForegroundObjectType_SpikedBallRollingLeft:
        case ForegroundObjectType_SpikedBallRollingRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikedBallRolling");
            return tr;
        }
            
        case ForegroundObjectType_SpikedBall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_SpikedBall");
            return tr;
        }
        case ForegroundObjectType_SpikedBallChain:
        {
            static Animation anim = ASSETS->findAnimation("ForegroundObjectType_SpikedBallChain");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_MetalGrassPlatform:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_MetalGrassPlatform");
            return tr;
        }
        case ForegroundObjectType_MetalGrassPlatformLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_MetalGrassPlatformLeft");
            return tr;
        }
        case ForegroundObjectType_MetalGrassPlatformCenter:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_MetalGrassPlatformCenter");
            return tr;
        }
        case ForegroundObjectType_MetalGrassPlatformRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_MetalGrassPlatformRight");
            return tr;
        }
            
        case ForegroundObjectType_WoodPlatform:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_WoodPlatform");
            return tr;
        }
        case ForegroundObjectType_WoodBoxTop:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_WoodBoxTop");
            return tr;
        }
        case ForegroundObjectType_WoodBox:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_WoodBox");
            return tr;
        }
            
        case ForegroundObjectType_GreenThornsLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GreenThornsLeft");
            return tr;
        }
        case ForegroundObjectType_GreenThornsCenterSmall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GreenThornsCenterSmall");
            return tr;
        }
        case ForegroundObjectType_GreenThornsCenterBig:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GreenThornsCenterBig");
            return tr;
        }
        case ForegroundObjectType_GreenThornsRight:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_GreenThornsRight");
            return tr;
        }
            
        case ForegroundObjectType_Logs:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Logs");
            return tr;
        }
            
        case ForegroundObjectType_Stone_Bottom:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Stone_Bottom");
            return tr;
        }
        case ForegroundObjectType_Stone_Middle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Stone_Middle");
            return tr;
        }
        case ForegroundObjectType_Stone_Top:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Stone_Top");
            return tr;
        }
        case ForegroundObjectType_Stone_Platform:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Stone_Platform");
            return tr;
        }
            
        case ForegroundObjectType_Floating_Platform:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Floating_Platform");
            return tr;
        }
            
        case ForegroundObjectType_Stone_Square:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundObjectType_Stone_Square");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(ForegroundCoverObject* foregroundCoverObject)
{
    switch (foregroundCoverObject->getType())
    {
        case ForegroundCoverObjectType_Tree:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Tree");
            return tr;
        }
        case ForegroundCoverObjectType_Plant:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Plant");
            return tr;
        }
        case ForegroundCoverObjectType_Bush:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Bush");
            return tr;
        }
        case ForegroundCoverObjectType_Ferns:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Ferns");
            return tr;
        }
        case ForegroundCoverObjectType_Wall:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Wall");
            return tr;
        }
        case ForegroundCoverObjectType_Wall_Bottom:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Wall_Bottom");
            return tr;
        }
        case ForegroundCoverObjectType_Wall_Window:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Wall_Window");
            return tr;
        }
        case ForegroundCoverObjectType_Wall_Window_Bottom:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Wall_Window_Bottom");
            return tr;
        }
        case ForegroundCoverObjectType_Roof_Side_Left:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Roof_Side_Left");
            return tr;
        }
        case ForegroundCoverObjectType_Roof_Side_Right:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Roof_Side_Right");
            return tr;
        }
        case ForegroundCoverObjectType_Roof_Plain:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Roof_Plain");
            return tr;
        }
        case ForegroundCoverObjectType_Roof_Chimney:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("ForegroundCoverObjectType_Roof_Chimney");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(CountHissWithMina* countHissWithMina)
{
    if (countHissWithMina->isFacingLeft())
    {
		static Animation anim = ASSETS->findAnimation("CountHissWithMina_Left");
		return anim.getTextureRegion(countHissWithMina->getStateTime());
    }
    else
    {
		static Animation anim = ASSETS->findAnimation("CountHissWithMina_Right");
		return anim.getTextureRegion(countHissWithMina->getStateTime());
    }
}

TextureRegion& MainAssets::get(SnakeSpirit* snakeSpirit)
{
    static Animation anim = ASSETS->findAnimation("SnakeSpirit");
    return anim.getTextureRegion(snakeSpirit->getStateTime());
}

TextureRegion& MainAssets::get(SnakeHeadImpact* snakeHeadImpact)
{
	static Animation anim = ASSETS->findAnimation("SnakeHeadImpact");
	return anim.getTextureRegion(snakeHeadImpact->getStateTime());
}

TextureRegion& MainAssets::get(SnakeSkin* snakeSkin)
{
    static Animation anim = ASSETS->findAnimation("SnakeSkin");
    return anim.getTextureRegion(snakeSkin->getStateTime());
}

TextureRegion& MainAssets::get(SnakeEye* snakeEye)
{
    static Animation anim = ASSETS->findAnimation("SnakeEye");
    return anim.getTextureRegion(snakeEye->getStateTime());
}

TextureRegion& MainAssets::get(SnakeTonque* snakeTonque)
{
    EndBossSnake& endBossSnake = snakeTonque->getEndBossSnake();
    switch (endBossSnake.getState())
    {
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_Awakening:
        case EndBossSnakeState_OpeningMouthLeft:
		case EndBossSnakeState_OpenMouthLeft:
        case EndBossSnakeState_ChargingLeft:
        {
            static Animation anim = ASSETS->findAnimation("SnakeTonque_Left");
            return anim.getTextureRegion(snakeTonque->getStateTime());
        }
        case EndBossSnakeState_Pursuing:
		case EndBossSnakeState_Waiting:
        case EndBossSnakeState_Damaged:
        case EndBossSnakeState_OpeningMouthRight:
		case EndBossSnakeState_OpenMouthRight:
        case EndBossSnakeState_ChargingRight:
        case EndBossSnakeState_Dying:
		case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            static Animation anim = ASSETS->findAnimation("SnakeTonque_Right");
            return anim.getTextureRegion(snakeTonque->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(SnakeBody* snakeBody)
{
    EndBossSnake& endBossSnake = snakeBody->getEndBossSnake();
    switch (endBossSnake.getState())
    {
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_Awakening:
        case EndBossSnakeState_OpeningMouthLeft:
		case EndBossSnakeState_OpenMouthLeft:
        case EndBossSnakeState_ChargingLeft:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("SnakeBody_Left");
            return tr;
        }
        case EndBossSnakeState_Pursuing:
		case EndBossSnakeState_Waiting:
        case EndBossSnakeState_Damaged:
        case EndBossSnakeState_OpeningMouthRight:
		case EndBossSnakeState_OpenMouthRight:
        case EndBossSnakeState_ChargingRight:
        case EndBossSnakeState_Dying:
		case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("SnakeBody_Right");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(EndBossSnake* endBossSnake)
{
    switch (endBossSnake->getState())
    {
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_Awakening:
		{
			static TextureRegion tr = ASSETS->findTextureRegion("EndBossSnakeState_Sleeping_Awakening");
			return tr;
		}
		case EndBossSnakeState_OpeningMouthLeft:
		{
			static Animation anim = ASSETS->findAnimation("EndBossSnakeState_OpeningMouthLeft");
			return anim.getTextureRegion(endBossSnake->getStateTime());
		}
		case EndBossSnakeState_OpenMouthLeft:
        case EndBossSnakeState_ChargingLeft:
		{
			static TextureRegion tr = ASSETS->findTextureRegion("EndBossSnakeState_OpenMouthLeft_ChargingLeft");
			return tr;
		}
        case EndBossSnakeState_Pursuing:
		case EndBossSnakeState_Waiting:
		case EndBossSnakeState_Damaged:
		{
			static TextureRegion tr = ASSETS->findTextureRegion("EndBossSnakeState_Pursuing_Waiting_Damaged");
			return tr;
		}
        case EndBossSnakeState_OpeningMouthRight:
		{
			static Animation anim = ASSETS->findAnimation("EndBossSnakeState_OpeningMouthRight");
			return anim.getTextureRegion(endBossSnake->getStateTime());
		}
		case EndBossSnakeState_OpenMouthRight:
        case EndBossSnakeState_ChargingRight:
		{
			static TextureRegion tr = ASSETS->findTextureRegion("EndBossSnakeState_OpenMouthRight_ChargingRight");
			return tr;
		}
        case EndBossSnakeState_Dying:
        {
            static Animation anim = ASSETS->findAnimation("EndBossSnakeState_Dying");
            return anim.getTextureRegion(endBossSnake->getStateTime());
        }
        case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("EndBossSnakeState_DeadSpiritReleasing_Dead");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(Enemy* enemy)
{
    switch (enemy->getType())
    {
        case EnemyType_MushroomGround:
        {
            static Animation anim = ASSETS->findAnimation("EnemyType_MushroomGround");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_MushroomCeiling:
        {
            static Animation anim = ASSETS->findAnimation("EnemyType_MushroomCeiling");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_SnakeGrunt:
        {
            static Animation anim = ASSETS->findAnimation("EnemyType_SnakeGrunt");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_Sparrow:
        {
            static Animation anim = ASSETS->findAnimation("EnemyType_Sparrow");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_Toad:
        {
            Toad* toad = reinterpret_cast<Toad *>(enemy);
            
            if (toad->isDead())
            {
                if (toad->isDeadPart1())
                {
                    if (toad->isJonVampire())
                    {
                        // Vampire Part 1
                        static Animation anim = ASSETS->findAnimation("EnemyType_Toad_Dead_Part_1_Jon_Is_Vampire");
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                    else
                    {
                        // Rabbit Part 1
                        static Animation anim = ASSETS->findAnimation("EnemyType_Toad_Dead_Part_1_Jon_Is_Rabbit");
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                }
                else
                {
                    // Part 2
                    static Animation anim = ASSETS->findAnimation("EnemyType_Toad_Dead_Part_2");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (toad->isEating())
            {
                if (toad->isJonVampire())
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Toad_Eating_Jon_Is_Vampire");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Toad_Eating_Jon_Is_Rabbit");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_Toad");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_Fox:
        {
            Fox* fox = reinterpret_cast<Fox *>(enemy);
            
            if (fox->isDead())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Is_Dead");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (fox->isHitting())
            {
                if (fox->isLeft())
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Is_Hitting_Left");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Is_Hitting_Right");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (fox->isBeingHit())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Is_Being_Hit");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                if (fox->isLeft())
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Left");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = ASSETS->findAnimation("EnemyType_Fox_Right");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
        }
        case EnemyType_BigMushroomGround:
        {
            Mushroom* mushroom = reinterpret_cast<Mushroom *>(enemy);
            if (mushroom->isBouncingBack())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomGround_Bouncing_Back");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (mushroom->isBeingBouncedOn())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomGround_Bouncing_On");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomGround");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_BigMushroomCeiling:
        {
            Mushroom* mushroom = reinterpret_cast<Mushroom *>(enemy);
            if (mushroom->isBouncingBack())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomCeiling_Bouncing_Back");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (mushroom->isBeingBouncedOn())
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomCeiling_Bouncing_On");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("EnemyType_BigMushroomCeiling");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_MovingSnakeGruntV1:
        case EnemyType_MovingSnakeGruntV2:
        case EnemyType_MovingSnakeGruntV3:
        case EnemyType_MovingSnakeGruntV4:
        case EnemyType_MovingSnakeGruntV5:
        {
            MovingSnakeGrunt* snake = reinterpret_cast<MovingSnakeGrunt *>(enemy);
            if (snake->getVelocity().getY() > 0)
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt_Jumping");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->getVelocity().getY() < 0)
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt_Falling");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isPreparingToJump())
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt_About_To_Jump");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isLanding())
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt_Landing");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isPausing())
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt_Pausing");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("MovingSnakeGrunt");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(EnemySpirit* spirit)
{
    switch (spirit->getType())
    {
        case EnemySpiritType_Snake:
        {
            static Animation anim = ASSETS->findAnimation("EnemySpiritType_Snake");
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_Sparrow:
        {
            static Animation anim = ASSETS->findAnimation("EnemySpiritType_Sparrow");
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_None:
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(CollectibleItem* collectibleItem)
{
    switch (collectibleItem->getType())
    {
        case CollectibleItemType_Carrot:
        {
            if (collectibleItem->isCollected())
            {
                static Animation anim = ASSETS->findAnimation("CollectibleItemType_Carrot_Collected");
                return anim.getTextureRegion(collectibleItem->getStateTime());
            }
            else
            {
                static TextureRegion tr = ASSETS->findTextureRegion("CollectibleItemType_Carrot_Not_Collected");
                return tr;
            }
        }
        case CollectibleItemType_GoldenCarrot:
        {
            GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>(collectibleItem);
            
            if (gc->isPreviouslyCollected())
            {
                static TextureRegion tr = ASSETS->findTextureRegion("CollectibleItemType_GoldenCarrot_Previously_Collected");
                return tr;
            }
            else if (collectibleItem->isCollected())
            {
                static Animation anim = ASSETS->findAnimation("CollectibleItemType_GoldenCarrot_Collected");
                return anim.getTextureRegion(collectibleItem->getStateTime());
            }
            else
            {
                static TextureRegion tr = ASSETS->findTextureRegion("CollectibleItemType_GoldenCarrot_Not_Collected");
                return tr;
            }
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(GoldenCarrotTwinkle* goldenCarrotTwinkle)
{
    static Animation anim = ASSETS->findAnimation("GoldenCarrotTwinkle");
    return anim.getTextureRegion(goldenCarrotTwinkle->getStateTime());
}

TextureRegion& MainAssets::get(Jon* jon)
{
    if (jon->isTransformingIntoVampire())
    {
        static Animation anim = ASSETS->findAnimation("Jon_isTransformingIntoVampire");
        
        return anim.getTextureRegion(jon->getTransformStateTime());
    }
    else if (jon->isRevertingToRabbit())
    {
        static Animation anim = ASSETS->findAnimation("Jon_isRevertingToRabbit");
        
        return anim.getTextureRegion(jon->getTransformStateTime());
    }
    
    if (jon->isVampire())
    {
        static Animation deathAnim = ASSETS->findAnimation("Jon_Vampire_Death");
        static Animation pushedBackAnim = ASSETS->findAnimation("Jon_Vampire_PushedBack");
        static Animation idleAnim = ASSETS->findAnimation("Jon_Vampire_Idle");
        static Animation runningAnim = ASSETS->findAnimation("Jon_Vampire_Running");
        static Animation upwardThrustAnim = ASSETS->findAnimation("Jon_Vampire_UpwardThrust");
        static Animation dashAnim = ASSETS->findAnimation("Jon_Vampire_Dash");
        static Animation doubleJumpingAnim = ASSETS->findAnimation("Jon_Vampire_DoubleJumping");
        static Animation glidingAnim = ASSETS->findAnimation("Jon_Vampire_Gliding");
        static Animation fallingAnim = ASSETS->findAnimation("Jon_Vampire_Falling");
        static Animation landingAnim = ASSETS->findAnimation("Jon_Vampire_Landing");
        
        if (jon->getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon->getDyingStateTime());
        }
        
        switch (jon->getAbilityState())
        {
            case ABILITY_GLIDE:
                return glidingAnim.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_UPWARD_THRUST:
                return upwardThrustAnim.getTextureRegion(jon->getActionStateTime());
            case ABILITY_DASH:
                if (jon->getAbilityStateTime() > 0.5f && jon->isFalling())
                {
                    return fallingAnim.getTextureRegion(jon->getStateTime());
                }
                
                return dashAnim.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon->getActionState())
        {
            case ACTION_JUMPING:
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon->getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon->isLanding())
            {
                return landingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        else if (jon->getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon->isFalling())
            {
                return fallingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        
        if (jon->isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon->getStateTime());
            
            if (keyFrameNumber == 1 && !jon->isRightFoot())
            {
                jon->setRightFoot(true);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_WOOD)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_WOOD);
                }
            }
            else if (keyFrameNumber == 7 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_WOOD)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_WOOD);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon->isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon->getStateTime());
        }
        
        return idleAnim.getTextureRegion(jon->getStateTime());
    }
    else
    {
        static Animation deathAnim = ASSETS->findAnimation("Jon_Rabbit_Death");
        static Animation warmUpAnim = ASSETS->findAnimation("Jon_Rabbit_WarmUp");
        static Animation pushedBackAnim = ASSETS->findAnimation("Jon_Rabbit_PushedBack");
        static Animation idleAnim = ASSETS->findAnimation("Jon_Rabbit_Idle");
        static Animation runningAnim = ASSETS->findAnimation("Jon_Rabbit_Running");
        static Animation jumpingAnim = ASSETS->findAnimation("Jon_Rabbit_Jumping");
        static Animation doubleJumpingAnim = ASSETS->findAnimation("Jon_Rabbit_DoubleJumping");
        static Animation fallingAnim = ASSETS->findAnimation("Jon_Rabbit_Falling");
        static Animation landingAnim = ASSETS->findAnimation("Jon_Rabbit_Landing");
        static Animation spinningBackFistAnimation = ASSETS->findAnimation("Jon_Rabbit_SpinningBackFist");
        static Animation burrowAnimation = ASSETS->findAnimation("Jon_Rabbit_Burrow");
        static Animation stompAnimation = ASSETS->findAnimation("Jon_Rabbit_Stomp");
        static Animation landingRollAnimation = ASSETS->findAnimation("Jon_Rabbit_LandingRoll");
        
        if (jon->getState() != JON_ALIVE)
        {
            return deathAnim.getTextureRegion(jon->getDyingStateTime());
        }
        
        switch (jon->getAbilityState())
        {
            case ABILITY_SPINNING_BACK_FIST:
                return spinningBackFistAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_BURROW:
                return burrowAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_STOMP:
                return stompAnimation.getTextureRegion(jon->getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon->getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon->getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon->getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon->isRollLanding() && !jon->isPushedBack())
            {
                return landingRollAnimation.getTextureRegion(jon->getStateTime());
            }
            
            if (jon->isLanding())
            {
                return landingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        else if (jon->getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon->isFalling())
            {
                return fallingAnim.getTextureRegion(jon->getStateTime());
            }
        }
        
        if (jon->isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon->getStateTime());
            
            if (keyFrameNumber == 1 && !jon->isRightFoot())
            {
                jon->setRightFoot(true);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_WOOD)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_WOOD);
                }
            }
            else if (keyFrameNumber == 6 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_WOOD)
                {
                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_WOOD);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon->isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon->getStateTime());
        }
        
        return jon->isIdle() ? idleAnim.getTextureRegion(jon->getStateTime()) : warmUpAnim.getTextureRegion(jon->getStateTime());
    }
}

TextureRegion& MainAssets::get(DustCloud* dustCloud)
{
    switch (dustCloud->getType())
    {
        case DustCloudType_Landing:
        {
            static Animation anim = ASSETS->findAnimation("DustCloudType_Landing");
            
            return anim.getTextureRegion(dustCloud->getStateTime());
        }
        case DustCloudType_Kick_Up:
        {
            static Animation anim = ASSETS->findAnimation("DustCloudType_Kick_Up");
            
            return anim.getTextureRegion(dustCloud->getStateTime());
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(JonShadow* jonShadow)
{
    switch (jonShadow->getState())
    {
        case JonShadowState_Grounded:
        {
            static Animation anim = ASSETS->findAnimation("JonShadowState_Grounded");
            
            return anim.getTextureRegion(jonShadow->getStateTime());
        }
        case JonShadowState_Jumping:
        {
            static Animation anim = ASSETS->findAnimation("JonShadowState_Jumping");
            
            return anim.getTextureRegion(jonShadow->getStateTime());
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(MidBossOwl* owl)
{
    int damage = owl->getDamage();
    
    switch (owl->getState())
    {
        case MidBossOwlState_Sleeping:
        {
            static Animation anim = ASSETS->findAnimation("MidBossOwlState_Sleeping");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Awakening:
        {
            static Animation anim = ASSETS->findAnimation("MidBossOwlState_Awakening");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Screeching:
        {
            static Animation anim = ASSETS->findAnimation("MidBossOwlState_Screeching");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Pursuing:
        case MidBossOwlState_FlyingOverTree:
        {
            if (damage == 2)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_Pursuing_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_Pursuing_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_Pursuing_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_SwoopingDown:
        {
            if (damage == 2)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_SwoopingDown_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_SwoopingDown_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_SwoopingDown_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        {
            if (damage == 2)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_SlammingIntoTree:
        {
            if (damage == 2)
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_SlammingIntoTree_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = ASSETS->findAnimation("MidBossOwlState_SlammingIntoTree_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_Dying:
        case MidBossOwlState_Dead:
        {
            static Animation anim = ASSETS->findAnimation("MidBossOwlState_Dying_Dead");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(Bat* bat)
{
    static Animation anim = ASSETS->findAnimation("Bat");
    return anim.getTextureRegion(bat->getStateTime());
}

TextureRegion& MainAssets::get(BatInstruction* batInstruction)
{
    if (batInstruction->isOpening())
    {
        static Animation anim = ASSETS->findAnimation("BatInstruction_Opening");
        return anim.getTextureRegion(batInstruction->getStateTime());
    }
    else if (batInstruction->isOpen())
    {
        switch (batInstruction->getType())
        {
            case BatInstructionType_Tap:
            {
                static Animation anim = ASSETS->findAnimation("BatInstructionType_Tap");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_TapHold:
            {
                static Animation anim = ASSETS->findAnimation("BatInstructionType_TapHold");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_SwipeDown:
            {
                static Animation anim = ASSETS->findAnimation("BatInstructionType_SwipeDown");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_SwipeRight:
            {
                static Animation anim = ASSETS->findAnimation("BatInstructionType_SwipeRight");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_None:
            default:
                break;
        }
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(GameButton* gameButton)
{
    switch (gameButton->getType())
    {
        case GameButtonType_ToggleMusic:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_ToggleMusic");
            return tr;
        }
        case GameButtonType_ToggleSound:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_ToggleSound");
            return tr;
        }
        case GameButtonType_BackToTitle:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_BackToTitle");
            return tr;
        }
        case GameButtonType_Leaderboards:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_Leaderboards");
            return tr;
        }
        case GameButtonType_ViewOpeningCutscene:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_ViewOpeningCutscene");
            return tr;
        }
        case GameButtonType_BackToLevelSelect:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_BackToLevelSelect");
            return tr;
        }
        case GameButtonType_ContinueToLevelSelect:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_ContinueToLevelSelect");
            return tr;
        }
        case GameButtonType_LevelEditor:
        {
            static TextureRegion tr = ASSETS->findTextureRegion("GameButtonType_LevelEditor");
            return tr;
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& MainAssets::get(GameHudCarrot* gameHudCarrot)
{
    if (gameHudCarrot->isGolden())
    {
        static TextureRegion tr = ASSETS->findTextureRegion("GameHudCarrot_Golden");
        return tr;
    }
    else
    {
        static TextureRegion tr = ASSETS->findTextureRegion("GameHudCarrot_Normal");
        return tr;
    }
}

TextureRegion& MainAssets::get(LevelEditorEntitiesPanel* levelEditorEntitiesPanel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("EditorEntitiesPanel");
    return tr;
}

TextureRegion& MainAssets::get(LevelEditorActionsPanel* levelEditorActionsPanel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("EditorActionsPanel");
    return tr;
}

TextureRegion& MainAssets::get(TrashCan* trashCan)
{
    if (trashCan->isHighlighted())
    {
        static TextureRegion tr = ASSETS->findTextureRegion("TrashCan_Highlighted");
        return tr;
    }
    else
    {
        static TextureRegion tr = ASSETS->findTextureRegion("TrashCan_Normal");
        return tr;
    }
}

TextureRegion& MainAssets::get(LevelSelectorPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("SelectorPanel");
    return tr;
}

TextureRegion& MainAssets::get(TextureSelectorPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("SelectorPanel");
    return tr;
}

TextureRegion& MainAssets::get(OffsetPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("OffsetPanel");
    return tr;
}

TextureRegion& MainAssets::get(ConfirmResetPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("ConfirmResetPanel");
    return tr;
}

TextureRegion& MainAssets::get(ConfirmExitPanel* panel)
{
    static TextureRegion tr = ASSETS->findTextureRegion("ConfirmExitPanel");
    return tr;
}

bool MainAssets::isUsingCompressedTextureSet()
{
    return m_isUsingCompressedTextureSet;
}

void MainAssets::setUsingCompressedTextureSet(bool isUsingCompressedTextureSet)
{
    m_isUsingCompressedTextureSet = isUsingCompressedTextureSet;
}

bool MainAssets::isUsingDesktopTextureSet()
{
    return m_isUsingDesktopTextureSet;
}

void MainAssets::setUsingDesktopTextureSet(bool isUsingDesktopTextureSet)
{
    m_isUsingDesktopTextureSet = isUsingDesktopTextureSet;
}

bool MainAssets::isUsingGamePadTextureSet()
{
    return m_isUsingGamePadTextureSet;
}

void MainAssets::setUsingGamePadTextureSet(bool isUsingGamePadTextureSet)
{
    m_isUsingGamePadTextureSet = isUsingGamePadTextureSet;
}

MainAssets() : m_isUsingCompressedTextureSet(false), m_isUsingDesktopTextureSet(false), m_isUsingGamePadTextureSet(false)
{
    // Empty
}
