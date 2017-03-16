//
//  MainScreenWorldMap.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenWorldMap.h"

#include "MainScreen.h"
#include "GameButton.h"
#include "WorldMapPanel.h"

#include "MainScreenLevels.h"
#include "NGAudioEngine.h"
#include "State.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "MainScreenLevelEditor.h"
#include "MainScreenTransitions.h"
#include "MainScreenTitle.h"
#include "MathUtil.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "KeyboardEvent.h"
#include "GamePadEvent.h"
#include "NGAudioEngine.h"
#include "TouchConverter.h"
#include "MainRenderer.h"
#include "ScreenEvent.h"
#include "FlagUtil.h"
#include "SaveData.h"
#include "SaveDataKeys.h"
#include "StringUtil.h"

static const int NUM_GC_REQ = 25;

/// World Map ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(MainScreen* ms)
{
	ms->m_stateMachine.setPreviousState(Title::getInstance());
    
    initRenderer(ms);
    
    m_clickedLevel = nullptr;
    m_userHasClickedOpeningCutscene = false;
    m_fGoldenCarrotCountFlickerTime = 1337;
    
    m_iNumTimesVisitedSinceLastAdBreak++;
    
    loadSaveData();
}

void WorldMap::execute(MainScreen* ms)
{
    if (ms->m_isRequestingRender)
    {
        ms->m_renderer->beginFrame();
        
        ms->m_renderer->renderWorldMapScreenBackground(m_panel);
        
        ms->m_renderer->renderWorldMapScreenUi(*this);
        ms->m_renderer->renderWorldMapScreenButtons(*this);
        
        if (ms->m_renderer->isLoadingData())
        {
            ms->m_renderer->renderLoading(ms->m_fDeltaTime * ms->m_iNumInternalUpdates);
        }
        
        ms->m_renderer->renderToScreen();
        
        ms->m_renderer->endFrame();
    }
    else
    {
        if (m_iNumTimesVisitedSinceLastAdBreak >= 1)
        {
            ms->m_iRequestedAction = REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD;
            m_iNumTimesVisitedSinceLastAdBreak = 0;
            
            return;
        }
        
        if (m_isReadyForTransition)
        {
            ms->m_stateMachine.changeState(WorldMapToLevel::getInstance());
			return;
        }
        
        if (m_needsRefresh)
        {
            loadSaveData();
            m_needsRefresh = false;
            
            return;
        }
        
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); ++i)
        {
            switch ((*i)->getType())
            {
                case KeyboardEventType_ARROW_KEY_RIGHT:
                {
                    if ((*i)->isUp())
                    {
                        navRight();
                        return;
                    }
                }
                    continue;
                case KeyboardEventType_ARROW_KEY_UP:
                {
                    if ((*i)->isUp())
                    {
                        navUp();
                        return;
                    }
                }
                    continue;
                case KeyboardEventType_ARROW_KEY_LEFT:
                {
                    if ((*i)->isUp())
                    {
                        navLeft();
                        return;
                    }
                }
                    continue;
                case KeyboardEventType_ARROW_KEY_DOWN:
                {
                    if ((*i)->isUp())
                    {
                        navDown();
                        return;
                    }
                }
                    continue;
                case KeyboardEventType_SPACE:
                case KeyboardEventType_ENTER:
                {
                    if ((*i)->isUp())
                    {
                        MAIN_ASSETS->setUsingGamePadTextureSet(false);
                        navSelect(ms);
                        return;
                    }
                }
                    continue;
                case KeyboardEventType_BACK:
                    if ((*i)->isUp())
                    {
                        ms->m_stateMachine.revertToPreviousState();
                        return;
                    }
                    continue;
                default:
                    break;
            }
        }
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); ++i)
        {
            switch ((*i)->getType())
            {
                case GamePadEventType_D_PAD_RIGHT:
                {
                    if ((*i)->isButtonPressed())
                    {
                        navRight();
                        return;
                    }
                }
                    continue;
                case GamePadEventType_D_PAD_UP:
                {
                    if ((*i)->isButtonPressed())
                    {
                        navUp();
                        return;
                    }
                }
                    continue;
                case GamePadEventType_D_PAD_LEFT:
                {
                    if ((*i)->isButtonPressed())
                    {
                        navLeft();
                        return;
                    }
                }
                    continue;
                case GamePadEventType_D_PAD_DOWN:
                {
                    if ((*i)->isButtonPressed())
                    {
                        navDown();
                        return;
                    }
                }
                    continue;
                case GamePadEventType_A_BUTTON:
                case GamePadEventType_START_BUTTON:
                {
                    if ((*i)->isButtonPressed())
                    {
                        MAIN_ASSETS->setUsingGamePadTextureSet(true);
                        navSelect(ms);
                        return;
                    }
                }
                    continue;
                case GamePadEventType_B_BUTTON:
                case GamePadEventType_BACK_BUTTON:
                    if ((*i)->isButtonPressed())
                    {
                        ms->m_stateMachine.revertToPreviousState();
                        return;
                    }
                    continue;
                default:
                    break;
            }
        }
        
        for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); ++i)
        {
            Vector2D& touchPoint = TOUCH_CONVERTER->touchToWorld(*(*i));
            
            switch ((*i)->getType())
            {
                case ScreenEventType_DOWN:
                    continue;
                case ScreenEventType_DRAGGED:
                    continue;
                case ScreenEventType_UP:
                    if (m_backButton->handleClick(touchPoint))
                    {
                        ms->m_stateMachine.revertToPreviousState();
                    }
                    else if (m_toggleMusic->handleClick(touchPoint))
                    {
                        NG_AUDIO_ENGINE->setMusicDisabled(!NG_AUDIO_ENGINE->isMusicDisabled());
                        m_toggleMusic->getColor().alpha = NG_AUDIO_ENGINE->isMusicDisabled() ? 0.35f : 1;
                        onButtonSelected();
                    }
                    else if (m_toggleSound->handleClick(touchPoint))
                    {
                        NG_AUDIO_ENGINE->setSoundDisabled(!NG_AUDIO_ENGINE->isSoundDisabled());
                        m_toggleSound->getColor().alpha = NG_AUDIO_ENGINE->isSoundDisabled() ? 0.35f : 1;
                        onButtonSelected();
                    }
                    else if (OverlapTester::isPointInNGRect(touchPoint, m_leaderBoardsButton->getMainBounds()))
                    {
                        // Temporary, replace with display Leaderboards
                        return;
                    }
                    else if (m_viewOpeningCutsceneButton->handleClick(touchPoint))
                    {
                        WorldMapToOpeningCutscene::getInstance()->setCutsceneButtonLocation(m_viewOpeningCutsceneButton->getPosition().getX(), m_viewOpeningCutsceneButton->getPosition().getY());
                        ms->m_stateMachine.changeState(WorldMapToOpeningCutscene::getInstance());
                        m_userHasClickedOpeningCutscene = true;
                        onButtonSelected();
                        return;
                    }
                    else
                    {
                        if (m_spendGoldenCarrotsBubble->isOpen())
                        {
                            int ret = m_spendGoldenCarrotsBubble->handleTouch(touchPoint);
                            if (ret == 1 || ret == 0)
                            {
                                if (ret == 1)
                                {
                                    // User has just spent X amount of golden carrots to unlock a boss level
                                    m_iNumCollectedGoldenCarrots -= NUM_GC_REQ;
                                    
                                    int worldToUnlock = m_spendGoldenCarrotsBubble->getWorld();
                                    int levelToUnlock = m_spendGoldenCarrotsBubble->getLevel();
                                    
                                    int worldIndex = worldToUnlock - 1;
                                    int levelIndex = levelToUnlock - 1;
                                    
                                    int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
                                    
                                    m_iUnlockedLevelStatsFlag = FlagUtil::setFlag(levelStats, FLAG_LEVEL_UNLOCKED);
                                    
                                    {
                                        std::string key = getKeyForLevelStats(worldToUnlock, levelToUnlock);
                                        std::string val = StringUtil::toString(m_iUnlockedLevelStatsFlag);
                                        NG_SAVE_DATA->getKeyValues()[key] = val;
                                    }
                                    
                                    {
                                        std::string key = getKeyForNumGoldenCarrots();
                                        std::string val = StringUtil::toString(m_iNumCollectedGoldenCarrots);
                                        NG_SAVE_DATA->getKeyValues()[key] = val;
                                    }
                                    
                                    NG_SAVE_DATA->save();
                                    
                                    m_needsRefresh = true;
                                }
                                
                                return;
                            }
                        }
                        
                        for (std::vector<AbilitySlot *>::iterator j = m_abilitySlots.begin(); j != m_abilitySlots.end(); ++j)
                        {
                            if ((*j)->isUnlocked()
                                && OverlapTester::isPointInNGRect(touchPoint, (*j)->getMainBounds()))
                            {
                                switch ((*j)->getType())
                                {
                                    case AbilitySlotType_Drill:
                                        // TODO, show informative panel about Jon's Drill Ability
                                        break;
                                    case AbilitySlotType_Dash:
                                        // TODO, show informative panel about Jon's Dash Ability
                                        break;
                                    default:
                                        break;
                                }
                                
                                return;
                            }
                        }
                        
                        for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); ++j)
                        {
                            if (OverlapTester::isPointInNGRect(touchPoint, (*j)->getMainBounds()))
                            {
                                if ((*j)->isSelected()
                                    && m_clickedLevel)
                                {
                                    MAIN_ASSETS->setUsingGamePadTextureSet(false);
                                    startLevel();
                                }
                                else if ((*j)->isPlayable()
                                         && !(*j)->isSelecting())
                                {
                                    selectLevel((*j));
                                    
                                    NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
                                }
                            }
                        }
                    }
                    
                    return;
            }
        }
        
        for (std::vector<AbilitySlot *>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); ++i)
        {
            (*i)->update(ms->m_fDeltaTime);
        }
        
        for (std::vector<LevelThumbnail *>::iterator i = m_levelThumbnails.begin(); i != m_levelThumbnails.end(); ++i)
        {
            (*i)->update(ms->m_fDeltaTime);
        }
        
        m_goldenCarrotsMarker->update(ms->m_fDeltaTime);
        m_scoreMarker->update(ms->m_fDeltaTime);
        m_spendGoldenCarrotsBubble->update(ms->m_fDeltaTime);
        
        m_fGoldenCarrotCountFlickerTime += ms->m_fDeltaTime / 2;
        
        updateButtons(ms->m_fDeltaTime);
    }
}

