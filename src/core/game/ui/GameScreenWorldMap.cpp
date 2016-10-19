//
//  GameScreenWorldMap.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/10/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "GameScreenWorldMap.h"

#include "State.h"
#include "GameScreen.h"
#include "EntityUtils.h"
#include "Vector2D.h"
#include "Game.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenTransitions.h"
#include "GameScreenTitle.h"
#include "MathUtil.h"

static const int NUM_GC_REQ = 25;

/// World Map ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(GameScreen* gs)
{
	gs->m_stateMachine->setPreviousState(Title::getInstance());
    
    initRenderer(gs);
    
    gs->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
    
    m_clickedLevel = nullptr;
    m_userHasClickedOpeningCutscene = false;
    m_fGoldenCarrotCountFlickerTime = 1337;
}

void WorldMap::initRenderer(GameScreen* gs)
{
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_1_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_2_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_3_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_4_END_BOSS);
    
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5_MID_BOSS);
    gs->m_renderer->unload(RENDERER_TYPE_WORLD_5_END_BOSS);
    
    gs->m_renderer->init(RENDERER_TYPE_WORLD_MAP);
}

void WorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorldMapScreenBackground(m_panel.get());
        
        gs->m_renderer->renderWorldMapScreenUi(*this);
        gs->m_renderer->renderWorldMapScreenButtons(*this);
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        
        gs->m_renderer->renderToScreen();
        
        gs->m_renderer->endFrame();
    }
    else
    {
        if (m_isReadyForTransition)
        {
            gs->m_stateMachine->changeState(WorldMapToLevel::getInstance());
			return;
        }
        
        if (m_needsRefresh)
        {
            gs->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
            m_needsRefresh = false;
            
            return;
        }
        
        for (std::vector<TouchEvent *>::iterator i = gs->m_touchEvents.begin(); i != gs->m_touchEvents.end(); i++)
        {
            gs->touchToWorld(*(*i));
            
            switch ((*i)->getTouchType())
            {
                case DOWN:
                    continue;
                case DRAGGED:
                    continue;
                case UP:
                    if (m_backButton->handleClick(*gs->m_touchPoint))
                    {
                        gs->m_stateMachine->revertToPreviousState();
                    }
                    else if (m_toggleMusic->handleClick(*gs->m_touchPoint))
                    {
                        Assets::getInstance()->setMusicEnabled(!Assets::getInstance()->isMusicEnabled());
                        m_toggleMusic->getColor().alpha = Assets::getInstance()->isMusicEnabled() ? 1 : 0.35f;
                    }
                    else if (m_toggleSound->handleClick(*gs->m_touchPoint))
                    {
                        Assets::getInstance()->setSoundEnabled(!Assets::getInstance()->isSoundEnabled());
                        m_toggleSound->getColor().alpha = Assets::getInstance()->isSoundEnabled() ? 1 : 0.35f;
                    }
                    //else if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_leaderBoardsButton->getMainBounds()))
                    //{
                    //    // Temporary, replace with display Leaderboards
                    //    gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_FEATURE_COMING_SOON_KEY;
                    //    return;
                    //}
                    else if (m_viewOpeningCutsceneButton->handleClick(*gs->m_touchPoint))
                    {
                        WorldMapToOpeningCutscene::getInstance()->setCutsceneButtonLocation(m_viewOpeningCutsceneButton->getPosition().getX(), m_viewOpeningCutsceneButton->getPosition().getY());
                        gs->m_stateMachine->changeState(WorldMapToOpeningCutscene::getInstance());
                        m_userHasClickedOpeningCutscene = true;
                        m_clickedLevel = nullptr;
                        return;
                    }
                    else
                    {
                        if (m_spendGoldenCarrotsBubble->isOpen())
                        {
                            int ret = m_spendGoldenCarrotsBubble->handleTouch(*gs->m_touchPoint);
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
                                    
                                    gs->m_iRequestedAction = REQUESTED_ACTION_UNLOCK_LEVEL * 1000;
                                    gs->m_iRequestedAction += worldToUnlock * 100;
                                    gs->m_iRequestedAction += levelToUnlock;
                                    
                                    m_needsRefresh = true;
                                }
                                
                                return;
                            }
                        }
                        
                        for (std::vector<AbilitySlot *>::iterator j = m_abilitySlots.begin(); j != m_abilitySlots.end(); j++)
                        {
                            if ((*j)->isUnlocked()
                                && OverlapTester::isPointInRectangle(*gs->m_touchPoint, (*j)->getMainBounds()))
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
                            if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, (*j)->getMainBounds()))
                            {
                                int worldToLoad = (*j)->getWorld();
                                int levelToLoad = (*j)->getLevel(); 
                                
                                int worldIndex = worldToLoad - 1;
                                int levelIndex = levelToLoad - 1;
                                
                                int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
                                int score = m_worldLevelStats.at(worldIndex)->m_scores.at(levelIndex);
                                
                                if ((*j)->isSelected())
                                {
                                    int onlineScore = m_worldLevelStats.at(worldIndex)->m_onlineScores.at(levelIndex);
                                    
                                    WorldMapToLevel::getInstance()->setLevelLocation((*j)->getPosition().getX(), (*j)->getPosition().getY());
                                    WorldMapToLevel::getInstance()->setWorldToLoad(worldToLoad);
                                    WorldMapToLevel::getInstance()->setLevelToLoad(levelToLoad);
                                    
                                    validateAbilityFlag();
                                    
                                    int abilityFlag = m_iJonAbilityFlag;
#if NG_LEVEL_EDITOR
                                    abilityFlag = FLAG_ABILITY_ALL;
#endif
                                    
                                    WorldMapToLevel::getInstance()->setBestStats(score, onlineScore, levelStats, m_iNumCollectedGoldenCarrots, abilityFlag);
              
                                    m_isReadyForTransition = true;
                                    
                                    m_goldenCarrotsMarker->onConfirm();
                                    (*j)->onConfirm();
                                    m_scoreMarker->onConfirm();
                                }
                                else if ((*j)->isPlayable()
                                         && !(*j)->isSelecting())
                                {
                                    selectLevel((*j), levelStats, score);
                                    
                                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_LEVEL_SELECTED);
                                }
                            }
                        }
                    }
                    
                    return;
            }
        }
        
        for (std::vector<AbilitySlot *>::iterator i = m_abilitySlots.begin(); i != m_abilitySlots.end(); i++)
        {
            (*i)->update(gs->m_fDeltaTime);
        }
        
        for (std::vector<LevelThumbnail *>::iterator i = m_levelThumbnails.begin(); i != m_levelThumbnails.end(); i++)
        {
            (*i)->update(gs->m_fDeltaTime);
        }
        
        m_goldenCarrotsMarker->update(gs->m_fDeltaTime);
        m_scoreMarker->update(gs->m_fDeltaTime);
        m_spendGoldenCarrotsBubble->update(gs->m_fDeltaTime);
        
        m_fGoldenCarrotCountFlickerTime += gs->m_fDeltaTime / 2;
        
        updateButtons(gs->m_fDeltaTime);
    }
}

