//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

#include "Background.h"
#include "Midground.h"
#include "Ground.h"
#include "ExitGround.h"
#include "Hole.h"
#include "ForegroundObject.h"
#include "Enemy.h"
#include "CollectibleItem.h"
#include "Jon.h"
#include "EndBossSnake.h"
#include "CountHissWithMina.h"
#include "GameMarker.h"

#include <memory>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define gridXKey "gridX"
#define gridYKey "gridY"
#define typeKey "type"

class Game
{
public:
    Game();
    
    void copy(Game* game);
    
    void load(const char* json);
    
    const char* save();
    
    void reset();
    
    void update(float deltaTime);
    
    void updateAndClean(float deltaTime);
    
    void updateBackgrounds(Vector2D& cameraPosition, float deltaTime);
    
    int calcSum();
    
    bool isEntityGrounded(PhysicalEntity* entity, float deltaTime);
    
    bool isJonBlockedOnRight(float deltaTime);

	bool isJonBlockedOnLeft(float deltaTime);
    
    bool isJonBlockedVertically(float deltaTime);
    
    bool isSpinningBackFistDelivered(float deltaTime);
    
    bool isBurrowEffective(float deltaTime);
    
    bool isUpwardThrustEffective(float deltaTime);
    
    bool isDashEffective(float deltaTime);
    
    std::vector<Background *>& getBackgroundUppers();
    
    std::vector<Background *>& getBackgroundMids();
    
    std::vector<Background *>& getBackgroundMidFronts();
    
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
    
    std::vector<GameMarker *>& getMarkers();
    
    void setBestLevelStatsFlag(int bestLevelStatsFlag);
    
    int getBestLevelStatsFlag();
    
    void setCameraBounds(Rectangle* cameraBounds);
    
    Rectangle* getCameraBounds();
    
    float getFarRight();
    
    float getFarRightBottom();
    
    float getStateTime();
    
    void setNumCarrotsCollected(int numCarrotsCollected);
    
    int getNumCarrotsCollected();
    
    void setNumGoldenCarrotsCollected(int numGoldenCarrotsCollected);
    
    int getNumGoldenCarrotsCollected();
    
    int getWorld();
    
    int getLevel();
    
    bool isLoaded();

	bool hasEndSign();
    
    void calcFarRight();
    
    void setStateTime(float stateTime);
    
private:
    std::vector<Background *> m_backgroundUppers;
    std::vector<Background *> m_backgroundMids;
    std::vector<Background *> m_backgroundMidFronts;
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
    std::vector<GameMarker *> m_markers;
    Rectangle* m_cameraBounds;
    
    float m_fStateTime;
    float m_fFarRight;
    float m_fFarRightBottom;
    int m_iBestLevelStatsFlag;
    int m_iNumCarrotsCollected;
    int m_iNumGoldenCarrotsCollected;
    int m_iWorld;
    int m_iLevel;
    bool m_isLoaded;
    
    template<typename T>
    void copyPhysicalEntities(std::vector<T*>& itemsFrom, std::vector<T*>& itemsTo)
    {
        for (typename std::vector<T*>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T::create((*i)->getGridX(), (*i)->getGridY(), (*i)->getType()));
        }
    }
    
    template<typename T>
    static void serialize(T* item, rapidjson::Writer<rapidjson::StringBuffer>& w)
    {
        w.StartObject();
        w.String(gridXKey);
        w.Int(item->getGridX());
        w.String(gridYKey);
        w.Int(item->getGridY());
        int type = item->getType();
        if (type != -1)
        {
            w.String(typeKey);
            w.Int(type);
        }
        
        w.EndObject();
    }
    
    template<typename T>
    static T* deserialize(rapidjson::Value& v)
    {
        int gridX = v[gridXKey].GetInt();
        int gridY = v[gridYKey].GetInt();
        int type = 0;
        if (v.HasMember(typeKey))
        {
            type = v[typeKey].GetInt();
        }
        
        return T::create(gridX, gridY, type);
    }
    
    template<typename T>
    void loadArray(std::vector<T*>& items, rapidjson::Document& d, const char * key)
    {
        using namespace rapidjson;
        
        if (d.HasMember(key))
        {
            Value& v = d[key];
            assert(v.IsArray());
            
            for (SizeType i = 0; i < v.Size(); i++)
            {
                items.push_back(deserialize<T>(v[i]));
            }
        }
    }
    
    template<typename T>
    void saveArray(std::vector<T*>& items, rapidjson::Writer<rapidjson::StringBuffer>& w, const char * key)
    {
        w.String(key);
        w.StartArray();
        
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); i++)
        {
            serialize((*i), w);
        }
        
        w.EndArray();
    }
    
    void onLoaded();
    
    void configureGoldenCarrots();
};

#endif /* defined(__nosfuratu__Game__) */
