//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "Assets.h"
#include "CollectibleItem.h"
#include "LevelEditorActionsPanel.h"
#include "LevelEditorEntitiesPanel.h"
#include "Enemy.h"
#include "EnemySpirit.h"
#include "ForegroundObject.h"
#include "TitlePanel.h"
#include "CutscenePanel.h"
#include "WorldMapPanel.h"
#include "GameScreenWorldMap.h"
#include "Game.h"
#include "BatPanel.h"
#include "GameScreen.h"
#include "EndBossSnake.h"
#include "UnknownEntity.h"
#include "SpriteTesterEntitiesPanel.h"
#include "SpriteTesterActionsPanel.h"
#include "TextureSelectorPanel.h"
#include "AssetsMapper.h"

Assets* Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::get(TitlePanel* panel)
{
    static TextureRegion tr = findTextureRegion("TitlePanel_Background");
    return tr;
}

TextureRegion& Assets::get(CutsceneEffect* effect)
{
    switch (effect->getType())
    {
        case CutsceneEffectType_Shadow_One:
        {
            static TextureRegion tr = findTextureRegion("CutsceneEffectType_Shadow_One");
            return tr;
        }
        case CutsceneEffectType_Shadow_Two:
        {
            static TextureRegion tr = findTextureRegion("CutsceneEffectType_Shadow_Two");
            return tr;
        }
        case CutsceneEffectType_POW:
        {
            static Animation anim = findAnimation("CutsceneEffectType_POW");
            return anim.getTextureRegion(effect->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(CutscenePanel* panel)
{
    switch (panel->getType())
    {
        case CutscenePanelType_Opening_One:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_One");
            return tr;
        }
        case CutscenePanelType_Opening_Two:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Two");
            return tr;
        }
        case CutscenePanelType_Opening_Three:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Three");
            return tr;
        }
        case CutscenePanelType_Opening_Four:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Four");
            return tr;
        }
        case CutscenePanelType_Opening_Five:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Five");
            return tr;
        }
        case CutscenePanelType_Opening_Six:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Six");
            return tr;
        }
        case CutscenePanelType_Opening_Seven:
        {
            static TextureRegion tr = findTextureRegion("CutscenePanelType_Opening_Seven");
            return tr;
        }
        case CutscenePanelType_Opening_None:
        default:
            assert(false);
    }
    
    assert(false);
}

TextureRegion& Assets::get(WorldMapPanel* panel)
{
    static TextureRegion tr = findTextureRegion("WorldMapPanel");
    return tr;
}

TextureRegion& Assets::get(AbilitySlot* abilitySlot)
{
    if (abilitySlot->isUnlocking())
    {
        static Animation anim = findAnimation("AbilitySlot_Unlocking");
        return anim.getTextureRegion(abilitySlot->getStateTime());
    }
    else if (abilitySlot->isUnlocked())
    {
        switch (abilitySlot->getType())
        {
            case AbilitySlotType_Drill:
            {
                static Animation anim = findAnimation("AbilitySlotType_Drill");
                return anim.getTextureRegion(abilitySlot->getStateTime());
            }
            case AbilitySlotType_Dash:
            {
                static Animation anim = findAnimation("AbilitySlotType_Dash");
                return anim.getTextureRegion(abilitySlot->getStateTime());
            }
        }
    }
    
    static TextureRegion tr = findTextureRegion("AbilitySlot_Locked");
    return tr;
}