void WorldMap::exit(GameScreen* gs)
{
    m_isReadyForTransition = false;
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
    
    short musicId = MUSIC_SET_VOLUME * 1000 + (short) (musicVolume * 100);
    Assets::getInstance()->setMusicId(musicId);
}

void WorldMap::loadUserSaveData(const char* json)
{
    m_worldLevelStats.clear();
    
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
        
        BossLevelThumbnail *bossLevelThumbnail = dynamic_cast<BossLevelThumbnail *>((*j));
        
        if (bossLevelThumbnail)
        {
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
        selectLevel(levelToSelect, levelStatsForLevelToSelect, scoreForLevelToSelect);
    }
}

WorldMapPanel* WorldMap::getWorldMapPanel()
{
    return m_panel.get();
}

std::vector<AbilitySlot*>& WorldMap::getAbilitySlots()
{
    return m_abilitySlots;
}

GoldenCarrotsMarker* WorldMap::getGoldenCarrotsMarker()
{
    return m_goldenCarrotsMarker.get();
}

ScoreMarker* WorldMap::getScoreMarker()
{
    return m_scoreMarker.get();
}

SpendGoldenCarrotsBubble* WorldMap::getSpendGoldenCarrotsBubble()
{
    return m_spendGoldenCarrotsBubble.get();
}

std::vector<LevelThumbnail*>& WorldMap::getLevelThumbnails()
{
    return m_levelThumbnails;
}

GameButton* WorldMap::getBackButton()
{
    return m_backButton.get();
}

GameButton* WorldMap::getToggleMusicButton()
{
    return m_toggleMusic.get();
}

GameButton* WorldMap::getToggleSoundButton()
{
    return m_toggleSound.get();
}

GameButton* WorldMap::getLeaderBoardsButton()
{
    return m_leaderBoardsButton.get();
}

GameButton* WorldMap::getViewOpeningCutsceneButton()
{
    return m_viewOpeningCutsceneButton.get();
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
        
        m_worldLevelStats.push_back(std::unique_ptr<WorldLevelCompletions>(wlc));
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

void WorldMap::selectLevel(LevelThumbnail* levelThumbnail, int levelStatsFlag, int score)
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
    {
        (*j)->deselect();
    }
    
    levelThumbnail->select();
    
    m_clickedLevel = levelThumbnail;
    
    BossLevelThumbnail *bossLevelThumbnail = dynamic_cast<BossLevelThumbnail *>(levelThumbnail);
    
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

WorldMap::WorldMap() :
m_fGoldenCarrotCountFlickerTime(1337),
m_iNumCollectedGoldenCarrots(0),
m_iJonAbilityFlag(0),
m_iUnlockedLevelStatsFlag(0),
m_iViewedCutsceneFlag(0),
m_isReadyForTransition(false),
m_clickedLevel(nullptr),
m_userHasClickedOpeningCutscene(false),
m_needsRefresh(false),
m_isNextWorldButtonEnabled(false)
{
    m_panel = std::unique_ptr<WorldMapPanel>(new WorldMapPanel());
    m_goldenCarrotsMarker = std::unique_ptr<GoldenCarrotsMarker>(new GoldenCarrotsMarker());
    m_scoreMarker = std::unique_ptr<ScoreMarker>(new ScoreMarker());
    m_spendGoldenCarrotsBubble = std::unique_ptr<SpendGoldenCarrotsBubble>(new SpendGoldenCarrotsBubble());
    m_backButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_BackToTitle));
    m_toggleMusic = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_ToggleMusic));
    m_toggleSound = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_ToggleSound));
    m_leaderBoardsButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_Leaderboards));
    m_viewOpeningCutsceneButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_ViewOpeningCutscene));
    
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
