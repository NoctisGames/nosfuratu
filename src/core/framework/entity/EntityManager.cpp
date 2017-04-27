//
//  EntityManager.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "EntityManager.h"

#include "Entity.h"

#include <cassert>

EntityManager* EntityManager::getInstance()
{
    static EntityManager instance = EntityManager();

    return &instance;
}

Entity* EntityManager::getEntityFromID(int id)const
{
    std::map<int, Entity*>::const_iterator ent = m_entityMap.find(id);
    
    assert((ent != m_entityMap.end()) && "<EntityManager::getEntityFromID>: invalid ID");
    
    return ent->second;
}

void EntityManager::registerEntity(Entity* entity)
{
    m_entityMap.insert(std::make_pair(entity->getID(), entity));
}

void EntityManager::removeEntity(Entity* entity)
{
    m_entityMap.erase(m_entityMap.find(entity->getID()));
}

void EntityManager::reset()
{
    m_entityMap.clear();
}

EntityManager::EntityManager()
{
    // Hide Constructor for Singleton
}