void WorldMap::exit(MainScreen* ms)
{
    m_isReadyForTransition = false;
}

void WorldMap::initRenderer(MainScreen* ms)
{
    ms->m_renderer->unload(RENDERER_TYPE_LEVEL_EDITOR);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    ms->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    ms->m_renderer->load(RENDERER_TYPE_WORLD_MAP);
}

void WorldMap::updateButtons(float deltaTime)
{
    m_toggleMusic->update(deltaTime);
    m_toggleSound->update(deltaTime);
    m_leaderBoardsButton->update(deltaTime);
    m_viewOpeningCutsceneButton->update(deltaTime);
}

void WorldMap::setFade(float fade)
{
    float alpha = 1 - fade;
    alpha = clamp(alpha, 1, 0);
    
    // Music Fade
    
    float musicVolume = alpha / 2;
    musicVolume = clamp(musicVolume, 1, 0);
    
    NG_AUDIO_ENGINE->setMusicVolume(musicVolume);
}

void WorldMap::loadSaveData()
{
    NG_SAVE_DATA->load();
    
    NGSTDUtil::cleanUpVectorOfPointers(m_worldLevelStats);
    
    loadWorld1SaveData();
    
    loadGlobalSaveData();
    
    m_isNextWorldButtonEnabled = false;
    
    std::string key = std::string("ng_unlock_all");
    std::string val = NG_SAVE_DATA->findValue(key);
    int isUnlockedAll = StringUtil::stringToInt(val);
    
    LevelThumbnail* levelToSelect = nullptr;
    int levelStatsForLevelToSelect = 0;
    int scoreForLevelToSelect = 0;
    for (std::vector<LevelThumbnail*>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); ++j)
    {
        int worldIndex = (*j)->getWorld() - 1;
        int levelIndex = (*j)->getLevel() - 1;
        
        int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
        int score = m_worldLevelStats.at(worldIndex)->m_scores.at(levelIndex);
        
        int previousLevelStats = false;
        if (worldIndex == 0 && levelIndex == 0)
        {
            previousLevelStats = 1;
        }
        else
        {
            int previousLevelIndex = levelIndex == 0 ? 20 : levelIndex - 1;
            int previousWorldIndex = levelIndex == 0 ? worldIndex - 1 : worldIndex;
            
            previousLevelStats = m_worldLevelStats.at(previousWorldIndex)->m_levelStats.at(previousLevelIndex);
        }
        
        bool isPlayable = FlagUtil::isFlagSet(previousLevelStats, FLAG_LEVEL_COMPLETE);
        
        if (isUnlockedAll)
        {
            isPlayable = true;
        }
        
		bool isCleared = FlagUtil::isFlagSet(levelStats, FLAG_LEVEL_COMPLETE)
        && FlagUtil::isFlagSet(levelStats, FLAG_FIRST_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_SECOND_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_THIRD_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_BONUS_GOLDEN_CARROT_COLLECTED);
        bool isClearing = isCleared && !(*j)->isCleared();
        
        if ((*j)->isBoss())
        {
            BossLevelThumbnail *bossLevelThumbnail = reinterpret_cast<BossLevelThumbnail *>((*j));
            
            bool isUnlocked = FlagUtil::isFlagSet(levelStats, FLAG_LEVEL_UNLOCKED);
            
            if (isUnlockedAll)
            {
                isUnlocked = true;
            }
            
            bool isUnlocking = isUnlocked && !bossLevelThumbnail->isUnlocked();
    
            bossLevelThumbnail->configLockStatus(isUnlocked, isUnlocking);
            
            if (isUnlocking)
            {
                m_fGoldenCarrotCountFlickerTime = 0;
            }
            
            if ((*j)->getWorld() == 1
                && (*j)->getLevel() == 21
                && FlagUtil::isFlagSet(levelStats, FLAG_LEVEL_COMPLETE))
            {
                m_isNextWorldButtonEnabled = true;
            }
        }
        
        (*j)->config(isPlayable, isClearing, isCleared);
        
        int nextLevelIndex = levelIndex == 20 ? 0 : levelIndex + 1;
        int nextWorldIndex = levelIndex == 20 ? worldIndex + 1 : worldIndex;
        
        int nextLevelStats;
        if (nextWorldIndex == 1)
        {
            nextLevelStats = 0;
        }
        else
        {
            nextLevelStats = m_worldLevelStats.at(nextWorldIndex)->m_levelStats.at(nextLevelIndex);
        }
        
        if (isPlayable && nextLevelStats == 0)
        {
            levelToSelect = (*j);
            levelStatsForLevelToSelect = levelStats;
            scoreForLevelToSelect = score;
        }
    }
    
    if (levelToSelect)
    {
        selectLevel(levelToSelect);
    }
}

