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

/// World Map ///

WorldMap * WorldMap::getInstance()
{
    static WorldMap *instance = new WorldMap();
    
    return instance;
}

void WorldMap::enter(GameScreen* gs)
{
	gs->m_stateMachine->setPreviousState(Title::getInstance());
    
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
    
    gs->m_iRequestedAction = REQUESTED_ACTION_GET_SAVE_DATA;
}

void WorldMap::execute(GameScreen* gs)
{
    if (gs->m_isRequestingRender)
    {
        gs->m_renderer->beginFrame(gs->m_fDeltaTime);
        
        gs->m_renderer->renderWorldMapScreenBackground(m_panel.get());
        
        if (gs->m_renderer->isLoadingAdditionalTextures())
        {
            gs->m_renderer->renderLoading();
        }
        else
        {
            gs->m_renderer->renderWorldMapScreenUi(m_levelThumbnails, m_goldenCarrotsMarker.get(), m_backButton.get(), m_leaderBoardsButton.get(), m_iNumCollectedGoldenCarrots);
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
        
        for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
        {
            (*j)->update(gs->m_fDeltaTime);
        }
        
        m_goldenCarrotsMarker->update(gs->m_fDeltaTime);

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
                    if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_backButton->getMainBounds()))
                    {
                        gs->m_stateMachine->revertToPreviousState();
                    }
                    else if (OverlapTester::isPointInRectangle(*gs->m_touchPoint, m_leaderBoardsButton->getMainBounds()))
                    {
                        // Temporary, replace with display Leaderboards
                        gs->m_iRequestedAction = REQUESTED_ACTION_SHOW_MESSAGE * 1000 + MESSAGE_FEATURE_COMING_SOON_KEY;
                        return;
                    }
                    else
                    {
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
                                int onlineScore = m_worldLevelStats.at(worldIndex)->m_onlineScores.at(levelIndex);
                                
                                WorldMapToLevel::getInstance()->setLevelLocation((*j)->getPosition().getX(), (*j)->getPosition().getY());
                                WorldMapToLevel::getInstance()->setWorldToLoad(worldToLoad);
                                WorldMapToLevel::getInstance()->setLevelToLoad(levelToLoad);
                                
                                WorldMapToLevel::getInstance()->setBestStats(score, onlineScore, levelStats, m_iNumCollectedGoldenCarrots, m_iJonAbilityFlag);
                                
                                if ((*j)->isSelected())
                                {
                                    m_isReadyForTransition = true;
                                }
                                else if ((*j)->isPlayable()
                                         && !(*j)->isSelecting())
                                {
                                    selectLevel((*j), levelStats);
                                }
                            }
                        }
                    }
                    
                    return;
            }
        }
    }
}

void WorldMap::exit(GameScreen* gs)
{
    m_isReadyForTransition = false;
}

void WorldMap::loadUserSaveData(const char* json)
{
    m_worldLevelStats.clear();
    
    rapidjson::Document d;
    d.Parse<0>(json);
    
    loadGlobalUserSaveData(d);
    
    loadUserSaveDataForWorld(d, "world_1");
    
    LevelThumbnail* levelToSelect = nullptr;
    int levelStatsForLevelToSelect = 0;
    for (std::vector<LevelThumbnail*>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
    {
        int worldIndex = (*j)->getWorld() - 1;
        int levelIndex = (*j)->getLevel() - 1;
        
        int levelStats = m_worldLevelStats.at(worldIndex)->m_levelStats.at(levelIndex);
        
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
        
        bool isPlayable = previousLevelStats > 0;
        bool isCleared = FlagUtil::isFlagSet(levelStats, FLAG_LEVEL_COMPLETE)
        && FlagUtil::isFlagSet(levelStats, FLAG_FIRST_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_SECOND_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_THIRD_GOLDEN_CARROT_COLLECTED)
        && FlagUtil::isFlagSet(levelStats, FLAG_BONUS_GOLDEN_CARROT_COLLECTED);
        bool isClearing = isCleared && !(*j)->isCleared();
        
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
        }
    }
    
    if (levelToSelect)
    {
        selectLevel(levelToSelect, levelStatsForLevelToSelect);
    }
}

