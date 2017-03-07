//
//  MainScreenWorldMap.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainScreenWorldMap.h"

#include "State.h"
#include "MainScreen.h"
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
#include "SoundManager.h"
#include "TouchConverter.h"
#include "MainRenderer.h"

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
    
    ms->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
    
    m_clickedLevel = nullptr;
    m_userHasClickedOpeningCutscene = false;
    m_fGoldenCarrotCountFlickerTime = 1337;
    
    m_iNumTimesVisitedSinceLastAdBreak++;
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
        if (m_iNumTimesVisitedSinceLastAdBreak >= 3)
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
            ms->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
            m_needsRefresh = false;
            
            return;
        }
        
        for (std::vector<KeyboardEvent *>::iterator i = KEYBOARD_INPUT_MANAGER->getEvents().begin(); i != KEYBOARD_INPUT_MANAGER->getEvents().end(); i++)
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
        
        for (std::vector<GamePadEvent *>::iterator i = GAME_PAD_INPUT_MANAGER->getEvents().begin(); i != GAME_PAD_INPUT_MANAGER->getEvents().end(); i++)
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
        
        for (std::vector<ScreenEvent *>::iterator i = SCREEN_INPUT_MANAGER->getEvents().begin(); i != SCREEN_INPUT_MANAGER->getEvents().end(); i++)
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
                        SOUND_MANAGER->setMusicEnabled(!SOUND_MANAGER->isMusicEnabled());
                        m_toggleMusic->getColor().alpha = SOUND_MANAGER->isMusicEnabled() ? 1 : 0.35f;
                        onButtonSelected();
                    }
                    else if (m_toggleSound->handleClick(touchPoint))
                    {
                        SOUND_MANAGER->setSoundEnabled(!SOUND_MANAGER->isSoundEnabled());
                        m_toggleSound->getColor().alpha = SOUND_MANAGER->isSoundEnabled() ? 1 : 0.35f;
                        onButtonSelected();
                    }
                    //else if (OverlapTester::isPointInNGRect(touchPoint, m_leaderBoardsButton->getMainBounds()))
                    //{
                    //    // Temporary, replace with display Leaderboards
                    //    return;
                    //}
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
                                    
                                    ms->m_iRequestedAction = REQUESTED_ACTION_UNLOCK_LEVEL * 1000;
                                    ms->m_iRequestedAction += worldToUnlock * 100;
                                    ms->m_iRequestedAction += levelToUnlock;
                                    
                                    m_needsRefresh = true;
                                }
                                
                                return;
                            }
                        }
                        
                        for (std::vector<AbilitySlot *>::iterator j = m_abilitySlots.begin(); j != m_abilitySlots.end(); j++)
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
                        
                        for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
                        {
                            if (OverlapTester::isPointInNGRect(touchPoint, (*j)->getMainBounds()))
                            {
                                if ((*j)->isSelected())
                                {
                                    MAIN_ASSETS->setUsingGamePadTextureSet(false);
                                    startLevel();
                                }
                                else if ((*j)->isPlayable()
                                         && !(*j)->isSelecting())
                                {
                                    selectLevel((*j));
                                    
                                    SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
                                }
                            }
                        }
                    }
                    
                    return;
            }
        }
        
        for (std::vector<AbilitySlot *>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); i++)
        {
            (*i)->update(ms->m_fDeltaTime);
        }
        
        for (std::vector<LevelThumbnail *>::iterator i = m_levelThumbnails.begin(); i != m_levelThumbnails.end(); i++)
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
    
    short musicId = MUSIC_SET_VOLUME * 1000 + (short) (musicVolume * 100);
    SOUND_MANAGER->addMusicIdToPlayQueue(musicId);
}