WorldMapPanel* WorldMap::getWorldMapPanel()
{
    return m_panel;
}

std::vector<AbilitySlot*>& WorldMap::getAbilitySlots()
{
    return m_abilitySlots;
}

GoldenCarrotsMarker* WorldMap::getGoldenCarrotsMarker()
{
    return m_goldenCarrotsMarker;
}

ScoreMarker* WorldMap::getScoreMarker()
{
    return m_scoreMarker;
}

SpendGoldenCarrotsBubble* WorldMap::getSpendGoldenCarrotsBubble()
{
    return m_spendGoldenCarrotsBubble;
}

std::vector<LevelThumbnail*>& WorldMap::getLevelThumbnails()
{
    return m_levelThumbnails;
}

GameButton* WorldMap::getBackButton()
{
    return m_backButton;
}

GameButton* WorldMap::getToggleMusicButton()
{
    return m_toggleMusic;
}

GameButton* WorldMap::getToggleSoundButton()
{
    return m_toggleSound;
}

GameButton* WorldMap::getLeaderBoardsButton()
{
    return m_leaderBoardsButton;
}

GameButton* WorldMap::getViewOpeningCutsceneButton()
{
    return m_viewOpeningCutsceneButton;
}

float WorldMap::getGoldenCarrotCountFlickerTime()
{
    return m_fGoldenCarrotCountFlickerTime;
}

int WorldMap::getNumCollectedGoldenCarrots()
{
    return m_iNumCollectedGoldenCarrots;
}

int WorldMap::getViewedCutsceneFlag()
{
    return m_iViewedCutsceneFlag;
}

int WorldMap::getUnlockedLevelStatsFlag()
{
    return m_iUnlockedLevelStatsFlag;
}

LevelThumbnail* WorldMap::getSelectedLevelThumbnail()
{
    return m_clickedLevel;
}

#pragma mark private

void WorldMap::loadGlobalSaveData()
{
    m_iNumCollectedGoldenCarrots = 0;
    m_iViewedCutsceneFlag = 0;
    
    {
        std::string key = getKeyForNumGoldenCarrots();
        std::string val = NG_SAVE_DATA->findValue(key);
        int numCollectedGoldenCarrots = StringUtil::stringToInt(val);
        
        m_spendGoldenCarrotsBubble->setUserHasEnoughGoldenCats(numCollectedGoldenCarrots >= NUM_GC_REQ);
        
        if (m_iNumCollectedGoldenCarrots != numCollectedGoldenCarrots
            && numCollectedGoldenCarrots != 0)
        {
            m_fGoldenCarrotCountFlickerTime = 0;
        }
        
        m_iNumCollectedGoldenCarrots = numCollectedGoldenCarrots;
    }
    
    {
        std::string key = getKeyForJonUnlockedAbilitiesFlag();
        std::string val = NG_SAVE_DATA->findValue(key);
        int jonAbilityFlag = StringUtil::stringToInt(val);
        
        {
            bool isUnlocked = FlagUtil::isFlagSet(jonAbilityFlag, FLAG_ABILITY_RABBIT_DOWN);
            bool isUnlocking = isUnlocked && !FlagUtil::isFlagSet(m_iJonAbilityFlag, FLAG_ABILITY_RABBIT_DOWN);
            
            configAbilitySlot(AbilitySlotType_Drill, isUnlocked, isUnlocking);
        }
        
        {
            bool isUnlocked = FlagUtil::isFlagSet(jonAbilityFlag, FLAG_ABILITY_VAMPIRE_RIGHT);
            bool isUnlocking = isUnlocked && !FlagUtil::isFlagSet(m_iJonAbilityFlag, FLAG_ABILITY_VAMPIRE_RIGHT);
            
            configAbilitySlot(AbilitySlotType_Dash, isUnlocked, isUnlocking);
        }
        
        m_iJonAbilityFlag = jonAbilityFlag;
    }
    
    {
        std::string key = getKeyForViewedCutscenesFlag();
        std::string val = NG_SAVE_DATA->findValue(key);
        m_iViewedCutsceneFlag = StringUtil::stringToInt(val);
    }
    
    validateAbilityFlag();
}

