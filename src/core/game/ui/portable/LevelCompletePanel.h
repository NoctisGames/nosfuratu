//
//  LevelCompletePanel.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/28/17.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__LevelCompletePanel__
#define __nosfuratu__LevelCompletePanel__

#include "PhysicalEntity.h"

#include "GameConstants.h"

#include "RTTI.h"

#include <vector>

class Game;
class GameButton;

class ScoreIcon;
class FinalScoreSparkle;

#define LEVEL_COMPLETE_PANEL_RC_UNHANDLED -1
#define LEVEL_COMPLETE_PANEL_RC_HANDLED 0
#define LEVEL_COMPLETE_PANEL_RC_REPLAY 1
#define LEVEL_COMPLETE_PANEL_RC_CONTINUE 2
#define LEVEL_COMPLETE_PANEL_RC_SUBMIT_SCORE 3

class LevelCompletePanel : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    LevelCompletePanel(float x = -CAM_WIDTH / 2, float y = CAM_HEIGHT / 2, float width = CAM_WIDTH * 0.64f, float height = CAM_HEIGHT * 0.64f);
    
    virtual ~LevelCompletePanel();
    
    virtual void update(float deltaTime);
    
    int handleInput();
    
    void onLevelCompleted(Game* game);
    
    void reset();
    
    std::vector<FinalScoreSparkle*>& getSparkles();
    
    GameButton* getReplayButton();
    GameButton* getContinueButton();
    GameButton* getLeaderboardsButton();
    
    ScoreIcon* getClockIcon();
    ScoreIcon* getCarrotIcon();
    ScoreIcon* getGoldenCarrotIcon();
    ScoreIcon* getVialIcon();
    
private:
    std::vector<FinalScoreSparkle*> m_sparkles;
    
    GameButton* m_replayButton;
    GameButton* m_continueButton;
    GameButton* m_leaderboardsButton;
    
    ScoreIcon* m_clockIcon;
    ScoreIcon* m_carrotIcon;
    ScoreIcon* m_goldenCarrotIcon;
    ScoreIcon* m_vialIcon;
    
    Game* m_game;
    bool m_isLevelCompleted;
};

class ScoreIcon : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    ScoreIcon(float x, float y, float width, float height, int type);
    
    virtual void update(float deltaTime);
    
    void animateIn();
    
    int getType();
    
    bool isHidden();
    
    void setHidden(bool isHidden);
    
private:
    int m_iType;
    bool m_isAnimatingIn;
    bool m_isHidden;
};

class FinalScoreSparkle : public PhysicalEntity
{
    RTTI_DECL;
    
public:
    FinalScoreSparkle(float x, float y, float width, float height);
};

#endif /* defined(__nosfuratu__LevelCompletePanel__) */
