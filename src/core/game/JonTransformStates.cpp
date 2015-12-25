//
//  JonTransformStates.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/31/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "JonTransformStates.h"

/// Transform to Vampire ///

RabbitToVampire * RabbitToVampire::getInstance()
{
    static RabbitToVampire *instance = new RabbitToVampire();
    
    return instance;
}

void RabbitToVampire::enter(Jon* jon)
{
    
}

void RabbitToVampire::execute(Jon* jon)
{
    // TODO
}

void RabbitToVampire::exit(Jon* jon)
{
    // TODO
}

/// Transform to Rabbit ///

VampireToRabbit * VampireToRabbit::getInstance()
{
    static VampireToRabbit *instance = new VampireToRabbit();
    
    return instance;
}

void VampireToRabbit::enter(Jon* jon)
{
    
}

void VampireToRabbit::execute(Jon* jon)
{
    // TODO
}

void VampireToRabbit::exit(Jon* jon)
{
    // TODO
}