void WorldMap::loadWorld1SaveData()
{
    WorldLevelCompletions* wlc = new WorldLevelCompletions();
    
    for (int i = 1; i <= 21; ++i)
    {
        {
            std::string key = getKeyForLevelStats(1, i);
            std::string val = NG_SAVE_DATA->findValue(key);
            int levelStats = StringUtil::stringToInt(val);
            
            wlc->m_levelStats.push_back(levelStats);
        }
        
        {
            std::string key = getKeyForLevelScore(1, i);
            std::string val = NG_SAVE_DATA->findValue(key);
            int score = StringUtil::stringToInt(val);
            
            wlc->m_scores.push_back(score);
        }
        
        {
            std::string key = getKeyForLevelOnlineScore(1, i);
            std::string val = NG_SAVE_DATA->findValue(key);
            int onlineScore = StringUtil::stringToInt(val);
            
            wlc->m_onlineScores.push_back(onlineScore);
        }
    }
    
    m_worldLevelStats.push_back(wlc);
}

void WorldMap::configAbilitySlot(AbilitySlotType abilitySlotType, bool isUnlocked, bool isUnlocking)
{
    for (std::vector<AbilitySlot *>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); ++i)
    {
        if ((*i)->getType() == abilitySlotType)
        {
            (*i)->config(isUnlocked, isUnlocking);
        }
    }
}

void WorldMap::selectLevel(LevelThumbnail* levelThumbnail)
{
    int worldToLoad = levelThumbnail->getWorld();
    int levelToLoad = levelThumbnail->getLevel();
    
    int worldIndex = worldToLoad - 1;
    int levelIndex = levelToLoad - 1;
    
    int levelStatsFlag = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
    int score = m_worldLevelStats.at(worldIndex)->m_scores.at(levelIndex);
    
    if (!levelThumbnail->isPlayable())
    {
        return;
    }
    
    m_viewOpeningCutsceneButton->deselect();
    m_toggleMusic->deselect();
    m_toggleSound->deselect();
    
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); ++j)
    {
        (*j)->deselect();
    }
    
    levelThumbnail->select();
    
    m_clickedLevel = levelThumbnail;
    
    BossLevelThumbnail *bossLevelThumbnail = nullptr;
    if (levelThumbnail->isBoss())
    {
        bossLevelThumbnail = reinterpret_cast<BossLevelThumbnail *>(levelThumbnail);
    }
    
    m_goldenCarrotsMarker->config(1337, 1337, 0);
    
    m_scoreMarker->config(1337, 1337, 0);
 
    if (bossLevelThumbnail && !bossLevelThumbnail->isUnlocked())
    {
        return;
    }
    
    if (FlagUtil::isFlagSet(levelStatsFlag, FLAG_LEVEL_COMPLETE))
    {
        int numGoldenCarrots = 0;
        if (FlagUtil::isFlagSet(levelStatsFlag, FLAG_FIRST_GOLDEN_CARROT_COLLECTED))
        {
            numGoldenCarrots++;
        }
        
        if (FlagUtil::isFlagSet(levelStatsFlag, FLAG_SECOND_GOLDEN_CARROT_COLLECTED))
        {
            numGoldenCarrots++;
        }
        
        if (FlagUtil::isFlagSet(levelStatsFlag, FLAG_THIRD_GOLDEN_CARROT_COLLECTED))
        {
            numGoldenCarrots++;
        }
        
        if (FlagUtil::isFlagSet(levelStatsFlag, FLAG_BONUS_GOLDEN_CARROT_COLLECTED))
        {
            numGoldenCarrots++;
        }
        
        float levelThumbnailHeight = CAM_HEIGHT * 0.20261437908497f;
        
        float levelThumbnailY = levelThumbnail->getPosition().getY();
        
        if (bossLevelThumbnail)
        {
            levelThumbnailY -= 0.006;
        }
        
        m_goldenCarrotsMarker->config(levelThumbnail->getPosition().getX(), levelThumbnailY + levelThumbnailHeight * 0.52f, numGoldenCarrots);
        
        m_scoreMarker->config(levelThumbnail->getPosition().getX(), levelThumbnailY - levelThumbnailHeight * 0.52f, score);
    }
}

void WorldMap::startLevel()
{
    int worldToLoad = m_clickedLevel->getWorld();
    int levelToLoad = m_clickedLevel->getLevel();
    
    int worldIndex = worldToLoad - 1;
    int levelIndex = levelToLoad - 1;
    
    int levelStatsFlag = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
    int score = m_worldLevelStats.at(worldIndex)->m_scores.at(levelIndex);
    int onlineScore = m_worldLevelStats.at(worldIndex)->m_onlineScores.at(levelIndex);
    
    WorldMapToLevel::getInstance()->setLevelLocation(m_clickedLevel->getPosition().getX(), m_clickedLevel->getPosition().getY());
    WorldMapToLevel::getInstance()->setWorldToLoad(worldToLoad);
    WorldMapToLevel::getInstance()->setLevelToLoad(levelToLoad);
    
    validateAbilityFlag();
    
    int abilityFlag = m_iJonAbilityFlag;
    
    std::string key = std::string("ng_unlock_all");
    std::string val = NG_SAVE_DATA->findValue(key);
    int isUnlockedAll = StringUtil::stringToInt(val);
    if (isUnlockedAll)
    {
        abilityFlag = FLAG_ABILITY_ALL;
    }
    
    WorldMapToLevel::getInstance()->setBestStats(score, onlineScore, levelStatsFlag, m_iNumCollectedGoldenCarrots, abilityFlag);
    
    m_isReadyForTransition = true;
    
    m_goldenCarrotsMarker->onConfirm();
    m_clickedLevel->onConfirm();
    m_scoreMarker->onConfirm();
}

