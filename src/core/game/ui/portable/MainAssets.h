//
//  MainAssets.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__MainAssets__
#define __nosfuratu__MainAssets__

#define MAIN_ASSETS (MainAssets::getInstance())

class TextureRegion;
class Background;
class Midground;
class Ground;
class ExitGround;
class ExitGroundCover;
class Hole;
class HoleCover;
class DustCloud;
class Jon;
class JonShadow;
class MidBossOwl;
class GameButton;
class TrashCan;
class LevelSelectorPanel;
class OffsetPanel;
class ConfirmResetPanel;
class ConfirmExitPanel;
class CollectibleItem;
class GoldenCarrotTwinkle;
class LevelEditorActionsPanel;
class LevelEditorEntitiesPanel;
class Enemy;
class EnemySpirit;
class ForegroundObject;
class TitlePanel;
class CutsceneEffect;
class CutscenePanel;
class WorldMapPanel;
class AbilitySlot;
class LevelThumbnail;
class GoldenCarrotsMarker;
class SpendGoldenCarrotsBubble;
class CountHissWithMina;
class SnakeSpirit;
class SnakeHeadImpact; 
class SnakeSkin;
class SnakeEye;
class SnakeTonque;
class SnakeBody;
class EndBossSnake;
class BatInstruction;
class BatPanel;
class GameHudItem;
class Bat;
class BatInstruction;
class TextureSelectorPanel;
class BigCloud;
class SmallCloud;
class Lightning;
class NosfuratuLogoLightEffect;
class NosfuratuLogo;
class CastleLightEffect;
class Castle;
class ForegroundCoverObject;

class MainAssets
{
public:
	static MainAssets* getInstance();
    
    TextureRegion& get(TitlePanel* panel);
    
    TextureRegion& get(BigCloud* bigCloud);
    
    TextureRegion& get(SmallCloud* smallCloud);
    
    TextureRegion& get(Lightning* lightning);
    
    TextureRegion& get(NosfuratuLogoLightEffect* nosfuratuLogoLightEffect);
    
    TextureRegion& get(NosfuratuLogo* nosfuratuLogo);
    
    TextureRegion& get(CastleLightEffect* castleLightEffect);
    
    TextureRegion& get(Castle* castle);
    
    TextureRegion& get(CutsceneEffect* effect);
    
    TextureRegion& get(CutscenePanel* panel);
    
    TextureRegion& get(WorldMapPanel* panel);
    
    TextureRegion& get(AbilitySlot* abilitySlot);
    
    TextureRegion& get(LevelThumbnail* thumbnail);
    
    TextureRegion& get(GoldenCarrotsMarker* goldenCarrotsMarker);
    
    TextureRegion& get(SpendGoldenCarrotsBubble* spendGoldenCarrotsBubble);
    
    TextureRegion& get(Background* background);
    
    TextureRegion& get(Midground* midground);
    
    TextureRegion& get(Ground* ground);
    
    TextureRegion& get(ExitGround* exitGround);
    
    TextureRegion& get(ExitGroundCover* exitGroundCover);
    
    TextureRegion& get(Hole* hole);
    
    TextureRegion& get(HoleCover* holeCover);
    
    TextureRegion& get(ForegroundObject* foregroundObject);
    
    TextureRegion& get(ForegroundCoverObject* foregroundCoverObject);
    
    TextureRegion& get(CountHissWithMina* countHissWithMina);
    
	TextureRegion& get(SnakeSpirit* snakeSpirit);
    
    TextureRegion& get(SnakeHeadImpact* snakeHeadImpact);

    TextureRegion& get(SnakeSkin* snakeSkin);
    
    TextureRegion& get(SnakeEye* snakeEye);
    
    TextureRegion& get(SnakeTonque* snakeTonque);
    
    TextureRegion& get(SnakeBody* snakeBody);
    
    TextureRegion& get(EndBossSnake* endBossSnake);
    
    TextureRegion& get(Enemy* enemy);
    
    TextureRegion& get(EnemySpirit* spirit);
    
    TextureRegion& get(CollectibleItem* collectibleItem);
    
    TextureRegion& get(GoldenCarrotTwinkle* goldenCarrotTwinkle);
    
    TextureRegion& get(Jon* jon);
    
    TextureRegion& get(DustCloud* dustCloud);
    
    TextureRegion& get(JonShadow* jonShadow);
    
    TextureRegion& get(MidBossOwl* owl);
    
    TextureRegion& get(Bat* bat);
    
    TextureRegion& get(BatInstruction* batInstruction);
    
    TextureRegion& get(GameButton* backButton);
    
    TextureRegion& get(GameHudItem* gameHudItem);
    
    TextureRegion& get(LevelEditorEntitiesPanel* levelEditorEntitiesPanel);
    
    TextureRegion& get(LevelEditorActionsPanel* levelEditorActionsPanel);
    
    TextureRegion& get(TrashCan* trashCan);
    
    TextureRegion& get(LevelSelectorPanel* panel);
    
    TextureRegion& get(TextureSelectorPanel* panel);
    
    TextureRegion& get(OffsetPanel* panel);
    
    TextureRegion& get(ConfirmResetPanel* panel);
    
    TextureRegion& get(ConfirmExitPanel* panel);
    
    bool isUsingCompressedTextureSet();
    
    void setUsingCompressedTextureSet(bool isUsingCompressedTextureSet);
    
    bool isUsingDesktopTextureSet();
    
    void setUsingDesktopTextureSet(bool isUsingDesktopTextureSet);
    
    bool isUsingGamePadTextureSet();
    
    void setUsingGamePadTextureSet(bool isUsingGamePadTextureSet);

private:
    bool m_isUsingCompressedTextureSet;
    bool m_isUsingDesktopTextureSet;
    bool m_isUsingGamePadTextureSet;
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    MainAssets();
    MainAssets(const MainAssets&);
    MainAssets& operator=(const MainAssets&);
};

#endif /* defined(__nosfuratu__MainAssets__) */
