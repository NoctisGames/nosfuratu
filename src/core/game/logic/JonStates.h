//
//  JonStates.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/3/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__JonStates__
#define __nosfuratu__JonStates__

typedef enum
{
    ABILITY_NONE,
    ABILITY_BURROW,
    ABILITY_STOMP,
    ABILITY_GLIDE,
    ABILITY_UPWARD_THRUST,
    ABILITY_DASH
} JonAbilityState;

typedef enum
{
    ACTION_NONE,
    ACTION_JUMPING,
    ACTION_DOUBLE_JUMPING
} JonActionState;

typedef enum
{
    PHYSICAL_GROUNDED,
    PHYSICAL_IN_AIR
} JonPhysicalState;

typedef enum
{
    JON_ALIVE,
    JON_DYING,
    JON_DEAD
} JonState;

#endif /* defined(__nosfuratu__JonStates__) */
