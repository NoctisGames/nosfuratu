//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

class Vector2D;
class PhysicalEntity;
class Background;
class Midground;
class Ground;
class ExitGround;
class Hole;
class ForegroundObject;
class CountHissWithMina;
class EndBossSnake;
class Enemy;
class CollectibleItem;
class Jon;
class ExtraForegroundObject;
class ForegroundCoverObject;
class GameMarker;
class NGRect;

#include <vector>

class Game
{
public:
    Game();
    
    ~Game();
    
    void copy(Game* game);
    
    void load(const char* json);
    
    const char* save();
    
    void reset();
    
    void update(float deltaTime);
    
    void updateAndClean(float deltaTime, bool onlyJonCollectiblesAndCountHiss = false);
    
    void updateBackgrounds(Vector2D& cameraPosition, float deltaTime);
    
    int calcSum();
    
    bool isEntityGrounded(PhysicalEntity* entity, float deltaTime);
    
    bool isJonBlockedOnRight(float deltaTime);

	bool isJonBlockedOnLeft(float deltaTime);
    
    bool isJonBlockedVertically(float deltaTime);
    
    bool isBurrowEffective(float deltaTime);
    
    bool isUpwardThrustEffective(float deltaTime);
    
    bool isDashEffective(float deltaTime);
    
    std::vector<Background *>& getBackgroundUppers();
    
    std::vector<Background *>& getBackgroundMids();
    
    std::vector<Background *>& getBackgroundLowerBacks();
    
    std::vector<Background *>& getBackgroundLowers();
    
    std::vector<Midground *>& getMidgrounds();
    
    std::vector<Background *>& getBackgroundMidgroundCovers();
    
    std::vector<Ground *>& getGrounds();
    
    std::vector<Ground *>& getPits();
    
    std::vector<ExitGround *>& getExitGrounds();
    
    std::vector<Hole *>& getHoles();
    
    std::vector<ForegroundObject *>& getForegroundObjects();
    
    std::vector<ForegroundObject *>& getMidBossForegroundObjects();
    
    std::vector<ForegroundObject *>& getEndBossForegroundObjects();
    
    std::vector<CountHissWithMina *>& getCountHissWithMinas();
    
    CountHissWithMina& getCountHissWithMina();
    
    std::vector<EndBossSnake *>& getEndBossSnakes();
    
    EndBossSnake& getEndBossSnake();
    
    EndBossSnake* getEndBossSnakeP();
    
    std::vector<Enemy *>& getEnemies();
    
    std::vector<CollectibleItem *>& getCollectibleItems();
    
    std::vector<Jon *>& getJons();
    
    Jon& getJon();
    
    Jon* getJonP();
    
    std::vector<ExtraForegroundObject *>& getExtraForegroundObjects();
    
    std::vector<ForegroundCoverObject *>& getForegroundCoverObjects();
    
    std::vector<GameMarker *>& getMarkers();
    
    void setBestLevelStatsFlag(int bestLevelStatsFlag);
    
    int getBestLevelStatsFlag();
    
    void setCameraBounds(NGRect* cameraBounds);
    
    NGRect* getCameraBounds();
    
    float getFarRight();
    
    float getFarRightBottom();
    
    float getCamFarRight();
    
    float getCamFarRightBottom();
    
    float getStateTime();
    
    void setNumCarrotsCollected(int numCarrotsCollected);
    
    int getNumCarrotsCollected();
    
    void setNumGoldenCarrotsCollected(int numGoldenCarrotsCollected);
    
    int getNumGoldenCarrotsCollected();
    
    void setNumVialsCollected(int numVialsCollected);
    
    int getNumVialsCollected();
    
    int getWorld();
    
    int getLevel();
    
    bool isLoaded();

	bool hasEndSign();
    
    void calcFarRight();
    
    void setStateTime(float stateTime);
    
    void setIsLevelEditor(bool isLevelEditor);
    
    bool isLevelEditor();
    
private:
    std::vector<Background *> m_backgroundUppers;
    std::vector<Background *> m_backgroundMids;
    std::vector<Background *> m_backgroundLowerBacks;
    std::vector<Background *> m_backgroundLowers;
    std::vector<Midground *> m_midgrounds;
    std::vector<Background *> m_backgroundMidgroundCovers;
    std::vector<Ground *> m_grounds;
    std::vector<Ground *> m_pits;
    std::vector<ExitGround *> m_exitGrounds;
    std::vector<Hole *> m_holes;
    std::vector<ForegroundObject *> m_foregroundObjects;
    std::vector<ForegroundObject *> m_midBossForegroundObjects;
    std::vector<ForegroundObject *> m_endBossForegroundObjects;
    std::vector<CountHissWithMina *> m_countHissWithMinas;
    std::vector<EndBossSnake *> m_endBossSnakes;
    std::vector<Enemy *> m_enemies;
    std::vector<CollectibleItem *> m_collectibleItems;
    std::vector<Jon *> m_jons;
    std::vector<ExtraForegroundObject *> m_extraForegroundObjects;
    std::vector<ForegroundCoverObject *> m_foregroundCoverObjects;
    std::vector<GameMarker *> m_markers;
    NGRect* m_cameraBounds;
    
    float m_fStateTime;
    float m_fFarRight;
    float m_fFarRightBottom;
    float m_fCamFarRight;
    float m_fCamFarRightBottom;
    int m_iBestLevelStatsFlag;
    int m_iNumCarrotsCollected;
    int m_iNumGoldenCarrotsCollected;
    int m_iNumVialsCollected;
    int m_iWorld;
    int m_iLevel;
    bool m_isLevelEditor;
    
    void onLoaded();
    
    void configureGoldenCarrots();
};

#endif /* defined(__nosfuratu__Game__) */