void WorldMap::validateAbilityFlag()
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); ++j)
    {
        int world = (*j)->getWorld();
        int level = (*j)->getLevel();
        
        int worldIndex = world - 1;
        int levelIndex = level - 1;
        
        int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
        
        if (FlagUtil::isFlagSet(levelStats, FLAG_LEVEL_COMPLETE))
        {
            if (world == 1)
            {
                if (level == 3)
                {
                    m_iJonAbilityFlag = FlagUtil::setFlag(m_iJonAbilityFlag, FLAG_ABILITY_DOUBLE_JUMP);
                }
                else if (level == 5)
                {
                    m_iJonAbilityFlag = FlagUtil::setFlag(m_iJonAbilityFlag, FLAG_ABILITY_TRANSFORM);
                }
                else if (level == 10)
                {
                    m_iJonAbilityFlag = FlagUtil::setFlag(m_iJonAbilityFlag, FLAG_ABILITY_RABBIT_DOWN);
                }
                else if (level == 21)
                {
                    m_iJonAbilityFlag = FlagUtil::setFlag(m_iJonAbilityFlag, FLAG_ABILITY_VAMPIRE_RIGHT);
                }
            }
        }
    }
}

void WorldMap::navRight()
{
    if (m_viewOpeningCutsceneButton->isSelected())
    {
        m_viewOpeningCutsceneButton->deselect();
        
        LevelThumbnail* newLevel = getLevelThumbnail(1, 1);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
  
        return;
    }
    
    if (m_toggleMusic->isSelected())
    {
        onButtonSelected();
        m_toggleSound->select();
        
        return;
    }
    
    if (m_toggleSound->isSelected())
    {
        return;
    }
    
    if (!m_clickedLevel)
    {
        return;
    }
    
    int world = m_clickedLevel->getWorld();
    int level = m_clickedLevel->getLevel();
    
    if (level < 7
        || (level > 14 && level < 21))
    {
        level++;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
    else if (level > 7 && level < 14)
    {
        level--;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
}

void WorldMap::navUp()
{
    if (m_viewOpeningCutsceneButton->isSelected())
    {
        return;
    }
    
    if (m_toggleMusic->isSelected())
    {
        onButtonSelected();
        m_viewOpeningCutsceneButton->select();
        return;
    }
    
    if (m_toggleSound->isSelected())
    {
        onButtonSelected();
        m_viewOpeningCutsceneButton->select();
        return;
    }
    
    if (!m_clickedLevel)
    {
        return;
    }
    
    int world = m_clickedLevel->getWorld();
    int level = m_clickedLevel->getLevel();
    int currentLevel = m_clickedLevel->getLevel();
    
    if (level == 13 || level == 20)
    {
        level -= 12;
    }
    else if (level == 12 || level == 19)
    {
        level -= 10;
    }
    else if (level == 11 || level == 18)
    {
        level -= 8;
    }
    else if (level == 10 || level == 17)
    {
        level -= 6;
    }
    else if (level == 9 || level == 16)
    {
        level -= 4;
    }
    else if (level == 8 || level == 15)
    {
        level -= 2;
    }
    else if (level == 7 || level == 14)
    {
        level -= 1;
    }
    
    if (level != currentLevel)
    {
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
}

void WorldMap::navLeft()
{
    if (m_viewOpeningCutsceneButton->isSelected())
    {
        return;
    }
    
    if (m_toggleMusic->isSelected())
    {
        return;
    }
    
    if (m_toggleSound->isSelected())
    {
        onButtonSelected();
        m_toggleMusic->select();
        return;
    }
    
    if (!m_clickedLevel)
    {
        return;
    }
    
    int world = m_clickedLevel->getWorld();
    int level = m_clickedLevel->getLevel();
    
    if ((level > 1 && level < 7)
        || level > 14)
    {
        level--;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
    else if (level > 7 && level < 14)
    {
        level++;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
    else if (level == 1)
    {
        onButtonSelected();
        m_viewOpeningCutsceneButton->select();
    }
}

void WorldMap::navDown()
{
    if (m_viewOpeningCutsceneButton->isSelected())
    {
        onButtonSelected();
        m_toggleMusic->select();
        return;
    }
    
    if (m_toggleMusic->isSelected())
    {
        return;
    }
    
    if (m_toggleSound->isSelected())
    {
        return;
    }
    
    if (!m_clickedLevel)
    {
        return;
    }
    
    int world = m_clickedLevel->getWorld();
    int level = m_clickedLevel->getLevel();
    int currentLevel = m_clickedLevel->getLevel();
    
    if (level == 1 || level == 8)
    {
        level += 12;
    }
    else if (level == 2 || level == 9)
    {
        level += 10;
    }
    else if (level == 3 || level == 10)
    {
        level += 8;
    }
    else if (level == 4 || level == 11)
    {
        level += 6;
    }
    else if (level == 5 || level == 12)
    {
        level += 4;
    }
    else if (level == 6 || level == 13)
    {
        level += 2;
    }
    else if (level == 7 || level == 14)
    {
        level += 1;
    }
    
    if (level != currentLevel)
    {
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_SELECTED);
    }
}

void WorldMap::navSelect(MainScreen* ms)
{
    if (m_viewOpeningCutsceneButton->isSelected())
    {
        m_viewOpeningCutsceneButton->click();
        WorldMapToOpeningCutscene::getInstance()->setCutsceneButtonLocation(m_viewOpeningCutsceneButton->getPosition().getX(), m_viewOpeningCutsceneButton->getPosition().getY());
        ms->m_stateMachine.changeState(WorldMapToOpeningCutscene::getInstance());
        m_userHasClickedOpeningCutscene = true;
        m_clickedLevel = nullptr;
    }
    if (m_toggleMusic->isSelected())
    {
        m_toggleMusic->click();
        NG_AUDIO_ENGINE->setMusicDisabled(!NG_AUDIO_ENGINE->isMusicDisabled());
        m_toggleMusic->getColor().alpha = NG_AUDIO_ENGINE->isMusicDisabled() ? 0.35f : 1;
        
    }
    else if (m_toggleSound->isSelected())
    {
        m_toggleSound->click();
        NG_AUDIO_ENGINE->setSoundDisabled(!NG_AUDIO_ENGINE->isSoundDisabled());
        m_toggleSound->getColor().alpha = NG_AUDIO_ENGINE->isSoundDisabled() ? 0.35f : 1;
    }
    else if (m_clickedLevel
        && m_clickedLevel->isSelected())
    {
        startLevel();
    }
}

void WorldMap::onButtonSelected()
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); ++j)
    {
        (*j)->deselect();
    }
    
    m_goldenCarrotsMarker->config(1337, 1337, 0);
    
    m_scoreMarker->config(1337, 1337, 0);
    
    m_clickedLevel = nullptr;
    
    m_viewOpeningCutsceneButton->deselect();
    m_toggleSound->deselect();
    m_toggleMusic->deselect();
}

LevelThumbnail * WorldMap::getLevelThumbnail(int world, int level)
{
    for (std::vector<LevelThumbnail *>::iterator i = m_levelThumbnails.begin(); i != m_levelThumbnails.end(); ++i)
    {
        int w = (*i)->getWorld();
        int l = (*i)->getLevel();
        
        if (world == w && level == l)
        {
            return *i;
        }
    }
    
    return nullptr;
}

WorldMap::WorldMap() : MainScreenState(),
m_panel(new WorldMapPanel()),
m_goldenCarrotsMarker(new GoldenCarrotsMarker()),
m_scoreMarker(new ScoreMarker()),
m_spendGoldenCarrotsBubble(new SpendGoldenCarrotsBubble()),
m_backButton(GameButton::create(GameButtonType_BackToTitle)),
m_toggleMusic(GameButton::create(GameButtonType_ToggleMusic)),
m_toggleSound(GameButton::create(GameButtonType_ToggleSound)),
m_leaderBoardsButton(GameButton::create(GameButtonType_Leaderboards)),
m_viewOpeningCutsceneButton(GameButton::create(GameButtonType_ViewOpeningCutscene)),
m_fGoldenCarrotCountFlickerTime(1337),
m_iNumCollectedGoldenCarrots(0),
m_iJonAbilityFlag(0),
m_iUnlockedLevelStatsFlag(0),
m_iViewedCutsceneFlag(0),
m_iNumTimesVisitedSinceLastAdBreak(0),
m_iStickNavDirection(0),
m_isReadyForTransition(false),
m_clickedLevel(nullptr),
m_userHasClickedOpeningCutscene(false),
m_needsRefresh(false),
m_isNextWorldButtonEnabled(false)
{
    float pW = m_panel->getWidth();
    float pH = m_panel->getHeight();

    m_abilitySlots.push_back(new AbilitySlot(pW * 0.18933823529412f, pH * 0.76633986928105f, AbilitySlotType_Drill));
    m_abilitySlots.push_back(new AbilitySlot(pW * 0.85477941176471f, pH * 0.83823529411765f, AbilitySlotType_Dash));
    
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.19117647058824f, pH * 0.56862745098039f, 1, 1));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.30882352941176f, pH * 0.56862745098039f, 1, 2));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.42647058823529f, pH * 0.56862745098039f, 1, 3));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.54411764705882f, pH * 0.56862745098039f, 1, 4));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.66176470588235f, pH * 0.56862745098039f, 1, 5));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.77941176470588f, pH * 0.56862745098039f, 1, 6));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.89705882352941f, pH * 0.4640522875817f, 1, 7));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.77941176470588f, pH * 0.35947712418301f, 1, 8));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.66176470588235f, pH * 0.35947712418301f, 1, 9));
    m_levelThumbnails.push_back(new MidBossLevelThumbnail(pW * 0.54411764705882f, pH * 0.35347712418301f, 1, 10, *m_spendGoldenCarrotsBubble));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.42647058823529f, pH * 0.35947712418301f, 1, 11));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.30882352941176f, pH * 0.35947712418301f, 1, 12));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.19117647058824f, pH * 0.35947712418301f, 1, 13));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.07552941176471f, pH * 0.25490196078432f, 1, 14));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.19117647058824f, pH * 0.15032679738562f, 1, 15));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.30882352941176f, pH * 0.15032679738562f, 1, 16));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.42647058823529f, pH * 0.15032679738562f, 1, 17));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.54411764705882f, pH * 0.15032679738562f, 1, 18));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.66176470588235f, pH * 0.15032679738562f, 1, 19));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.77941176470588f, pH * 0.15032679738562f, 1, 20));
    m_levelThumbnails.push_back(new BossLevelThumbnail(pW * 0.89705882352941f, pH * 0.14432679738562f, 1, 21, *m_spendGoldenCarrotsBubble));
}

