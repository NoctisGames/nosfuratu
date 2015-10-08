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
#include "CaveSkeleton.h"
#include "Ground.h"
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

#include <memory>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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
    
    bool isJonHit(float deltaTime);
    
    bool isJonLandingOnSpring(float deltaTime);
    
    bool isSpinningBackFistDelivered(float deltaTime);
    
    std::vector<BackgroundSky>& getBackgroundSkies();
    
    std::vector<BackgroundTrees>& getBackgroundTrees();
    
    std::vector<BackgroundCave>& getBackgroundCaves();
    
    std::vector<Tree>& getTrees();
    
    std::vector<CaveSkeleton>& getCaveSkeletons();
    
    std::vector<Ground>& getGrounds();
    
    std::vector<LogVerticalTall>& getLogVerticalTalls();
    
    std::vector<LogVerticalShort>& getLogVerticalShorts();
    
    std::vector<Thorns>& getThorns();
    
    std::vector<Stump>& getStumps();
    
    std::vector<SideSpike>& getSideSpikes();
    
    std::vector<UpwardSpike>& getUpwardSpikes();
    
    std::vector<JumpSpring>& getJumpSprings();
    
    std::vector<Rock>& getRocks();
    
    std::vector<GroundPlatform>& getPlatforms();
    
    std::vector<EndSign>& getEndSigns();
    
    std::vector<Carrot>& getCarrots();
    
    std::vector<GoldenCarrot>& getGoldenCarrots();
    
    std::vector<Jon>& getJons();
    
    Jon& getJon();
    
    float getFarRight();
    
    float getStateTime();
    
    bool isLoaded();
    
private:
    std::unique_ptr<std::vector<BackgroundSky>> m_backgroundSkies;
    std::unique_ptr<std::vector<BackgroundTrees>> m_backgroundTrees;
    std::unique_ptr<std::vector<BackgroundCave>> m_backgroundCaves;
    std::unique_ptr<std::vector<Tree>> m_trees;
    std::unique_ptr<std::vector<CaveSkeleton>> m_caveSkeletons;
    std::unique_ptr<std::vector<Ground>> m_grounds;
    std::unique_ptr<std::vector<LogVerticalTall>> m_logVerticalTalls;
    std::unique_ptr<std::vector<LogVerticalShort>> m_logVerticalShorts;
    std::unique_ptr<std::vector<Thorns>> m_thorns;
    std::unique_ptr<std::vector<Stump>> m_stumps;
    std::unique_ptr<std::vector<SideSpike>> m_sideSpikes;
    std::unique_ptr<std::vector<UpwardSpike>> m_upwardSpikes;
    std::unique_ptr<std::vector<JumpSpring>> m_jumpSprings;
    std::unique_ptr<std::vector<Rock>> m_rocks;
    std::unique_ptr<std::vector<GroundPlatform>> m_platforms;
    std::unique_ptr<std::vector<EndSign>> m_endSigns;
    std::unique_ptr<std::vector<Carrot>> m_carrots;
    std::unique_ptr<std::vector<GoldenCarrot>> m_goldenCarrots;
    std::unique_ptr<std::vector<Jon>> m_jons;
    
    float m_fStateTime;
    bool m_isLoaded;
    
    template<typename T>
    void copyPhysicalEntities(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight()));
        }
    }
    
    template<typename T>
    void copyTrees(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getTreeType()));
        }
    }
    
    template<typename T>
    void copyCaveSkeletons(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getCaveSkeletonType()));
        }
    }
    
    template<typename T>
    void copyGrounds(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getGroundType(), i->getBoundsHeightFactor()));
        }
    }
    
    template<typename T>
    void copyUpwardSpikes(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getUpwardSpikeType()));
        }
    }
    
    template<typename T>
    void copyJumpSprings(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getJumpSpringType()));
        }
    }
    
    template<typename T>
    void copyRocks(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->isCracked()));
        }
    }
    
    template<typename T>
    void copyPlatforms(std::vector<T>& itemsFrom, std::vector<T>& itemsTo)
    {
        for (typename std::vector<T>::iterator i = itemsFrom.begin(); i != itemsFrom.end(); i++)
        {
            itemsTo.push_back(T(i->getPosition().getX(), i->getPosition().getY(), i->getWidth(), i->getHeight(), i->getGroundPlatformType()));
        }
    }
    
    template<typename T>
    void loadArray(std::vector<T>& items, rapidjson::Document& d, const char * key)
    {
        using namespace rapidjson;
        
        if (d.HasMember(key))
        {
            Value& v = d[key];
            assert(v.IsArray());
            for (SizeType i = 0; i < v.Size(); i++)
            {
                items.push_back(T::deserialize(v[i]));
            }
        }
    }
    
    template<typename T>
    void saveArray(std::vector<T>& items, rapidjson::Writer<rapidjson::StringBuffer>& w, const char * key)
    {
        w.String(key);
        w.StartArray();
        
        for (typename std::vector<T>::iterator i = items.begin(); i != items.end(); i++)
        {
            i->serialize(w);
        }
        
        w.EndArray();
    }
};

#endif /* defined(__nosfuratu__Game__) */