TextureRegion& Assets::get(LevelThumbnail* thumbnail)
{
    if (thumbnail->isBoss())
    {
        BossLevelThumbnail *bossLevelThumbnail = reinterpret_cast<BossLevelThumbnail *>(thumbnail);
        
		if (bossLevelThumbnail->isUnlocking())
        {
            static Animation anim = findAnimation("BossLevelThumbnail_Unlocking");
            return anim.getTextureRegion(thumbnail->getStateTime());
        }
        else if (bossLevelThumbnail->isUnlocked())
        {
            if (thumbnail->isSelected())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = findAnimation("BossLevelThumbnail_Selected_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("BossLevelThumbnail_Selected");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isSelecting())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = findAnimation("BossLevelThumbnail_Selecting_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("BossLevelThumbnail_Selecting");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isClearing())
            {
                static Animation anim = findAnimation("BossLevelThumbnail_Clearing");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else if (thumbnail->isCleared())
            {
                static TextureRegion tr = findTextureRegion("BossLevelThumbnail_Cleared");
                return tr;
            }
            else
            {
                static TextureRegion tr = findTextureRegion("BossLevelThumbnail_Unlocked");
                return tr;
            }
        }
        else
        {
            if (bossLevelThumbnail->isJawMoving())
            {
                static Animation anim = findAnimation("BossLevelThumbnail_Locked_Jaw_Moving");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else
            {
                static TextureRegion tr = findTextureRegion("BossLevelThumbnail_Locked");
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
                    static Animation anim = findAnimation("NormalLevelThumbnail_Selected_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("NormalLevelThumbnail_Selected");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isSelecting())
            {
                if (thumbnail->isCleared())
                {
                    static Animation anim = findAnimation("NormalLevelThumbnail_Selecting_Cleared");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("NormalLevelThumbnail_Selecting");
                    return anim.getTextureRegion(thumbnail->getStateTime());
                }
            }
            else if (thumbnail->isClearing())
            {
                static Animation anim = findAnimation("NormalLevelThumbnail_Clearing");
                return anim.getTextureRegion(thumbnail->getStateTime());
            }
            else if (thumbnail->isCleared())
            {
                static TextureRegion tr = findTextureRegion("NormalLevelThumbnail_Cleared");
                return tr;
            }
            else
            {
                static TextureRegion tr = findTextureRegion("NormalLevelThumbnail_Playable");
                return tr;
            }
        }
        else
        {
            static TextureRegion tr = findTextureRegion("NormalLevelThumbnail_Unplayable");
            return tr;
        }
    }
}

TextureRegion& Assets::get(GoldenCarrotsMarker* goldenCarrotsMarker)
{
    switch (goldenCarrotsMarker->getNumGoldenCarrots())
    {
        case 0:
        {
            static Animation anim = findAnimation("GoldenCarrotsMarker_0");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 1:
        {
            static Animation anim = findAnimation("GoldenCarrotsMarker_1");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 2:
        {
            static Animation anim = findAnimation("GoldenCarrotsMarker_2");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 3:
        {
            static Animation anim = findAnimation("GoldenCarrotsMarker_3");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
        case 4:
        {
            static Animation anim = findAnimation("GoldenCarrotsMarker_4");
            return anim.getTextureRegion(goldenCarrotsMarker->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(SpendGoldenCarrotsBubble* spendGoldenCarrotsBubble)
{
    if (spendGoldenCarrotsBubble->userHasEnoughGoldenCats())
    {
        static Animation anim = findAnimation("SpendGoldenCarrotsBubble_Has_Enough");
        return anim.getTextureRegion(spendGoldenCarrotsBubble->getStateTime());
    }
    
    static Animation anim = findAnimation("SpendGoldenCarrotsBubble_Not_Enough");
    return anim.getTextureRegion(spendGoldenCarrotsBubble->getStateTime());
}

TextureRegion& Assets::get(Background* background)
{
    switch (background->getType())
    {
        case BackgroundType_Upper:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Upper");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Mid_Hills:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Mid_Hills");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Lower_Innermost:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Lower_Innermost");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Mid_Trees:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Mid_Trees");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Lower_Inner:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Lower_Inner");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Lower_Top:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Lower_Top");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Lower_Bottom:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Lower_Bottom");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_WaterBack:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_WaterBack");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_WaterFront:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_WaterFront");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_GAME, TEXTURE_SIZE_2048);
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Midground* midground)
{
    switch (midground->getType())
    {
        case MidgroundType_TreeOne:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_TreeOne");
            return tr;
        }
        case MidgroundType_TreeTwo:
        case MidgroundType_TreeThree:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_TreeTwo");
            return tr;
        }
        case MidgroundType_DeepCaveColumnSmall:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_DeepCaveColumnSmall");
            return tr;
        }
        case MidgroundType_DeepCaveColumnMedium:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_DeepCaveColumnMedium");
            return tr;
        }
        case MidgroundType_DeepCaveColumnBig:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_DeepCaveColumnBig");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Ground* ground)
{
    switch (ground->getType())
    {
        case GroundType_CaveExtraDeepEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveExtraDeepEndLeft");
            return tr;
        }
        case GroundType_CaveExtraDeepSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveExtraDeepSmall");
            return tr;
        }
        case GroundType_CaveExtraDeepMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveExtraDeepMedium");
            return tr;
        }
        case GroundType_CaveExtraDeepLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveExtraDeepLarge");
            return tr;
        }
        case GroundType_CaveExtraDeepEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveExtraDeepEndRight");
            return tr;
        }
            
        case GroundType_CaveDeepEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveDeepEndLeft");
            return tr;
        }
        case GroundType_CaveDeepSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveDeepSmall");
            return tr;
        }
        case GroundType_CaveDeepMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveDeepMedium");
            return tr;
        }
        case GroundType_CaveDeepLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveDeepLarge");
            return tr;
        }
        case GroundType_CaveDeepEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveDeepEndRight");
            return tr;
        }
            
        case GroundType_CaveEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveEndLeft");
            return tr;
        }
        case GroundType_CaveSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveSmall");
            return tr;
        }
        case GroundType_CaveMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveMedium");
            return tr;
        }
        case GroundType_CaveLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveLarge");
            return tr;
        }
        case GroundType_CaveEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveEndRight");
            return tr;
        }
            
        case GroundType_CaveRaisedEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveRaisedEndLeft");
            return tr;
        }
        case GroundType_CaveRaisedSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveRaisedSmall");
            return tr;
        }
        case GroundType_CaveRaisedMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveRaisedMedium");
            return tr;
        }
        case GroundType_CaveRaisedLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveRaisedLarge");
            return tr;
        }
        case GroundType_CaveRaisedEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_CaveRaisedEndRight");
            return tr;
        }
            
        case GroundType_GrassWithCaveEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithCaveEndLeft");
            return tr;
        }
        case GroundType_GrassWithCaveSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithCaveSmall");
            return tr;
        }
        case GroundType_GrassWithCaveMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithCaveMedium");
            return tr;
        }
        case GroundType_GrassWithCaveLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithCaveLarge");
            return tr;
        }
        case GroundType_GrassWithCaveEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithCaveEndRight");
            return tr;
        }
            
        case GroundType_GrassWithoutCaveEndLeft:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithoutCaveEndLeft");
            return tr;
        }
        case GroundType_GrassWithoutCaveSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithoutCaveSmall");
            return tr;
        }
        case GroundType_GrassWithoutCaveMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithoutCaveMedium");
            return tr;
        }
        case GroundType_GrassWithoutCaveLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithoutCaveLarge");
            return tr;
        }
        case GroundType_GrassWithoutCaveEndRight:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassWithoutCaveEndRight");
            return tr;
        }
            
        case GroundType_GrassPitSmall:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassPitSmall");
            return tr;
        }
        case GroundType_GrassPitMedium:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassPitMedium");
            return tr;
        }
        case GroundType_GrassPitLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassPitLarge");
            return tr;
        }
        case GroundType_GrassPitExtraLarge:
        {
            static TextureRegion tr = findTextureRegion("GroundType_GrassPitExtraLarge");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGround* exitGround)
{
    switch (exitGround->getType())
    {
        case ExitGroundType_GrassWithCaveSmallExitMid:
        {
            static TextureRegion tr = findTextureRegion("ExitGroundType_GrassWithCaveSmallExitMid");
            return tr;
        }
        case ExitGroundType_GrassWithCaveSmallExitEnd:
        {
            static TextureRegion tr = findTextureRegion("ExitGroundType_GrassWithCaveSmallExitEnd");
            return tr;
        }
        case ExitGroundType_CaveSmallExit:
        {
            static TextureRegion tr = findTextureRegion("ExitGroundType_CaveSmallExit");
            return tr;
        }
            
        case ExitGroundType_CaveDeepSmallWaterfall:
        {
            static Animation anim = findAnimation("ExitGroundType_CaveDeepSmallWaterfall");
            return anim.getTextureRegion(exitGround->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ExitGroundCover* exitGroundCover)
{
    switch (exitGroundCover->getType())
    {
        case ExitGroundCoverType_Grass:
        {
            static Animation anim = findAnimation("ExitGroundCoverType_Grass");
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
        case ExitGroundCoverType_Cave:
        {
            static Animation anim = findAnimation("ExitGroundCoverType_Cave");
            return anim.getTextureRegion(exitGroundCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Hole* hole)
{
    switch (hole->getType())
    {
        case HoleType_GrassTileLeft:
        {
            static TextureRegion tr = findTextureRegion("HoleType_GrassTileLeft");
            return tr;
        }
        case HoleType_GrassTileCenter:
        {
            static TextureRegion tr = findTextureRegion("HoleType_GrassTileCenter");
            return tr;
        }
        case HoleType_GrassTileRight:
        {
            static TextureRegion tr = findTextureRegion("HoleType_GrassTileRight");
            return tr;
        }
        case HoleType_Grass:
        {
            static TextureRegion tr = findTextureRegion("HoleType_Grass");
            return tr;
        }
        case HoleType_Cave:
        {
            static TextureRegion tr = findTextureRegion("HoleType_Cave");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(HoleCover* holeCover)
{
    switch (holeCover->getType())
    {
        case HoleCoverType_GrassTileLeft:
        {
            static Animation anim = findAnimation("HoleCoverType_GrassTileLeft");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileCenter:
        {
            static Animation anim = findAnimation("HoleCoverType_GrassTileCenter");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_GrassTileRight:
        {
            static Animation anim = findAnimation("HoleCoverType_GrassTileRight");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Grass:
        {
            static Animation anim = findAnimation("HoleCoverType_Grass");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
        case HoleCoverType_Cave:
        {
            static Animation anim = findAnimation("HoleCoverType_Cave");
            return anim.getTextureRegion(holeCover->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(ForegroundObject* foregroundObject)
{
    switch (foregroundObject->getType())
    {
        case ForegroundObjectType_GrassPlatformLeft:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_GrassPlatformLeft");
            return tr;
        }
        case ForegroundObjectType_GrassPlatformCenter:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_GrassPlatformCenter");
            return tr;
        }
        case ForegroundObjectType_GrassPlatformRight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_GrassPlatformRight");
            return tr;
        }
            
        case ForegroundObjectType_CavePlatformLeft:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_CavePlatformLeft");
            return tr;
        }
        case ForegroundObjectType_CavePlatformCenter:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_CavePlatformCenter");
            return tr;
        }
        case ForegroundObjectType_CavePlatformRight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_CavePlatformRight");
            return tr;
        }
            
        case ForegroundObjectType_RockLarge:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_RockLarge");
            return tr;
        }
        case ForegroundObjectType_RockMedium:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_RockMedium");
            return tr;
        }
        case ForegroundObjectType_RockSmall:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_RockSmall");
            return tr;
        }
        case ForegroundObjectType_RockSmallCracked:
        {
            static Animation anim = findAnimation("ForegroundObjectType_RockSmallCracked");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_StumpBig:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_StumpBig");
            return tr;
        }
        case ForegroundObjectType_StumpSmall:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_StumpSmall");
            return tr;
        }
            
        case ForegroundObjectType_EndSign:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_EndSign");
            return tr;
        }
            
        case ForegroundObjectType_ThornsLeft:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_ThornsLeft");
            return tr;
        }
        case ForegroundObjectType_ThornsCenterSmall:
        case ForegroundObjectType_ThornsCenterBig:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_ThornsCenter");
            return tr;
        }
        case ForegroundObjectType_ThornsRight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_ThornsRight");
            return tr;
        }
            
        case ForegroundObjectType_LogVerticalTall:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_LogVerticalTall");
            return tr;
        }
        case ForegroundObjectType_LogVerticalShort:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_LogVerticalShort");
            return tr;
        }
            
        case ForegroundObjectType_JumpSpringLightFlush:
        {
            static Animation anim = findAnimation("ForegroundObjectType_JumpSpringLightFlush");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringLight:
        {
            static Animation anim = findAnimation("ForegroundObjectType_JumpSpringLight");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringMedium:
        {
            static Animation anim = findAnimation("ForegroundObjectType_JumpSpringMedium");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_JumpSpringHeavy:
        {
            static Animation anim = findAnimation("ForegroundObjectType_JumpSpringHeavy");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_SpikeGrassSingle:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeGrassSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeGrassFour:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeGrassFour");
            return tr;
        }
        case ForegroundObjectType_SpikeGrassEight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeGrassEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveSingle:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveFour:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveFour");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveEight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeCaveCeilingSingle:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveCeilingSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingFour:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveCeilingFour");
            return tr;
        }
        case ForegroundObjectType_SpikeCaveCeilingEight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeCaveCeilingEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeWallSingle:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeWallSingle");
            return tr;
        }
        case ForegroundObjectType_SpikeWallFour:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeWallFour");
            return tr;
        }
        case ForegroundObjectType_SpikeWallEight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeWallEight");
            return tr;
        }
            
        case ForegroundObjectType_SpikeStar:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeStar");
            return tr;
        }
            
        case ForegroundObjectType_VerticalSaw:
        {
            static Animation anim = findAnimation("ForegroundObjectType_VerticalSaw");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
            
        case ForegroundObjectType_GiantTree:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_GiantTree");
            return tr;
        }
        case ForegroundObjectType_GiantShakingTree:
        {
            static Animation anim = findAnimation("ForegroundObjectType_GiantShakingTree");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
        case ForegroundObjectType_GiantPerchTree:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_GiantPerchTree");
            return tr;
        }
            
        case ForegroundObjectType_SpikeTower:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeTower");
            return tr;
        }
        case ForegroundObjectType_SpikeTowerBg:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikeTowerBg");
            return tr;
        }
            
        case ForegroundObjectType_SpikedBallRollingLeft:
        case ForegroundObjectType_SpikedBallRollingRight:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikedBallRolling");
            return tr;
        }
            
        case ForegroundObjectType_SpikedBall:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_SpikedBall");
            return tr;
        }
        case ForegroundObjectType_SpikedBallChain:
        {
            static Animation anim = findAnimation("ForegroundObjectType_SpikedBallChain");
            return anim.getTextureRegion(foregroundObject->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(CountHissWithMina* countHissWithMina)
{
    if (countHissWithMina->isFacingLeft())
    {
		static Animation anim = findAnimation("CountHissWithMina_Left");
		return anim.getTextureRegion(countHissWithMina->getStateTime());
    }
    else
    {
		static Animation anim = findAnimation("CountHissWithMina_Right");
		return anim.getTextureRegion(countHissWithMina->getStateTime());
    }
}

TextureRegion& Assets::get(SnakeSpirit* snakeSpirit)
{
    static Animation anim = findAnimation("SnakeSpirit");
    return anim.getTextureRegion(snakeSpirit->getStateTime());
}

TextureRegion& Assets::get(SnakeHeadImpact* snakeHeadImpact)
{
	static Animation anim = findAnimation("SnakeHeadImpact");
	return anim.getTextureRegion(snakeHeadImpact->getStateTime());
}

TextureRegion& Assets::get(SnakeSkin* snakeSkin)
{
    static Animation anim = findAnimation("SnakeSkin");
    return anim.getTextureRegion(snakeSkin->getStateTime());
}

TextureRegion& Assets::get(SnakeEye* snakeEye)
{
    static Animation anim = findAnimation("SnakeEye");
    return anim.getTextureRegion(snakeEye->getStateTime());
}

TextureRegion& Assets::get(SnakeTonque* snakeTonque)
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
            static Animation anim = findAnimation("SnakeTonque_Left");
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
            static Animation anim = findAnimation("SnakeTonque_Right");
            return anim.getTextureRegion(snakeTonque->getStateTime());
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(SnakeBody* snakeBody)
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
            static TextureRegion tr = findTextureRegion("SnakeBody_Left");
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
            static TextureRegion tr = findTextureRegion("SnakeBody_Right");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(EndBossSnake* endBossSnake)
{
    switch (endBossSnake->getState())
    {
        case EndBossSnakeState_Sleeping:
        case EndBossSnakeState_Awakening:
		{
			static TextureRegion tr = findTextureRegion("EndBossSnakeState_Sleeping_Awakening");
			return tr;
		}
		case EndBossSnakeState_OpeningMouthLeft:
		{
			static Animation anim = findAnimation("EndBossSnakeState_OpeningMouthLeft");
			return anim.getTextureRegion(endBossSnake->getStateTime());
		}
		case EndBossSnakeState_OpenMouthLeft:
        case EndBossSnakeState_ChargingLeft:
		{
			static TextureRegion tr = findTextureRegion("EndBossSnakeState_OpenMouthLeft_ChargingLeft");
			return tr;
		}
        case EndBossSnakeState_Pursuing:
		case EndBossSnakeState_Waiting:
		case EndBossSnakeState_Damaged:
		{
			static TextureRegion tr = findTextureRegion("EndBossSnakeState_Pursuing_Waiting_Damaged");
			return tr;
		}
        case EndBossSnakeState_OpeningMouthRight:
		{
			static Animation anim = findAnimation("EndBossSnakeState_OpeningMouthRight");
			return anim.getTextureRegion(endBossSnake->getStateTime());
		}
		case EndBossSnakeState_OpenMouthRight:
        case EndBossSnakeState_ChargingRight:
		{
			static TextureRegion tr = findTextureRegion("EndBossSnakeState_OpenMouthRight_ChargingRight");
			return tr;
		}
        case EndBossSnakeState_Dying:
        {
            static Animation anim = findAnimation("EndBossSnakeState_Dying");
            return anim.getTextureRegion(endBossSnake->getStateTime());
        }
        case EndBossSnakeState_DeadSpiritReleasing:
        case EndBossSnakeState_Dead:
        {
            static TextureRegion tr = findTextureRegion("EndBossSnakeState_DeadSpiritReleasing_Dead");
            return tr;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(Enemy* enemy)
{
    switch (enemy->getType())
    {
        case EnemyType_MushroomGround:
        {
            static Animation anim = findAnimation("EnemyType_MushroomGround");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_MushroomCeiling:
        {
            static Animation anim = findAnimation("EnemyType_MushroomCeiling");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_SnakeGrunt:
        {
            static Animation anim = findAnimation("EnemyType_SnakeGrunt");
            return anim.getTextureRegion(enemy->getStateTime());
        }
        case EnemyType_Sparrow:
        {
            static Animation anim = findAnimation("EnemyType_Sparrow");
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
                        static Animation anim = findAnimation("EnemyType_Toad_Dead_Part_1_Jon_Is_Vampire");
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                    else
                    {
                        // Rabbit Part 1
                        static Animation anim = findAnimation("EnemyType_Toad_Dead_Part_1_Jon_Is_Rabbit");
                        return anim.getTextureRegion(enemy->getStateTime());
                    }
                }
                else
                {
                    // Part 2
                    static Animation anim = findAnimation("EnemyType_Toad_Dead_Part_2");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (toad->isEating())
            {
                if (toad->isJonVampire())
                {
                    static Animation anim = findAnimation("EnemyType_Toad_Eating_Jon_Is_Vampire");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("EnemyType_Toad_Eating_Jon_Is_Rabbit");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else
            {
                static Animation anim = findAnimation("EnemyType_Toad");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_Fox:
        {
            Fox* fox = reinterpret_cast<Fox *>(enemy);
            
            if (fox->isDead())
            {
                static Animation anim = findAnimation("EnemyType_Fox_Is_Dead");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (fox->isHitting())
            {
                if (fox->isLeft())
                {
                    static Animation anim = findAnimation("EnemyType_Fox_Is_Hitting_Left");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("EnemyType_Fox_Is_Hitting_Right");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
            else if (fox->isBeingHit())
            {
                static Animation anim = findAnimation("EnemyType_Fox_Is_Being_Hit");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                if (fox->isLeft())
                {
                    static Animation anim = findAnimation("EnemyType_Fox_Left");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
                else
                {
                    static Animation anim = findAnimation("EnemyType_Fox_Right");
                    return anim.getTextureRegion(enemy->getStateTime());
                }
            }
        }
        case EnemyType_BigMushroomGround:
        {
            Mushroom* mushroom = reinterpret_cast<Mushroom *>(enemy);
            if (mushroom->isBouncingBack())
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomGround_Bouncing_Back");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (mushroom->isBeingBouncedOn())
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomGround_Bouncing_On");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomGround");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
        case EnemyType_BigMushroomCeiling:
        {
            Mushroom* mushroom = reinterpret_cast<Mushroom *>(enemy);
            if (mushroom->isBouncingBack())
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomCeiling_Bouncing_Back");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (mushroom->isBeingBouncedOn())
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomCeiling_Bouncing_On");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("EnemyType_BigMushroomCeiling");
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
                static Animation anim = findAnimation("MovingSnakeGrunt_Jumping");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->getVelocity().getY() < 0)
            {
                static Animation anim = findAnimation("MovingSnakeGrunt_Falling");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isPreparingToJump())
            {
                static Animation anim = findAnimation("MovingSnakeGrunt_About_To_Jump");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isLanding())
            {
                static Animation anim = findAnimation("MovingSnakeGrunt_Landing");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else if (snake->isPausing())
            {
                static Animation anim = findAnimation("MovingSnakeGrunt_Pausing");
                return anim.getTextureRegion(enemy->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("MovingSnakeGrunt");
                return anim.getTextureRegion(enemy->getStateTime());
            }
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(EnemySpirit* spirit)
{
    switch (spirit->getType())
    {
        case EnemySpiritType_Snake:
        {
            static Animation anim = findAnimation("EnemySpiritType_Snake");
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_Sparrow:
        {
            static Animation anim = findAnimation("EnemySpiritType_Sparrow");
            return anim.getTextureRegion(spirit->getStateTime());
        }
        case EnemySpiritType_None:
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& Assets::get(CollectibleItem* collectibleItem)
{
    switch (collectibleItem->getType())
    {
        case CollectibleItemType_Carrot:
        {
            if (collectibleItem->isCollected())
            {
                static Animation anim = findAnimation("CollectibleItemType_Carrot_Collected");
                return anim.getTextureRegion(collectibleItem->getStateTime());
            }
            else
            {
                static TextureRegion tr = findTextureRegion("CollectibleItemType_Carrot_Not_Collected");
                return tr;
            }
        }
        case CollectibleItemType_GoldenCarrot:
        {
            GoldenCarrot* gc = reinterpret_cast<GoldenCarrot *>(collectibleItem);
            
            if (gc->isPreviouslyCollected())
            {
                static TextureRegion tr = findTextureRegion("CollectibleItemType_GoldenCarrot_Previously_Collected");
                return tr;
            }
            else if (collectibleItem->isCollected())
            {
                static Animation anim = findAnimation("CollectibleItemType_GoldenCarrot_Collected");
                return anim.getTextureRegion(collectibleItem->getStateTime());
            }
            else
            {
                static TextureRegion tr = findTextureRegion("CollectibleItemType_GoldenCarrot_Not_Collected");
                return tr;
            }
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(GoldenCarrotTwinkle* goldenCarrotTwinkle)
{
    static Animation anim = findAnimation("GoldenCarrotTwinkle");
    return anim.getTextureRegion(goldenCarrotTwinkle->getStateTime());
}

TextureRegion& Assets::get(Jon* jon)
{
    if (jon->isTransformingIntoVampire())
    {
        static Animation anim = findAnimation("Jon_isTransformingIntoVampire");
        
        return anim.getTextureRegion(jon->getTransformStateTime());
    }
    else if (jon->isRevertingToRabbit())
    {
        static Animation anim = findAnimation("Jon_isRevertingToRabbit");
        
        return anim.getTextureRegion(jon->getTransformStateTime());
    }
    
    if (jon->isVampire())
    {
        static Animation deathAnim = findAnimation("Jon_Vampire_Death");
        static Animation pushedBackAnim = findAnimation("Jon_Vampire_PushedBack");
        static Animation idleAnim = findAnimation("Jon_Vampire_Idle");
        static Animation runningAnim = findAnimation("Jon_Vampire_Running");
        static Animation upwardThrustAnim = findAnimation("Jon_Vampire_UpwardThrust");
        static Animation dashAnim = findAnimation("Jon_Vampire_Dash");
        static Animation doubleJumpingAnim = findAnimation("Jon_Vampire_DoubleJumping");
        static Animation glidingAnim = findAnimation("Jon_Vampire_Gliding");
        static Animation fallingAnim = findAnimation("Jon_Vampire_Falling");
        static Animation landingAnim = findAnimation("Jon_Vampire_Landing");
        
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
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    ASSETS->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    ASSETS->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
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
        static Animation deathAnim = findAnimation("Jon_Rabbit_Death");
        static Animation warmUpAnim = findAnimation("Jon_Rabbit_WarmUp");
        static Animation pushedBackAnim = findAnimation("Jon_Rabbit_PushedBack");
        static Animation idleAnim = findAnimation("Jon_Rabbit_Idle");
        static Animation runningAnim = findAnimation("Jon_Rabbit_Running");
        static Animation jumpingAnim = findAnimation("Jon_Rabbit_Jumping");
        static Animation doubleJumpingAnim = findAnimation("Jon_Rabbit_DoubleJumping");
        static Animation fallingAnim = findAnimation("Jon_Rabbit_Falling");
        static Animation landingAnim = findAnimation("Jon_Rabbit_Landing");
        static Animation spinningBackFistAnimation = findAnimation("Jon_Rabbit_SpinningBackFist");
        static Animation burrowAnimation = findAnimation("Jon_Rabbit_Burrow");
        static Animation stompAnimation = findAnimation("Jon_Rabbit_Stomp");
        static Animation landingRollAnimation = findAnimation("Jon_Rabbit_LandingRoll");
        
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
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon->isRightFoot())
            {
                jon->setRightFoot(false);
                
                if (jon->getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    ASSETS->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon->getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    ASSETS->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
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

TextureRegion& Assets::get(DustCloud* dustCloud)
{
    static Animation anim = findAnimation("DustCloud");
    
    return anim.getTextureRegion(dustCloud->getStateTime());
}

TextureRegion& Assets::get(MidBossOwl* owl)
{
    int damage = owl->getDamage();
    
    switch (owl->getState())
    {
        case MidBossOwlState_Sleeping:
        {
            static Animation anim = findAnimation("MidBossOwlState_Sleeping");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Awakening:
        {
            static Animation anim = findAnimation("MidBossOwlState_Awakening");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Screeching:
        {
            static Animation anim = findAnimation("MidBossOwlState_Screeching");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        case MidBossOwlState_Pursuing:
        case MidBossOwlState_FlyingOverTree:
        {
            if (damage == 2)
            {
                static Animation anim = findAnimation("MidBossOwlState_Pursuing_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = findAnimation("MidBossOwlState_Pursuing_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("MidBossOwlState_Pursuing_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_SwoopingDown:
        {
            if (damage == 2)
            {
                static Animation anim = findAnimation("MidBossOwlState_SwoopingDown_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = findAnimation("MidBossOwlState_SwoopingDown_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("MidBossOwlState_SwoopingDown_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_FlyingAwayAfterCatchingJon:
        {
            if (damage == 2)
            {
                static Animation anim = findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else if (damage == 1)
            {
                static Animation anim = findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_0");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_SlammingIntoTree:
        {
            if (damage == 2)
            {
                static Animation anim = findAnimation("MidBossOwlState_SlammingIntoTree_Damage_2");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
            else
            {
                static Animation anim = findAnimation("MidBossOwlState_SlammingIntoTree_Damage_1");
                
                return anim.getTextureRegion(owl->getStateTime());
            }
        }
        case MidBossOwlState_Dying:
        case MidBossOwlState_Dead:
        {
            static Animation anim = findAnimation("MidBossOwlState_Dying_Dead");
            
            return anim.getTextureRegion(owl->getStateTime());
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& Assets::get(Bat* bat)
{
    static Animation anim = findAnimation("Bat");
    return anim.getTextureRegion(bat->getStateTime());
}

TextureRegion& Assets::get(BatInstruction* batInstruction)
{
    if (batInstruction->isOpening())
    {
        static Animation anim = findAnimation("BatInstruction_Opening");
        return anim.getTextureRegion(batInstruction->getStateTime());
    }
    else if (batInstruction->isOpen())
    {
        switch (batInstruction->getType())
        {
            case BatInstructionType_Tap:
            {
                static Animation anim = findAnimation("BatInstructionType_Tap");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_TapHold:
            {
                static Animation anim = findAnimation("BatInstructionType_TapHold");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_SwipeDown:
            {
                static Animation anim = findAnimation("BatInstructionType_SwipeDown");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_SwipeRight:
            {
                static Animation anim = findAnimation("BatInstructionType_SwipeRight");
                return anim.getTextureRegion(batInstruction->getStateTime());
            }
            case BatInstructionType_None:
            default:
                break;
        }
    }
    
    assert(false);
}

TextureRegion& Assets::get(GameButton* gameButton)
{
    switch (gameButton->getType())
    {
        case GameButtonType_ToggleMusic:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_ToggleMusic");
            return tr;
        }
        case GameButtonType_ToggleSound:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_ToggleSound");
            return tr;
        }
        case GameButtonType_BackToTitle:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_BackToTitle");
            return tr;
        }
        case GameButtonType_Leaderboards:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_Leaderboards");
            return tr;
        }
        case GameButtonType_ViewOpeningCutscene:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_ViewOpeningCutscene");
            return tr;
        }
        case GameButtonType_BackToLevelSelect:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_BackToLevelSelect");
            return tr;
        }
        case GameButtonType_ContinueToLevelSelect:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_ContinueToLevelSelect");
            return tr;
        }
        case GameButtonType_LevelEditor:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_LevelEditor");
            return tr;
        }
        case GameButtonType_SpriteTester:
        {
            static TextureRegion tr = findTextureRegion("GameButtonType_SpriteTester");
            return tr;
        }
        default:
            break;
    }
    
    assert(false);
}

TextureRegion& Assets::get(GameHudCarrot* gameHudCarrot)
{
    if (gameHudCarrot->isGolden())
    {
        static TextureRegion tr = findTextureRegion("GameHudCarrot_Golden");
        return tr;
    }
    else
    {
        static TextureRegion tr = findTextureRegion("GameHudCarrot_Normal");
        return tr;
    }
}

TextureRegion& Assets::get(LevelEditorEntitiesPanel* levelEditorEntitiesPanel)
{
    static TextureRegion tr = findTextureRegion("EditorEntitiesPanel");
    return tr;
}

TextureRegion& Assets::get(LevelEditorActionsPanel* levelEditorActionsPanel)
{
    static TextureRegion tr = findTextureRegion("EditorActionsPanel");
    return tr;
}

TextureRegion& Assets::get(SpriteTesterEntitiesPanel* spriteTesterEntitiesPanel)
{
    static TextureRegion tr = findTextureRegion("EditorEntitiesPanel");
    return tr;
}

TextureRegion& Assets::get(SpriteTesterActionsPanel* spriteTesterActionsPanel)
{
    static TextureRegion tr = findTextureRegion("EditorActionsPanel");
    return tr;
}

TextureRegion& Assets::get(TrashCan* trashCan)
{
    if (trashCan->isHighlighted())
    {
        static TextureRegion tr = findTextureRegion("TrashCan_Highlighted");
        return tr;
    }
    else
    {
        static TextureRegion tr = findTextureRegion("TrashCan_Normal");
        return tr;
    }
}

TextureRegion& Assets::get(LevelSelectorPanel* panel)
{
    static TextureRegion tr = findTextureRegion("SelectorPanel");
    return tr;
}

TextureRegion& Assets::get(TextureSelectorPanel* panel)
{
    static TextureRegion tr = findTextureRegion("SelectorPanel");
    return tr;
}

TextureRegion& Assets::get(OffsetPanel* panel)
{
    static TextureRegion tr = findTextureRegion("OffsetPanel");
    return tr;
}

TextureRegion& Assets::get(ConfirmResetPanel* panel)
{
    static TextureRegion tr = findTextureRegion("ConfirmResetPanel");
    return tr;
}

TextureRegion& Assets::get(ConfirmExitPanel* panel)
{
    static TextureRegion tr = findTextureRegion("ConfirmExitPanel");
    return tr;
}

TextureRegion& Assets::get(UnknownEntity* entity)
{
    TextureRegion& tr = findTextureRegion(entity->getAssetId(), entity->getStateTime());
    
    return tr;
}

short Assets::getFirstSoundId()
{
    return m_sSoundIds.size() > 0 ? m_sSoundIds.front() : 0;
}

void Assets::addSoundIdToPlayQueue(short soundId)
{
    if (!m_isSoundEnabled)
    {
        return;
    }
    
    if (soundId > NO_SOUND && m_sSoundIds.size() < MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::forceAddSoundIdToPlayQueue(short soundId)
{
    if (!m_isSoundEnabled)
    {
        return;
    }
    
    if (soundId > NO_SOUND)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::eraseFirstSoundId()
{
    if (m_sSoundIds.size() > 0)
    {
        m_sSoundIds.erase(m_sSoundIds.begin());
    }
}

short Assets::getFirstMusicId()
{
    return m_sMusicIds.size() > 0 ? m_sMusicIds.front() : 0;
}

void Assets::addMusicIdToPlayQueue(short musicId)
{
    if (!m_isMusicEnabled
        && musicId > MUSIC_STOP)
    {
        return;
    }
    
    m_sMusicIds.push_back(musicId);
}

void Assets::eraseFirstMusicId()
{
    if (m_sMusicIds.size() > 0)
    {
        m_sMusicIds.erase(m_sMusicIds.begin());
    }
}

TextureRegion& Assets::findTextureRegion(std::string key)
{
    return ASSETS_MAPPER->findTextureRegion(key);
}

Animation& Assets::findAnimation(std::string key)
{
    return ASSETS_MAPPER->findAnimation(key);
}

TextureRegion& Assets::findTextureRegion(std::string key, float stateTime)
{
    return ASSETS_MAPPER->findTextureRegion(key, stateTime);
}

void Assets::initTextureRegion(TextureRegion& tr, int x, int regionWidth, int textureWidth)
{
	if (m_isUsingCompressedTextureSet)
	{
		x /= 2.0;
		regionWidth /= 2.0;
		textureWidth /= 2.0;
	}

	tr.init(x, regionWidth, textureWidth);
}