WorldMap::~WorldMap()
{
    delete m_panel;
    delete m_goldenCarrotsMarker;
    delete m_scoreMarker;
    delete m_spendGoldenCarrotsBubble;
    delete m_backButton;
    delete m_toggleMusic;
    delete m_toggleSound;
    delete m_leaderBoardsButton;
    delete m_viewOpeningCutsceneButton;
    
    NGSTDUtil::cleanUpVectorOfPointers(m_abilitySlots);
    NGSTDUtil::cleanUpVectorOfPointers(m_levelThumbnails);
    NGSTDUtil::cleanUpVectorOfPointers(m_worldLevelStats);
}

WorldLevelCompletions::WorldLevelCompletions()
{
    // Empty
}

LevelThumbnail::LevelThumbnail(float x, float y, float width, float height, float selectTime, float clearTime, int world, int level, LevelThumbnailType type) : PhysicalEntity(x, y, width, height),
m_color(1, 1, 1, 1),
m_type(type),
m_fSelectTime(selectTime),
m_fClearTime(clearTime),
m_iWorld(world),
m_iLevel(level),
m_isPlayable(false),
m_isSelecting(false),
m_isSelected(false),
m_isClearing(false),
m_isCleared(false),
m_needsToPlayClearSound(false)
{
    // Empty
}