std::vector<LevelThumbnail*>& WorldMap::getLevelThumbnails()
{
    return m_levelThumbnails;
}

WorldMapPanel* WorldMap::getWorldMapPanel()
{
    return m_panel.get();
}

GameButton* WorldMap::getBackButton()
{
    return m_backButton.get();
}

GameButton* WorldMap::getLeaderBoardsButton()
{
    return m_leaderBoardsButton.get();
}

int WorldMap::getViewedCutsceneFlag()
{
    return m_iViewedCutsceneFlag;
}

#pragma mark private

void WorldMap::loadGlobalUserSaveData(rapidjson::Document& d)
{
    m_iNumCollectedGoldenCarrots = 0;
    m_iJonAbilityFlag = 0;
    m_iViewedCutsceneFlag = 0;
    
    const char * num_golden_carrots_key = "num_golden_carrots";
    const char * jon_unlocked_abilities_flag_key = "jon_unlocked_abilities_flag";
    const char * viewed_cutscenes_flag_key = "viewed_cutscenes_flag";
    
    using namespace rapidjson;
    
    if (d.HasMember(num_golden_carrots_key))
    {
        Value& v = d[num_golden_carrots_key];
        assert(v.IsInt());
        
        m_iNumCollectedGoldenCarrots = v.GetInt();
    }
    
    if (d.HasMember(jon_unlocked_abilities_flag_key))
    {
        Value& v = d[jon_unlocked_abilities_flag_key];
        assert(v.IsInt());
        
        m_iJonAbilityFlag = v.GetInt();
    }
    
    if (d.HasMember(viewed_cutscenes_flag_key))
    {
        Value& v = d[viewed_cutscenes_flag_key];
        assert(v.IsInt());
        
        m_iViewedCutsceneFlag = v.GetInt();
    }
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

void WorldMap::selectLevel(LevelThumbnail* levelThumbnail, int levelStatsFlag)
{
    for (std::vector<LevelThumbnail *>::iterator j = m_levelThumbnails.begin(); j != m_levelThumbnails.end(); j++)
    {
        (*j)->deselect();
    }
    
    levelThumbnail->select();
    
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
        
        m_goldenCarrotsMarker->config(levelThumbnail->getPosition().getX(), levelThumbnail->getPosition().getY() + levelThumbnail->getHeight() * 0.56f, numGoldenCarrots);
        
        // TODO, display score
    }
}

WorldMap::WorldMap() :
m_iNumCollectedGoldenCarrots(0),
m_iJonAbilityFlag(0),
m_iViewedCutsceneFlag(0),
m_isReadyForTransition(false)
{
    m_panel = std::unique_ptr<WorldMapPanel>(new WorldMapPanel());
    m_goldenCarrotsMarker = std::unique_ptr<GoldenCarrotsMarker>(new GoldenCarrotsMarker());
    m_backButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_BackToTitle));
    m_leaderBoardsButton = std::unique_ptr<GameButton>(GameButton::create(GameButtonType_Leaderboards));
    
    float pW = m_panel->getWidth();
    float pH = m_panel->getHeight();
    
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.19117647058824f, pH * 0.56862745098039f, 1, 1));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.30882352941176f, pH * 0.56862745098039f, 1, 2));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.42647058823529f, pH * 0.56862745098039f, 1, 3));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.54411764705882f, pH * 0.56862745098039f, 1, 4));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.66176470588235f, pH * 0.56862745098039f, 1, 5));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.77941176470588f, pH * 0.56862745098039f, 1, 6));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.89705882352941f, pH * 0.4640522875817f, 1, 7));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.77941176470588f, pH * 0.35947712418301f, 1, 8));
    m_levelThumbnails.push_back(new NormalLevelThumbnail(pW * 0.66176470588235f, pH * 0.35947712418301f, 1, 9));
    m_levelThumbnails.push_back(new BossLevelThumbnail(pW * 0.54411764705882f, pH * 0.35947712418301f, 1, 10));
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
    m_levelThumbnails.push_back(new BossLevelThumbnail(pW * 0.89705882352941f, pH * 0.15032679738562f, 1, 21));
}
