//
//  Game.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Game__
#define __nosfuratu__Game__

#include "BackgroundSky.h"
#include "BackgroundTrees.h"
#include "BackgroundCave.h"
#include "Tree.h"
#include "Ground.h"
#include "Hole.h"
#include "CaveExit.h"
#include "LogVerticalTall.h"
#include "LogVerticalShort.h"
#include "Thorns.h"
#include "Stump.h"
#include "SideSpike.h"
#include "UpwardSpike.h"
#include "JumpSpring.h"
#include "Rock.h"
#include "GroundPlatform.h"
#include "EndSign.h"
#include "Carrot.h"
#include "GoldenCarrot.h"
#include "Jon.h"
#include "SnakeEnemy.h"
#include "SnakeGrunt.h"
#include "SnakeHorned.h"

#include <memory>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define xKey "x"
#define ykey "y"
#define widthKey "width"
#define heightKey "height"
#define typeKey "type"

class Game
{
public:
    Game();
    
    void copy(Game* game);
    
    void load(const char* json);
    
    const char* save();
    
    void reset();
    
    void updateAndClean(float deltaTime);
    
    int calcSum();
    
    bool isJonGrounded(float deltaTime);
    
    bool isJonBlockedHorizontally(float deltaTime);
    
    bool isJonBlockedVertically(float deltaTime);
    
    bool isJonHit();
    
    bool isJonLandingOnSpring(float deltaTime);
    
    bool isJonLandingOnEnemy(float deltaTime);
    
    bool isSpinningBackFistDelivered(float deltaTime);
    
    bool isBurrowEffective();
    
    bool isUpwardThrustEffectiveAgainstEnemy();
    
    std::vector<std::unique_ptr<BackgroundSky>>& getBackgroundSkies();
    
    std::vector<std::unique_ptr<BackgroundTrees>>& getBackgroundTrees();
    
    std::vector<std::unique_ptr<BackgroundCave>>& getBackgroundCaves();
    
    std::vector<std::unique_ptr<Tree>>& getTrees();
    
    std::vector<std::unique_ptr<Ground>>& getGrounds();
    
    std::vector<std::unique_ptr<Hole>>& getHoles();
    
    std::vector<std::unique_ptr<CaveExit>>& getCaveExits();
    
    std::vector<std::unique_ptr<LogVerticalTall>>& getLogVerticalTalls();
    
    std::vector<std::unique_ptr<LogVerticalShort>>& getLogVerticalShorts();
    
    std::vector<std::unique_ptr<Thorns>>& getThorns();
    
    std::vector<std::unique_ptr<Stump>>& getStumps();
    
    std::vector<std::unique_ptr<SideSpike>>& getSideSpikes();
    
    std::vector<std::unique_ptr<UpwardSpike>>& getUpwardSpikes();
    
    std::vector<std::unique_ptr<JumpSpring>>& getJumpSprings();
    
    std::vector<std::unique_ptr<Rock>>& getRocks();
    
    std::vector<std::unique_ptr<GroundPlatform>>& getPlatforms();
    
    std::vector<std::unique_ptr<EndSign>>& getEndSigns();
    
    std::vector<std::unique_ptr<Carrot>>& getCarrots();
    
    std::vector<std::unique_ptr<GoldenCarrot>>& getGoldenCarrots();
    
    std::vector<std::unique_ptr<SnakeGrunt>>& getSnakeGruntEnemies();
    
    std::vector<std::unique_ptr<SnakeHorned>>& getSnakeHornedEnemies();
    
    std::vector<std::unique_ptr<Jon>>& getJons();
    
    Jon& getJon();
    
    float getFarRight();
    
    float getStateTime();
    
    int getNumTotalCarrots();
    
    int getNumTotalGoldenCarrots();
    