void LevelThumbnail::update(float deltaTime)
{
    if (m_needsToPlayClearSound)
    {
        NG_AUDIO_ENGINE->playSound(m_type == LevelThumbnailType_Boss ? SOUND_BOSS_LEVEL_CLEAR : SOUND_LEVEL_CLEAR);
        
        m_needsToPlayClearSound = false;
    }
    
    if (m_isClearing)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > m_fClearTime)
        {
            m_isClearing = false;
            
            m_fStateTime = 0;
        }
    }
    else if (m_isSelecting)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > m_fSelectTime)
        {
            m_isSelected = true;
            m_isSelecting = false;
            
            m_fStateTime = 0;
        }
    }
    else if (m_isSelected)
    {
        m_fStateTime += deltaTime;
    }
}

Color& LevelThumbnail::getColor()
{
    return m_color;
}

int LevelThumbnail::getWorld()
{
    return m_iWorld;
}

int LevelThumbnail::getLevel()
{
    return m_iLevel;
}

void LevelThumbnail::config(bool isPlayable, bool isClearing, bool isCleared)
{
    m_isPlayable = isPlayable;
    m_isSelecting = false;
    m_isSelected = false;
    m_isClearing = isClearing;
    m_isCleared = isCleared;
    
    m_needsToPlayClearSound = m_isClearing;
    
    m_fStateTime = 0;
}

void LevelThumbnail::select()
{
    m_isSelecting = true;
    m_isSelected = false;
    
    m_fStateTime = 0;
}

void LevelThumbnail::deselect()
{
    m_isSelecting = false;
    m_isSelected = false;
    
    m_fStateTime = 0;
}

bool LevelThumbnail::isPlayable()
{
    return m_isPlayable;
}

bool LevelThumbnail::isSelecting()
{
    return m_isSelecting;
}

bool LevelThumbnail::isSelected()
{
    return m_isSelected;
}

void LevelThumbnail::onConfirm()
{
    NG_AUDIO_ENGINE->playSound(SOUND_LEVEL_CONFIRMED);
    
    m_fStateTime = 0;
}

bool LevelThumbnail::isClearing()
{
    return m_isClearing;
}

bool LevelThumbnail::isCleared()
{
    return m_isCleared;
}

NormalLevelThumbnail::NormalLevelThumbnail(float x, float y, int world, int level) : LevelThumbnail(x, y, CAM_WIDTH * 0.11397058823529f, CAM_HEIGHT * 0.20261437908497f, 0.6f, 1.10f, world, level, LevelThumbnailType_Normal)
{
    // Empty
}

bool NormalLevelThumbnail::isBoss()
{
    return false;
}

SpendGoldenCarrotsBubble::SpendGoldenCarrotsBubble() : PhysicalEntity(1337, 1337, CAM_WIDTH * 0.21507352941176f, CAM_HEIGHT * 0.30065359477124f),
m_color(1, 1, 1, 1),
m_iWorld(-1),
m_iLevel(-1),
m_userHasEnoughGoldenCats(false),
m_isOpen(false)
{
    // Empty
}

void SpendGoldenCarrotsBubble::update(float deltaTime)
{
    if (m_isOpen)
    {
        m_fStateTime += deltaTime;
    }
}

/**
 * Return 1 if user spent golden carrots to unlock a boss level; 0 otherwise
 * Return 2 if the user didn't even touch the bubble at all
 */
int SpendGoldenCarrotsBubble::handleTouch(Vector2D& touchPoint)
{
    if (OverlapTester::isPointInNGRect(touchPoint, getMainBounds()))
    {
        if (m_userHasEnoughGoldenCats && touchPoint.getY() > (getMainBounds().getBottom() + getMainBounds().getHeight() / 2))
        {
            return 1;
        }
        
        return 0;
    }
    
    return 2;
}

void SpendGoldenCarrotsBubble::setUserHasEnoughGoldenCats(bool userHasEnoughGoldenCats)
{
    m_userHasEnoughGoldenCats = userHasEnoughGoldenCats;
}

void SpendGoldenCarrotsBubble::config(float x, float y, int world, int level)
{
    m_position.set(x, y);
    
    m_iWorld = world;
    m_iLevel = level;
    
    updateBounds();
    
    m_fStateTime = 0;
    
    m_isOpen = true;
}

void SpendGoldenCarrotsBubble::close()
{
    m_position.set(1337, 1337);
    
    updateBounds();
    
    m_fStateTime = 0;
    
    m_isOpen = false;
}

Color& SpendGoldenCarrotsBubble::getColor()
{
    return m_color;
}

int SpendGoldenCarrotsBubble::getWorld()
{
    return m_iWorld;
}

int SpendGoldenCarrotsBubble::getLevel()
{
    return m_iLevel;
}

bool SpendGoldenCarrotsBubble::userHasEnoughGoldenCats()
{
    return m_userHasEnoughGoldenCats;
}

bool SpendGoldenCarrotsBubble::isOpen()
{
    return m_isOpen;
}

BossLevelThumbnail::BossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width, float height) : LevelThumbnail(x, y, width, height, 0.3f, 0.7f, world, level, LevelThumbnailType_Boss), m_spendGoldenCarrotsBubble(spendGoldenCarrotsBubble), m_isJawMoving(false), m_isUnlocking(false), m_isUnlocked(false), m_needsToPlayUnlockSound(false)
{
    // Empty
}

bool BossLevelThumbnail::isBoss()
{
    return true;
}

void BossLevelThumbnail::update(float deltaTime)
{
    if (m_needsToPlayUnlockSound)
    {
        NG_AUDIO_ENGINE->playSound(SOUND_BOSS_LEVEL_UNLOCK);
        
        m_needsToPlayUnlockSound = false;
    }
    
    if (m_isUnlocking)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 1.30f)
        {
            m_isUnlocking = false;
            
            m_fStateTime = 0;
            
            if (m_isSelecting)
            {
                m_isSelected = true;
                m_isSelecting = false;
                
                m_fStateTime = 0;
            }
        }
    }
    else if (m_isUnlocked)
    {
        LevelThumbnail::update(deltaTime);
    }
    else
    {
        m_fStateTime += deltaTime;
    }
}

void BossLevelThumbnail::config(bool isPlayable, bool isClearing, bool isCleared)
{
    LevelThumbnail::config(isPlayable, isClearing, isCleared);
    
    m_color.red = m_isPlayable ? 1 : 0;
    m_color.green = m_isPlayable ? 1 : 0;
    m_color.blue = m_isPlayable ? 1 : 0;
}