void WorldMap::loadUserSaveData(const char* json)
{
    VectorUtil::cleanUpVectorOfPointers(m_worldLevelStats);
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadUserSaveDataForWorld(d, "world_1");
    
    loadGlobalUserSaveData(d);
    
    m_isNextWorldButtonEnabled = false;
    
    LevelThumbnail* levelToSelect = nullptr;
    int levelStatsForLevelToSelect = 0;
    int scoreForLevelToSelect = 0;
    for (std::vector<LevelThumbnail*>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
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
#if NG_LEVEL_EDITOR
        isPlayable = true;
#endif
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
#if NG_LEVEL_EDITOR
            isUnlocked = true;
#endif
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

#pragma mark private

void WorldMap::loadGlobalUserSaveData(rapidjson::Document& d)
{
    m_iNumCollectedGoldenCarrots = 0;
    m_iViewedCutsceneFlag = 0;
    
    const char * num_golden_carrots_key = "num_golden_carrots";
    const char * jon_unlocked_abilities_flag_key = "jon_unlocked_abilities_flag";
    const char * viewed_cutscenes_flag_key = "viewed_cutscenes_flag";
    
    using namespace rapidjson;
    
    if (d.HasMember(num_golden_carrots_key))
    {
        Value& v = d[num_golden_carrots_key];
        assert(v.IsInt());
        
        int numCollectedGoldenCarrots = v.GetInt();
        
        m_spendGoldenCarrotsBubble->setUserHasEnoughGoldenCats(numCollectedGoldenCarrots >= NUM_GC_REQ);
        
        if (m_iNumCollectedGoldenCarrots != numCollectedGoldenCarrots
            && numCollectedGoldenCarrots != 0)
        {
            m_fGoldenCarrotCountFlickerTime = 0;
        }
        
        m_iNumCollectedGoldenCarrots = numCollectedGoldenCarrots;
    }
    
    if (d.HasMember(jon_unlocked_abilities_flag_key))
    {
        Value& v = d[jon_unlocked_abilities_flag_key];
        assert(v.IsInt());
        
        int jonAbilityFlag = v.GetInt();
        
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
    
    if (d.HasMember(viewed_cutscenes_flag_key))
    {
        Value& v = d[viewed_cutscenes_flag_key];
        assert(v.IsInt());
        
        m_iViewedCutsceneFlag = v.GetInt();
    }
    
    validateAbilityFlag();
}

void WorldMap::loadUserSaveDataForWorld(rapidjson::Document& d, const char * key)
{
    const char * stats_flag_key = "stats_flag";
    const char * score_key = "score";
    const char * score_online_key = "score_online";
    
    using namespace rapidjson;
    
    if (d.HasMember(key))
    {
        Value& levelArrayVal = d[key];
        assert(levelArrayVal.IsArray());
        
        WorldLevelCompletions* wlc = new WorldLevelCompletions();
        
        for (SizeType i = 0; i < levelArrayVal.Size(); i++)
        {
            Value& levelVal = levelArrayVal[i];
            
            if (levelVal.HasMember(stats_flag_key))
            {
                Value& levelStatsVal = levelVal[stats_flag_key];
                assert(levelStatsVal.IsInt());
                
                int levelStats = levelStatsVal.GetInt();
                
                wlc->m_levelStats.push_back(levelStats);
            }
            
            if (levelVal.HasMember(score_key))
            {
                Value& scoreVal = levelVal[score_key];
                assert(scoreVal.IsInt());
                
                int score = scoreVal.GetInt();
                
                wlc->m_scores.push_back(score);
            }
            
            if (levelVal.HasMember(score_online_key))
            {
                Value& onlineScoreVal = levelVal[score_online_key];
                assert(onlineScoreVal.IsInt());
                
                int onlineScore = onlineScoreVal.GetInt();
                
                wlc->m_onlineScores.push_back(onlineScore);
            }
        }
        
        m_worldLevelStats.push_back(wlc);
    }
}

void WorldMap::configAbilitySlot(AbilitySlotType abilitySlotType, bool isUnlocked, bool isUnlocking)
{
    for (std::vector<AbilitySlot *>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); i++)
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
    
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
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
#if NG_LEVEL_EDITOR
    abilityFlag = FLAG_ABILITY_ALL;
#endif
    
    WorldMapToLevel::getInstance()->setBestStats(score, onlineScore, levelStatsFlag, m_iNumCollectedGoldenCarrots, abilityFlag);
    
    m_isReadyForTransition = true;
    
    m_goldenCarrotsMarker->onConfirm();
    m_clickedLevel->onConfirm();
    m_scoreMarker->onConfirm();
}

void WorldMap::validateAbilityFlag()
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
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
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
  
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
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
    }
    else if (level > 7 && level < 14)
    {
        level--;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
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
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
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
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
    }
    else if (level > 7 && level < 14)
    {
        level++;
        LevelThumbnail* newLevel = getLevelThumbnail(world, level);
        selectLevel(newLevel);
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
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
        
        SOUND_MANAGER->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
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
        SOUND_MANAGER->setMusicEnabled(!SOUND_MANAGER->isMusicEnabled());
        m_toggleMusic->getColor().alpha = SOUND_MANAGER->isMusicEnabled() ? 1 : 0.35f;
        
    }
    else if (m_toggleSound->isSelected())
    {
        m_toggleSound->click();
        SOUND_MANAGER->setSoundEnabled(!SOUND_MANAGER->isSoundEnabled());
        m_toggleSound->getColor().alpha = SOUND_MANAGER->isSoundEnabled() ? 1 : 0.35f;
    }
    else if (m_clickedLevel
        && m_clickedLevel->isSelected())
    {
        startLevel();
    }
}

void WorldMap::onButtonSelected()
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
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
    for (std::vector<LevelThumbnail *>::iterator i = m_levelThumbnails.begin(); i != m_levelThumbnails.end(); i++)
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
    
    VectorUtil::cleanUpVectorOfPointers(m_abilitySlots);
    VectorUtil::cleanUpVectorOfPointers(m_levelThumbnails);
    VectorUtil::cleanUpVectorOfPointers(m_worldLevelStats);
}

RTTI_IMPL_NOPARENT(WorldLevelCompletions);
RTTI_IMPL(LevelThumbnail, PhysicalEntity);
RTTI_IMPL(NormalLevelThumbnail, LevelThumbnail);
RTTI_IMPL(SpendGoldenCarrotsBubble, PhysicalEntity);
RTTI_IMPL(BossLevelThumbnail, LevelThumbnail);
RTTI_IMPL(MidBossLevelThumbnail, BossLevelThumbnail);
RTTI_IMPL(AbilitySlot, PhysicalEntity);
RTTI_IMPL(GoldenCarrotsMarker, PhysicalEntity);
RTTI_IMPL_NOPARENT(ScoreMarker);
RTTI_IMPL(WorldMap, MainScreenState);
