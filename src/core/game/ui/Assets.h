//
//  Assets.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Assets__
#define __nosfuratu__Assets__

#include "TextureRegion.h"
#include "Animation.h"
#include "Vector2D.h"
#include "Rectangle.h"
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

#include <map>
#include <vector>

#define ASSETS (Assets::getInstance())
#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

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
class GameHudCarrot;
class Bat;
class BatInstruction;

class Assets
{
public:
	static Assets * getInstance();
    
    void initializeAssets();
    
    TextureRegion& get(TitlePanel* panel);
    
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
    
    TextureRegion& get(MidBossOwl* owl);
    
    TextureRegion& get(Bat* bat);
    
    TextureRegion& get(BatInstruction* batInstruction);
    
    TextureRegion& get(GameButton* backButton);
    
    TextureRegion& get(GameHudCarrot* gameHudCarrot);
    
    TextureRegion& get(LevelEditorEntitiesPanel* levelEditorEntitiesPanel);
    
    TextureRegion& get(LevelEditorActionsPanel* levelEditorActionsPanel);
    
    TextureRegion& get(TrashCan* trashCan);
    
    TextureRegion& get(LevelSelectorPanel* panel);
    
    TextureRegion& get(OffsetPanel* panel);
    
    TextureRegion& get(ConfirmResetPanel* panel);
    
    TextureRegion& get(ConfirmExitPanel* panel);
    
    short getFirstSoundId();
    
    void addSoundIdToPlayQueue(short soundId);
    
    void forceAddSoundIdToPlayQueue(short soundId);
    
    void eraseFirstSoundId();
    
    short getFirstMusicId();
    
    void addMusicIdToPlayQueue(short musicId);
    
    void eraseFirstMusicId();
    
    bool isUsingCompressedTextureSet()
    {
        return m_isUsingCompressedTextureSet;
    }
    
    void setUsingCompressedTextureSet(bool isUsingCompressedTextureSet)
    {
        m_isUsingCompressedTextureSet = isUsingCompressedTextureSet;
    }
    
    bool isUsingDesktopTextureSet()
    {
        return m_isUsingDesktopTextureSet;
    }
    
    void setUsingDesktopTextureSet(bool isUsingDesktopTextureSet)
    {
        m_isUsingDesktopTextureSet = isUsingDesktopTextureSet;
    }
    
    bool isMusicEnabled() { return m_isMusicEnabled; }
    
    void setMusicEnabled(bool isMusicEnabled)
    {
        m_isMusicEnabled = isMusicEnabled;
        
        addMusicIdToPlayQueue(m_isMusicEnabled ? MUSIC_RESUME : MUSIC_STOP);
    }
    
    bool isSoundEnabled() { return m_isSoundEnabled; }
    
    void setSoundEnabled(bool isSoundEnabled) { m_isSoundEnabled = isSoundEnabled; }

private:
    std::map<std::string, TextureRegion*> m_textureRegions;
    std::map<std::string, Animation*> m_animations;
    std::vector<short> m_sSoundIds;
    std::vector<short> m_sMusicIds;
    bool m_isUsingCompressedTextureSet;
    bool m_isUsingDesktopTextureSet;
    bool m_isMusicEnabled;
    bool m_isSoundEnabled;
    
    Animation* createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames);
    
    Animation* createAnimation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame = 0, int yPadding = 0);
    
    TextureRegion* createTextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight);
    
    TextureRegion& findTextureRegion(std::string key);
    
    Animation& findAnimation(std::string key);
    
	void initTextureRegion(TextureRegion& tr, int x, int regionWidth, int textureWidth);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    Assets() : m_isUsingCompressedTextureSet(false), m_isMusicEnabled(true), m_isSoundEnabled(true) {}
    Assets(const Assets&);
    Assets& operator=(const Assets&);
};

#endif /* defined(__nosfuratu__Assets__) */
