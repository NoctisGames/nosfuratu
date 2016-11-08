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

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

void Assets::initializeAssets()
{
    /// Texture Regions
    m_textureRegions["CutsceneEffectType_Shadow_One"] = createTextureRegion(0, 0, 966, 504, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutsceneEffectType_Shadow_Two"] = createTextureRegion(0, 508, 1600, 760, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_One"] = createTextureRegion(0, 0, 1600, 1200, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Two"] = createTextureRegion(0, 1204, 1600, 900, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Three"] = createTextureRegion(0, 2108, 1600, 900, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Four"] = createTextureRegion(1604, 0, 1600, 900, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Five"] = createTextureRegion(0, 3012, 1600, 900, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Six"] = createTextureRegion(1604, 904, 1600, 1200, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CutscenePanelType_Opening_Seven"] = createTextureRegion(1604, 2108, 1600, 900, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["WorldMapPanel"] = createTextureRegion(0, 0, 1088, 612, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["AbilitySlot_Locked"] = createTextureRegion(0, 458, 118, 114, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BossLevelThumbnail_Cleared"] = createTextureRegion(0, 1832, 198, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BossLevelThumbnail_Unlocked"] = createTextureRegion(198, 1420, 198, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BossLevelThumbnail_Locked"] = createTextureRegion(990, 1008, 198, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["NormalLevelThumbnail_Cleared"] = createTextureRegion(0, 330, 124, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["NormalLevelThumbnail_Playable"] = createTextureRegion(124, 78, 124, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["NormalLevelThumbnail_Unplayable"] = createTextureRegion(0, 78, 124, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BackgroundType_Upper"] = createTextureRegion(0, 0, PIXEL_WIDTH_FOR_BACKGROUND, 2048, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BackgroundType_Mid"] = createTextureRegion(0, 1024, PIXEL_WIDTH_FOR_BACKGROUND, 1024, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BackgroundType_Lower"] = createTextureRegion(0, 320, PIXEL_WIDTH_FOR_BACKGROUND, 1728, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BackgroundType_WaterBack"] = createTextureRegion(0, 168, PIXEL_WIDTH_FOR_BACKGROUND, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["BackgroundType_WaterFront"] = createTextureRegion(0, 248, PIXEL_WIDTH_FOR_BACKGROUND, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["MidgroundType_TreeOne"] = createTextureRegion(1940, 224, 512, 672, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["MidgroundType_TreeTwo"] = createTextureRegion(2468, 96, 656, 800, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["MidgroundType_TreeThree"] = createTextureRegion(3156, 48, 928, 848, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["MidgroundType_DeepCaveColumnSmall"] = createTextureRegion(3052, 1444, 96, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["MidgroundType_DeepCaveColumnMedium"] = createTextureRegion(3884, 900, 128, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["MidgroundType_DeepCaveColumnBig"] = createTextureRegion(3310, 900, 256, 832, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveExtraDeepEndLeft"] = createTextureRegion(0, 1132, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveExtraDeepSmall"] = createTextureRegion(128, 1132, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveExtraDeepMedium"] = createTextureRegion(640, 1132, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveExtraDeepLarge"] = createTextureRegion(1664, 1132, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveExtraDeepEndRight"] = createTextureRegion(3712, 1132, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveDeepEndLeft"] = createTextureRegion(0, 1264, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveDeepSmall"] = createTextureRegion(128, 1264, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveDeepMedium"] = createTextureRegion(640, 1264, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveDeepLarge"] = createTextureRegion(1664, 1264, 2048, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveDeepEndRight"] = createTextureRegion(3712, 1264, 128, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveEndLeft"] = createTextureRegion(0, 1912, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveSmall"] = createTextureRegion(128, 1912, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveMedium"] = createTextureRegion(640, 1912, 1024, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveLarge"] = createTextureRegion(1664, 1912, 2048, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveEndRight"] = createTextureRegion(3712, 1912, 128, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveRaisedEndLeft"] = createTextureRegion(0, 1396, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveRaisedSmall"] = createTextureRegion(128, 1396, 512, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveRaisedMedium"] = createTextureRegion(640, 1396, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveRaisedLarge"] = createTextureRegion(1664, 1396, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_CaveRaisedEndRight"] = createTextureRegion(3712, 1396, 128, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithCaveEndLeft"] = createTextureRegion(0, 2300, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithCaveSmall"] = createTextureRegion(128, 2300, 512, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithCaveMedium"] = createTextureRegion(640, 2300, 1024, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithCaveLarge"] = createTextureRegion(1664, 2300, 2048, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithCaveEndRight"] = createTextureRegion(3712, 2300, 128, 192, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithoutCaveEndLeft"] = createTextureRegion(0, 2496, 128, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithoutCaveSmall"] = createTextureRegion(128, 2496, 512, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithoutCaveMedium"] = createTextureRegion(640, 2496, 1024, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithoutCaveLarge"] = createTextureRegion(1664, 2496, 2048, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassWithoutCaveEndRight"] = createTextureRegion(3712, 2496, 128, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassPitSmall"] = createTextureRegion(2208, 880, 512, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassPitMedium"] = createTextureRegion(2954, 880, 1024, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassPitLarge"] = createTextureRegion(0, 880, 2048, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GroundType_GrassPitExtraLarge"] = createTextureRegion(0, 2496, 4096, 1600, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ExitGroundType_GrassWithCaveSmallExitMid"] = createTextureRegion(0, 0, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ExitGroundType_GrassWithCaveSmallExitEnd"] = createTextureRegion(3500, 228, 512, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ExitGroundType_CaveSmallExit"] = createTextureRegion(0, 228, 512, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["HoleType_GrassTileLeft"] = createTextureRegion(0, 1400, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["HoleType_GrassTileCenter"] = createTextureRegion(0, 1772, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["HoleType_GrassTileRight"] = createTextureRegion(0, 2144, 256, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["HoleType_Grass"] = createTextureRegion(0, 2528, 256, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["HoleType_Cave"] = createTextureRegion(0, 2788, 272, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_GrassPlatformLeft"] = createTextureRegion(0, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_GrassPlatformCenter"] = createTextureRegion(64, 0, 224, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_GrassPlatformRight"] = createTextureRegion(288, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_CavePlatformLeft"] = createTextureRegion(378, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_CavePlatformCenter"] = createTextureRegion(442, 0, 224, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_CavePlatformRight"] = createTextureRegion(666, 0, 64, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_RockLarge"] = createTextureRegion(0, 130, 704, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_RockMedium"] = createTextureRegion(0, 686, 384, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_RockSmall"] = createTextureRegion(3624, 2158, 384, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_StumpBig"] = createTextureRegion(388, 696, 256, 288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_StumpSmall"] = createTextureRegion(0, 1080, 320, 272, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_EndSign"] = createTextureRegion(704, 688, 112, 160, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_ThornsLeft"] = createTextureRegion(1156, 1196, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_ThornsCenterSmall"] = createTextureRegion(1588, 1196, 176, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_ThornsCenterBig"] = createTextureRegion(1236, 1196, 352, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_ThornsRight"] = createTextureRegion(1764, 1196, 80, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_LogVerticalTall"] = createTextureRegion(452, 1004, 128, 176, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_LogVerticalShort"] = createTextureRegion(452, 1190, 128, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeGrassSingle"] = createTextureRegion(1904, 900, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeGrassFour"] = createTextureRegion(2000, 900, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeGrassEight"] = createTextureRegion(2288, 900, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveSingle"] = createTextureRegion(1904, 1108, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveFour"] = createTextureRegion(2000, 1108, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveEight"] = createTextureRegion(2288, 1108, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveCeilingSingle"] = createTextureRegion(1904, 1004, 96, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveCeilingFour"] = createTextureRegion(2000, 1004, 288, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeCaveCeilingEight"] = createTextureRegion(2288, 1004, 544, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeWallSingle"] = createTextureRegion(4000, 3140, 96, 64, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeWallFour"] = createTextureRegion(4000, 3824, 96, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeWallEight"] = createTextureRegion(4000, 3248, 96, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeStar"] = createTextureRegion(890, 1162, 224, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_GiantTree"] = createTextureRegion(832, 0, 1088, 1040, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_GiantPerchTree"] = createTextureRegion(3008, 2080, 1088, 1040, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeTower"] = createTextureRegion(2892, 3200, 512, 896, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikeTowerBg"] = createTextureRegion(3408, 3200, 512, 896, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikedBallRolling"] = createTextureRegion(3440, 2104, 512, 480, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["ForegroundObjectType_SpikedBall"] = createTextureRegion(3440, 2588, 512, 480, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["SnakeBody_Left"] = createTextureRegion(0, 548, 3488, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["SnakeBody_Right"] = createTextureRegion(0, 1640, 3488, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EndBossSnakeState_Sleeping_Awakening"] = createTextureRegion(0, 0, 864, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EndBossSnakeState_OpenMouthLeft_ChargingLeft"] = createTextureRegion(1728, 0, 864, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EndBossSnakeState_Pursuing_Waiting_Damaged"] = createTextureRegion(0, 0, 864, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EndBossSnakeState_OpenMouthRight_ChargingRight"] = createTextureRegion(1728, 0, 864, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EndBossSnakeState_DeadSpiritReleasing_Dead"] = createTextureRegion(2592, 2720, 864, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CollectibleItemType_Carrot_Not_Collected"] = createTextureRegion(0, 3204, 218, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CollectibleItemType_GoldenCarrot_Previously_Collected"] = createTextureRegion(2286, 3204, 254, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["CollectibleItemType_GoldenCarrot_Not_Collected"] = createTextureRegion(2032, 3204, 254, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GameButtonType_ToggleMusic"] = createTextureRegion(1244, 72, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["GameButtonType_ToggleSound"] = createTextureRegion(1244, 204, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["GameButtonType_BackToTitle"] = createTextureRegion(1800, 0, 104, 104, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["GameButtonType_Leaderboards"] = createTextureRegion(1904, 0, 104, 104, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["GameButtonType_ViewOpeningCutscene"] = createTextureRegion(1152, 0, 96, 64, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["GameButtonType_BackToLevelSelect"] = createTextureRegion(0, 0, 104, 104, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);
    m_textureRegions["GameButtonType_ContinueToLevelSelect"] = createTextureRegion(804, 0, 104, 104, TEXTURE_SIZE_1024, TEXTURE_SIZE_1024);
    m_textureRegions["GameButtonType_LevelEditor"] = createTextureRegion(2848, 2644, 190, 62, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GameButtonType_SpriteTester"] = createTextureRegion(3048, 2644, 190, 62, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GameHudCarrot_Golden"] = createTextureRegion(1904, 1254, 96, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["GameHudCarrot_Normal"] = createTextureRegion(884, 1054, 96, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096);
    m_textureRegions["EditorEntitiesPanel"] = createTextureRegion(0, 46, 592, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["EditorActionsPanel"] = createTextureRegion(840, 46, 592, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["TrashCan_Highlighted"] = createTextureRegion(128, 1454, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["TrashCan_Normal"] = createTextureRegion(0, 1454, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["SelectorPanel"] = createTextureRegion(1538, 12, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["OffsetPanel"] = createTextureRegion(1538, 542, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["ConfirmResetPanel"] = createTextureRegion(938, 1484, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    m_textureRegions["ConfirmExitPanel"] = createTextureRegion(1538, 1072, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    /// Animations
    m_animations["TitlePanel_Lightning_Striking"] = createAnimation(0, 0, 1280, 720, 3840, 1448, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 6, 0, 8);
    m_animations["TitlePanel_Lightning_Not_Striking"] = createAnimation(0, 1456, 1280, 720, 2560, 720, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
    m_animations["CutsceneEffectType_POW"] = createAnimation(0, 1270, 1952, 1412, 3904, 2824, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.20f, 3);
    m_animations["AbilitySlot_Unlocking"] = createAnimation(0, 458, 118, 114, 1180, 114, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 10);
    m_animations["AbilitySlotType_Drill"] = createAnimation(0, 572, 118, 114, 1298, 114, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 11);
    m_animations["AbilitySlotType_Dash"] = createAnimation(0, 686, 118, 114, 1298, 114, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 11);
    m_animations["BossLevelThumbnail_Unlocking"] = createAnimation(0, 804, 198, 204, 1980, 408, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 13);
    m_animations["BossLevelThumbnail_Selected_Cleared"] = createAnimation(198, 1832, 198, 204, 1782, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 9);
    m_animations["BossLevelThumbnail_Selected"] = createAnimation(0, 1216, 198, 204, 1782, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 9);
    m_animations["BossLevelThumbnail_Selecting_Cleared"] = createAnimation(1386, 1628, 198, 204, 792, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 4);
    m_animations["BossLevelThumbnail_Selecting"] = createAnimation(198, 1420, 198, 204, 594, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 3);
    m_animations["BossLevelThumbnail_Clearing"] = createAnimation(0, 1628, 198, 204, 1386, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 7);
    m_animations["BossLevelThumbnail_Locked_Jaw_Moving"] = createAnimation(990, 1008, 198, 204, 990, 204, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 5, 3);
    m_animations["NormalLevelThumbnail_Selected_Cleared"] = createAnimation(744, 330, 124, 124, 1240, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 10);
    m_animations["NormalLevelThumbnail_Selected"] = createAnimation(868, 78, 124, 124, 1116, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 9);
    m_animations["NormalLevelThumbnail_Selecting_Cleared"] = createAnimation(0, 330, 124, 124, 744, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 6);
    m_animations["NormalLevelThumbnail_Selecting"] = createAnimation(124, 78, 124, 124, 744, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 6);
    m_animations["NormalLevelThumbnail_Clearing"] = createAnimation(0, 206, 124, 124, 1364, 124, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 11);
    m_animations["GoldenCarrotsMarker_0"] = createAnimation(0, 0, 118, 74, 944, 74, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 8);
    m_animations["GoldenCarrotsMarker_1"] = createAnimation(0, 0, 118, 74, 1062, 74, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 9);
    m_animations["GoldenCarrotsMarker_2"] = createAnimation(0, 0, 118, 74, 1180, 74, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 10);
    m_animations["GoldenCarrotsMarker_3"] = createAnimation(0, 0, 118, 74, 1298, 74, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 11);
    m_animations["GoldenCarrotsMarker_4"] = createAnimation(0, 0, 118, 74, 1416, 74, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 12);
    m_animations["SpendGoldenCarrotsBubble_Has_Enough"] = createAnimation(0, 748, 234, 184, 1872, 552, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 21, 3);
    m_animations["SpendGoldenCarrotsBubble_Not_Enough"] = createAnimation(0, 1304, 234, 184, 1872, 552, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 21, 3);
    m_animations["ExitGroundType_CaveDeepSmallWaterfall"] = createAnimation(0, 616, 512, 448, 1536, 448, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 3);
    m_animations["ExitGroundCoverType_Grass"] = createAnimation(1024, 0, 512, 224, 3072, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.08333333333333f, 6);
    m_animations["ExitGroundCoverType_Cave"] = createAnimation(514, 228, 512, 384, 2560, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 5);
    m_animations["HoleCoverType_GrassTileLeft"] = createAnimation(256, 1400, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["HoleCoverType_GrassTileCenter"] = createAnimation(256, 1772, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["HoleCoverType_GrassTileRight"] = createAnimation(256, 2144, 256, 368, 2560, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["HoleCoverType_Grass"] = createAnimation(256, 2528, 256, 256, 3072, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04166666666667f, 12);
    m_animations["HoleCoverType_Cave"] = createAnimation(272, 2788, 272, 384, 2720, 384, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["ForegroundObjectType_RockSmallCracked"] = createAnimation(384, 3236, 384, 368, 2304, 368, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
    m_animations["ForegroundObjectType_JumpSpringLightFlush"] = createAnimation(2904, 1804, 160, 112, 1120, 112, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05142857142857f, 7);
    m_animations["ForegroundObjectType_JumpSpringLight"] = createAnimation(2126, 3960, 96, 80, 480, 80, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 4);
    m_animations["ForegroundObjectType_JumpSpringMedium"] = createAnimation(0, 3668, 272, 144, 1904, 144, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05142857142857, 7);
    m_animations["ForegroundObjectType_JumpSpringHeavy"] = createAnimation(272, 3816, 272, 224, 1904, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 6);
    m_animations["ForegroundObjectType_VerticalSaw"] = createAnimation(3590, 2556, 240, 528, 480, 528, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 2);
    m_animations["ForegroundObjectType_GiantShakingTree"] = createAnimation(832, 0, 1088, 1040, 3264, 3120, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.025f, 7);
    m_animations["ForegroundObjectType_SpikedBallChain"] = createAnimation(0, 1472, 1616, 1312, 3232, 2624, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    m_animations["CountHissWithMina_Left"] = createAnimation(2048, 3072, 512, 512, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 6);
    m_animations["CountHissWithMina_Right"] = createAnimation(2048, 3072, 512, 512, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 7);
    m_animations["SnakeSpirit"] = createAnimation(0, 2188, 820, 360, 3280, 1080, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 12);
    m_animations["SnakeHeadImpact"] = createAnimation(0, 3584, 512, 512, 4096, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 8);
    m_animations["SnakeSkin"] = createAnimation(0, 548, 864, 544, 3456, 1088, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 7);
    m_animations["SnakeEye"] = createAnimation(2756, 304, 118, 118, 354, 118, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    m_animations["SnakeTonque_Left"] = createAnimation(2596, 0, 554, 78, 1108, 234, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 5, 3);
    m_animations["SnakeTonque_Right"] = createAnimation(2792, 0, 554, 78, 1108, 234, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 5, 3);
    m_animations["EndBossSnakeState_OpeningMouthLeft"] = createAnimation(0, 0, 864, 544, 2592, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    m_animations["EndBossSnakeState_OpeningMouthRight"] = createAnimation(0, 0, 864, 544, 2592, 544, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    {
        Animation* anim = createAnimation(0, 0, 864, 544, 3456, 3264, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 24);
        anim->setFrameTimes(24, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f);
        m_animations["EndBossSnakeState_Dying"] = anim;
    }
    m_animations["EnemyType_MushroomGround"] = createAnimation(0, 0, 112, 128, 448, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.12f, 4);
    m_animations["EnemyType_MushroomCeiling"] = createAnimation(0, 132, 112, 128, 448, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.12f, 4);
    m_animations["EnemyType_SnakeGrunt"] = createAnimation(0, 332, 128, 96, 512, 96, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 4);
    m_animations["EnemyType_Sparrow"] = createAnimation(0, 692, 160, 160, 1120, 160, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.08f, 7);
    m_animations["EnemyType_Toad_Dead_Part_1_Jon_Is_Vampire"] = createAnimation(1024, 2144, 512, 256, 1024, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 2);
    m_animations["EnemyType_Toad_Dead_Part_1_Jon_Is_Rabbit"] = createAnimation(0, 1888, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 5);
    m_animations["EnemyType_Toad_Dead_Part_2"] = createAnimation(2048, 1888, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 7);
    m_animations["EnemyType_Toad_Eating_Jon_Is_Vampire"] = createAnimation(2048, 1372, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 8);
    m_animations["EnemyType_Toad_Eating_Jon_Is_Rabbit"] = createAnimation(0, 1372, 512, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 8);
    m_animations["EnemyType_Toad"] = createAnimation(492, 0, 512, 256, 3584, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.15f, 7);
    m_animations["EnemyType_Fox_Is_Dead"] = createAnimation(0, 2664, 256, 512, 2560, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["EnemyType_Fox_Is_Hitting_Left"] = createAnimation(2080, 260, 256, 256, 1792, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 11);
    m_animations["EnemyType_Fox_Is_Hitting_Right"] = createAnimation(0, 2404, 256, 256, 2816, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 11);
    m_animations["EnemyType_Fox_Is_Being_Hit"] = createAnimation(3072, 2404, 256, 256, 768, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 6);
    m_animations["EnemyType_Fox_Left"] = createAnimation(3104, 516, 256, 256, 768, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.15f, 3);
    m_animations["EnemyType_Fox_Right"] = createAnimation(3104, 772, 256, 256, 768, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.15f, 3);
    m_animations["EnemyType_BigMushroomGround_Bouncing_Back"] = createAnimation(2560, 3298, 256, 208, 1024, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["EnemyType_BigMushroomGround_Bouncing_On"] = createAnimation(1536, 3298, 256, 208, 1024, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["EnemyType_BigMushroomGround"] = createAnimation(0, 3298, 256, 208, 1536, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["EnemyType_BigMushroomCeiling_Bouncing_Back"] = createAnimation(2560, 3510, 256, 208, 1024, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["EnemyType_BigMushroomCeiling_Bouncing_On"] = createAnimation(1536, 3510, 256, 208, 1024, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["EnemyType_BigMushroomCeiling"] = createAnimation(0, 3510, 256, 208, 1536, 208, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["MovingSnakeGrunt_Jumping"] = createAnimation(768, 3850, 256, 128, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["MovingSnakeGrunt_Falling"] = createAnimation(1792, 3850, 256, 128, 512, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 2);
    m_animations["MovingSnakeGrunt_About_To_Jump"] = createAnimation(0, 3850, 256, 128, 768, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    m_animations["MovingSnakeGrunt_Landing"] = createAnimation(2304, 3850, 256, 128, 1024, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 4);
    m_animations["MovingSnakeGrunt_Pausing"] = createAnimation(1536, 3722, 256, 128, 1280, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 5);
    m_animations["MovingSnakeGrunt"] = createAnimation(0, 3722, 256, 128, 1536, 128, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["EnemySpiritType_Snake"] = createAnimation(0, 432, 208, 256, 2080, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 10);
    m_animations["EnemySpiritType_Sparrow"] = createAnimation(0, 856, 256, 512, 3072, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04166666666667f, 12);
    m_animations["CollectibleItemType_Carrot_Collected"] = createAnimation(0, 3204, 218, 224, 1744, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 8);
    m_animations["CollectibleItemType_GoldenCarrot_Collected"] = createAnimation(0, 3432, 254, 224, 2794, 224, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 11);
    m_animations["GoldenCarrotTwinkle"] = createAnimation(3004, 2004, 74, 78, 518, 78, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 7);
    {
        Animation* anim = createAnimation(2048, 2048, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        anim->setFrameTimes(19, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        m_animations["Jon_isTransformingIntoVampire"] = anim;
    }
    {
        Animation* anim = createAnimation(2048, 2816, 256, 256, 2048, 768, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 19);
        anim->setFrameTimes(19, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.0078125f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        m_animations["Jon_isRevertingToRabbit"] = anim;
    }
    m_animations["Jon_Vampire_Death"] = createAnimation(2048, 3584, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06666666666667f, 15);
    m_animations["Jon_Vampire_PushedBack"] = createAnimation(512, 256, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 6);
    m_animations["Jon_Vampire_Idle"] = createAnimation(0, 1792, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.125f, 8);
    m_animations["Jon_Vampire_Running"] = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.056f, 10);
    m_animations["Jon_Vampire_UpwardThrust"] = createAnimation(0, 3072, 256, 512, 1536, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.04545454545455f, 11);
    {
        Animation* anim = createAnimation(768, 512, 768, 256, 3072, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 14);
        anim->setFrameTimes(14, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.1f, 0.1f, 0.40f, 0.1f);
        m_animations["Jon_Vampire_Dash"] = anim;
    }
    m_animations["Jon_Vampire_DoubleJumping"] = createAnimation(0, 512, 256, 256, 768, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 6);
    m_animations["Jon_Vampire_Gliding"] = createAnimation(0, 1024, 256, 256, 512, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 2);
    m_animations["Jon_Vampire_Falling"] = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
    m_animations["Jon_Vampire_Landing"] = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.066f, 3);
    m_animations["Jon_Rabbit_Death"] = createAnimation(0, 3072, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 10);
    m_animations["Jon_Rabbit_WarmUp"] = createAnimation(0, 2048, 256, 256, 2048, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 28);
    m_animations["Jon_Rabbit_PushedBack"] = createAnimation(512, 1024, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 6);
    m_animations["Jon_Rabbit_Idle"] = createAnimation(0, 1792, 256, 256, 1536, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6, 2);
    m_animations["Jon_Rabbit_Running"] = createAnimation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.064f, 10);
    m_animations["Jon_Rabbit_Jumping"] = createAnimation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.09f, 7);
    m_animations["Jon_Rabbit_DoubleJumping"] = createAnimation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.07f, 9);
    m_animations["Jon_Rabbit_Falling"] = createAnimation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.06f, 3);
    m_animations["Jon_Rabbit_Landing"] = createAnimation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.05f, 4);
    m_animations["Jon_Rabbit_SpinningBackFist"] = createAnimation(2048, 0, 256, 256, 2048, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 7);
    m_animations["Jon_Rabbit_Burrow"] = createAnimation(1792, 256, 256, 256, 2304, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.06f, 9);
    m_animations["Jon_Rabbit_Stomp"] = createAnimation(2048, 1024, 256, 256, 2048, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.05f, 11, 9);
    m_animations["Jon_Rabbit_LandingRoll"] = createAnimation(2816, 1280, 256, 256, 1280, 256, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 5);
    m_animations["DustCloud"] = createAnimation(1100, 1644, 116, 60, 580, 60, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.08f, 5);
    m_animations["MidBossOwlState_Sleeping"] = createAnimation(0, 0, 638, 572, 638, 2288, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.15f, 4);
    m_animations["MidBossOwlState_Awakening"] = createAnimation(0, 2288, 638, 572, 638, 1144, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 2);
    m_animations["MidBossOwlState_Screeching"] = createAnimation(0, 3432, 638, 572, 1276, 572, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
    m_animations["MidBossOwlState_Pursuing_Damage_2"] = createAnimation(0, 3072, 512, 512, 3072, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["MidBossOwlState_Pursuing_Damage_1"] = createAnimation(0, 1536, 512, 512, 3072, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["MidBossOwlState_Pursuing_Damage_0"] = createAnimation(0, 0, 512, 512, 3072, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 6);
    m_animations["MidBossOwlState_SwoopingDown_Damage_2"] = createAnimation(3072, 3072, 512, 512, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
    m_animations["MidBossOwlState_SwoopingDown_Damage_1"] = createAnimation(3072, 1536, 512, 512, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
    m_animations["MidBossOwlState_SwoopingDown_Damage_0"] = createAnimation(3072, 0, 512, 512, 1024, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 2);
    m_animations["MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_2"] = createAnimation(0, 3584, 512, 512, 3584, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 7, 1);
    m_animations["MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_1"] = createAnimation(0, 2048, 512, 512, 3584, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 7, 1);
    m_animations["MidBossOwlState_FlyingAwayAfterCatchingJon_Damage_0"] = createAnimation(0, 512, 512, 512, 3584, 512, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 7, 1);
    {
        Animation* anim = createAnimation(0, 2560, 512, 512, 4096, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 8);
        anim->setFrameTimes(8, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.1f, 0.1f, 0.1f);
        m_animations["MidBossOwlState_SlammingIntoTree_Damage_2"] = anim;
    }
    {
        Animation* anim = createAnimation(0, 1024, 512, 512, 4096, 1024, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 8);
        anim->setFrameTimes(8, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.1f, 0.1f, 0.1f);
        m_animations["MidBossOwlState_SlammingIntoTree_Damage_1"] = anim;
    }
    {
        Animation* anim = createAnimation(0, 0, 720, 800, 3600, 4000, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 22);
        anim->setFrameTimes(22, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
        m_animations["MidBossOwlState_Dying_Dead"] = anim;
    }
    m_animations["Bat"] = createAnimation(2048, 0, 164, 158, 1312, 316, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 15, 8);
    m_animations["BatInstruction_Opening"] = createAnimation(0, 3588, 498, 394, 1494, 394, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, false, 0.10f, 3);
    m_animations["BatInstructionType_Tap"] = createAnimation(1498, 398, 498, 394, 3984, 394, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 5);
    m_animations["BatInstructionType_TapHold"] = createAnimation(0, 0, 498, 394, 3984, 788, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 11);
    m_animations["BatInstructionType_SwipeDown"] = createAnimation(2048, 2048, 498, 394, 1992, 788, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 8);
    m_animations["BatInstructionType_SwipeRight"] = createAnimation(0, 2048, 498, 394, 1992, 788, TEXTURE_SIZE_4096, TEXTURE_SIZE_4096, true, 0.10f, 8);
}

TextureRegion& Assets::get(TitlePanel* panel)
{
    if (panel->isLightningStriking())
    {
        static Animation anim = findAnimation("TitlePanel_Lightning_Striking");
        return anim.getTextureRegion(panel->getStateTime());
    }
    else
    {
        static Animation anim = findAnimation("TitlePanel_Lightning_Not_Striking");
        return anim.getTextureRegion(panel->getStateTime());
    }
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
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Mid:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Mid");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_Lower:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_Lower");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_WaterBack:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_WaterBack");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
            return tr;
        }
        case BackgroundType_WaterFront:
        {
            static TextureRegion tr = findTextureRegion("BackgroundType_WaterFront");
            initTextureRegion(tr, background->getTextureRegionX(), PIXEL_WIDTH_FOR_BACKGROUND, TEXTURE_SIZE_2048);
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
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_TreeTwo");
            return tr;
        }
        case MidgroundType_TreeThree:
        {
            static TextureRegion tr = findTextureRegion("MidgroundType_TreeThree");
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
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_ThornsCenterSmall");
            return tr;
        }
        case ForegroundObjectType_ThornsCenterBig:
        {
            static TextureRegion tr = findTextureRegion("ForegroundObjectType_ThornsCenterBig");
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

Animation* Assets::createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames)
{
    if (m_isUsingCompressedTextureSet
        && textureWidth > TEXTURE_SIZE_2048
        && textureHeight > TEXTURE_SIZE_2048)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        animationWidth /= 2.0;
        animationHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
    }
    
    return new Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, numFrames);
}

Animation* Assets::createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame, int yPadding)
{
    if (m_isUsingCompressedTextureSet
        && textureWidth > TEXTURE_SIZE_2048
        && textureHeight > TEXTURE_SIZE_2048)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        animationWidth /= 2.0;
        animationHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
        yPadding /= 2.0;
    }
    
    return new Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, frameTime, numFrames, firstLoopingFrame, yPadding);
}

TextureRegion* Assets::createTextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
{
    if (m_isUsingCompressedTextureSet
        && textureWidth > TEXTURE_SIZE_2048
        && textureHeight > TEXTURE_SIZE_2048)
    {
        x /= 2.0;
        y /= 2.0;
        regionWidth /= 2.0;
        regionHeight /= 2.0;
        textureWidth /= 2.0;
        textureHeight /= 2.0;
    }
    
    return new TextureRegion(x, y, regionWidth, regionHeight, textureWidth, textureHeight);
}

TextureRegion& Assets::findTextureRegion(std::string key)
{
    auto q = m_textureRegions.find(key);
    
    assert(q != m_textureRegions.end());
    
    TextureRegion* tr = q->second;
    
    return *tr;
}

Animation& Assets::findAnimation(std::string key)
{
    auto q = m_animations.find(key);
    
    assert(q != m_animations.end());
    
    Animation* anim = q->second;
    
    return *anim;
}

TextureRegion& Assets::findTextureRegion(std::string key, float stateTime)
{
    auto q = m_textureRegions.find(key);
    
    if (q != m_textureRegions.end())
    {
        TextureRegion* tr = q->second;
        
        return *tr;
    }
    else
    {
        auto q2 = m_animations.find(key);
        
        assert(q2 != m_animations.end());
        
        Animation* anim = q2->second;
        
        return anim->getTextureRegion(stateTime);
    }
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