void BossLevelThumbnail::select()
{
    if (m_isUnlocked)
    {
        LevelThumbnail::select();
    }
    else
    {
        m_isJawMoving = true;
        
        m_fStateTime = 0;
        
        float width = CAM_WIDTH * 0.18198529411765f;
        float height = CAM_HEIGHT * 0.33333333333333f;
        
        m_spendGoldenCarrotsBubble.config(m_position.getX() - width * 0.28f, m_position.getY() + height * 0.44f, getWorld(), getLevel());
    }
}

void BossLevelThumbnail::deselect()
{
    if (m_isUnlocked)
    {
        LevelThumbnail::deselect();
    }
    else
    {
        m_isJawMoving = false;
        
        m_fStateTime = 0;
    }
    
    m_spendGoldenCarrotsBubble.close();
}

void BossLevelThumbnail::configLockStatus(bool isUnlocked, bool isUnlocking)
{
    m_isUnlocked = isUnlocked;
    m_isUnlocking = isUnlocking;
    
    m_needsToPlayUnlockSound = m_isUnlocking;
    
    m_fStateTime = 0;
}

bool BossLevelThumbnail::isJawMoving()
{
    return m_isJawMoving;
}

bool BossLevelThumbnail::isUnlocking()
{
    return m_isUnlocking;
}

bool BossLevelThumbnail::isUnlocked()
{
    return m_isUnlocked;
}

MidBossLevelThumbnail::MidBossLevelThumbnail(float x, float y, int world, int level, SpendGoldenCarrotsBubble& spendGoldenCarrotsBubble, float width, float height) : BossLevelThumbnail(x, y, world, level, spendGoldenCarrotsBubble, width, height)
{
    // Empty
}

AbilitySlot::AbilitySlot(float x, float y, AbilitySlotType type) : PhysicalEntity(x, y, CAM_WIDTH * 0.10845588235294f, CAM_HEIGHT * 0.18627450980392f),
m_color(1, 1, 1, 1),
m_type(type),
m_isUnlocked(false),
m_isUnlocking(false),
m_isRevealing(false),
m_needsToPlayUnlockSound(false)
{
    // Empty
}

void AbilitySlot::update(float deltaTime)
{
    if (m_needsToPlayUnlockSound)
    {
        NG_AUDIO_ENGINE->playSound(SOUND_ABILITY_UNLOCK);
        
        m_needsToPlayUnlockSound = false;
    }
    
    if (m_isUnlocking)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 1.00f)
        {
            m_isUnlocking = false;
            m_isRevealing = true;
            
            m_fStateTime = 0;
        }
    }
    else if (m_isRevealing)
    {
        m_fStateTime += deltaTime;
        
        if (m_fStateTime > 1.10f)
        {
            m_isRevealing = false;
        }
    }
}

Color& AbilitySlot::getColor()
{
    return m_color;
}

AbilitySlotType AbilitySlot::getType()
{
    return m_type;
}

bool AbilitySlot::isUnlocked()
{
    return m_isUnlocked;
}

bool AbilitySlot::isUnlocking()
{
    return m_isUnlocking;
}

void AbilitySlot::config(bool isUnlocked, bool isUnlocking)
{
    m_fStateTime = 0.0f;
    m_isUnlocked = isUnlocked;
    m_isUnlocking = isUnlocking;
    m_isRevealing = false;
    
    m_needsToPlayUnlockSound = m_isUnlocking;
    
    if (m_isUnlocked && !m_isUnlocking)
    {
        m_fStateTime = 1.10f;
    }
}

GoldenCarrotsMarker::GoldenCarrotsMarker() : PhysicalEntity(1337, 1337, CAM_WIDTH * 0.10845588235294f, CAM_HEIGHT * 0.12091503267974f), m_color(1, 1, 1, 1), m_iNumGoldenCarrots(0)
{
    // Empty
}

void GoldenCarrotsMarker::update(float deltaTime)
{
    m_fStateTime += deltaTime;
}

void GoldenCarrotsMarker::config(float x, float y, int numGoldenCarrots)
{
    m_position.set(x, y);
    
    m_iNumGoldenCarrots = numGoldenCarrots;
    
    m_fStateTime = 0;
}

Color& GoldenCarrotsMarker::getColor()
{
    return m_color;
}

int GoldenCarrotsMarker::getNumGoldenCarrots()
{
    return m_iNumGoldenCarrots;
}

void GoldenCarrotsMarker::onConfirm()
{
    update(1337);
}

ScoreMarker::ScoreMarker() :
m_color(1, 1, 1, 0),
m_fX(0),
m_fY(0),
m_iScore(0)
{
    // Empty
}

void ScoreMarker::update(float deltaTime)
{
    m_color.alpha += deltaTime;
    
    if (m_color.alpha > 1)
    {
        m_color.alpha = 1;
    }
}

void ScoreMarker::config(float x, float y, int score)
{
    m_fX = x;
    m_fY = y;
    m_iScore = score;
    
    m_color.alpha = 0;
}

Color& ScoreMarker::getColor()
{
    return m_color;
}

float ScoreMarker::getX()
{
    return m_fX;
}

float ScoreMarker::getY()
{
    return m_fY;
}

int ScoreMarker::getScore()
{
    return m_iScore;
}

void ScoreMarker::onConfirm()
{
    update(1337);
}

RTTI_IMPL(WorldMap, MainScreenState);
RTTI_IMPL_NOPARENT(WorldLevelCompletions);
RTTI_IMPL(LevelThumbnail, PhysicalEntity);
RTTI_IMPL(NormalLevelThumbnail, LevelThumbnail);
RTTI_IMPL(SpendGoldenCarrotsBubble, PhysicalEntity);
RTTI_IMPL(BossLevelThumbnail, LevelThumbnail);
RTTI_IMPL(MidBossLevelThumbnail, BossLevelThumbnail);
RTTI_IMPL(AbilitySlot, PhysicalEntity);
RTTI_IMPL(GoldenCarrotsMarker, PhysicalEntity);
RTTI_IMPL_NOPARENT(ScoreMarker);