    bool isLoaded();
    
private:
    std::vector<std::unique_ptr<BackgroundSky>> m_backgroundSkies;
    std::vector<std::unique_ptr<BackgroundTrees>> m_backgroundTrees;
    std::vector<std::unique_ptr<BackgroundCave>> m_backgroundCaves;
    std::vector<std::unique_ptr<Tree>> m_trees;
    std::vector<std::unique_ptr<Ground>> m_grounds;
    std::vector<std::unique_ptr<Hole>> m_holes;
    std::vector<std::unique_ptr<CaveExit>> m_caveExits;
    std::vector<std::unique_ptr<LogVerticalTall>> m_logVerticalTalls;
    std::vector<std::unique_ptr<LogVerticalShort>> m_logVerticalShorts;
    std::vector<std::unique_ptr<Thorns>> m_thorns;
    std::vector<std::unique_ptr<Stump>> m_stumps;
    std::vector<std::unique_ptr<SideSpike>> m_sideSpikes;
    std::vector<std::unique_ptr<UpwardSpike>> m_upwardSpikes;
    std::vector<std::unique_ptr<JumpSpring>> m_jumpSprings;
    std::vector<std::unique_ptr<Rock>> m_rocks;
    std::vector<std::unique_ptr<GroundPlatform>> m_platforms;
    std::vector<std::unique_ptr<EndSign>> m_endSigns;
    std::vector<std::unique_ptr<Carrot>> m_carrots;
    std::vector<std::unique_ptr<GoldenCarrot>> m_goldenCarrots;
    std::vector<std::unique_ptr<SnakeGrunt>> m_snakeGruntEnemies;
    std::vector<std::unique_ptr<SnakeHorned>> m_snakeHornedEnemies;
    std::vector<std::unique_ptr<Jon>> m_jons;
    
    float m_fStateTime;
    int m_iNumTotalCarrots;
    int m_iNumTotalGoldenCarrots;
    bool m_isLoaded;
    
    template<typename T>
    void copyPhysicalEntities(std::vector<std::unique_ptr<T>>& itemsFrom, std::vector<std::unique_ptr<T>>& itemsTo)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(std::unique_ptr<T>(T::create((*i)->getPosition().getX(), (*i)->getPosition().getY(), (*i)->getType())));
        }
    }
    
    template<typename T>
    static void serialize(T* item, rapidjson::Writer<rapidjson::StringBuffer>& w)
    {
        w.StartObject();
        w.String(xKey);
        w.Double(item->getPosition().getX());
        w.String(ykey);
        w.Double(item->getPosition().getY());
        int type = item->getType();
        if (type != -1)
        {
            w.String(typeKey);
            w.Double(type);
        }
        
        w.EndObject();
    }
    
    template<typename T>
    static T* deserialize(rapidjson::Value& v)
    {
        float x = v[xKey].GetDouble();
        float y = v[ykey].GetDouble();
        int type = 0;
        if (v.HasMember(typeKey))
        {
            type = v[typeKey].GetInt();
        }
        
        return T::create(x, y, type);
    }
    
    template<typename T>
    void loadArray(std::vector<std::unique_ptr<T>>& items, rapidjson::Document& d, const char * key)
    {
        using namespace rapidjson;
        
        if (d.HasMember(key))
        {
            Value& v = d[key];
            assert(v.IsArray());
            for (SizeType i = 0; i < v.Size(); i++)
            {
                items.push_back(std::unique_ptr<T>(deserialize<T>(v[i])));
            }
        }
    }
    
    template<typename T>
    void saveArray(std::vector<std::unique_ptr<T>>& items, rapidjson::Writer<rapidjson::StringBuffer>& w, const char * key)
    {
        w.String(key);
        w.StartArray();
        
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            serialize((*i).get(), w);
        }
        
        w.EndArray();
    }
    
    template<typename T>
    void setGameToEntities(std::vector<std::unique_ptr<T>>& items, Game* game)
    {
        for (typename std::vector<std::unique_ptr<T>>::iterator i = items.begin(); i != items.end(); i++)
        {
            (*i)->setGame(game);
        }
    }
    
    static bool isBurstingThroughCaveToSurface(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime);
    static bool isFallingThroughCaveExit(PhysicalEntity& entity, std::vector<std::unique_ptr<CaveExit>>& items, float deltaTime);
};

#endif /* defined(__nosfuratu__Game__) */
