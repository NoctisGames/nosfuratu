//
//  EndBossSnakeState.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/3/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__EndBossSnakeState__
#define __nosfuratu__EndBossSnakeState__

typedef enum
{
    EndBossSnakeState_Sleeping,
    EndBossSnakeState_Awakening,
    EndBossSnakeState_OpeningMouthLeft,
    EndBossSnakeState_OpenMouthLeft,
    EndBossSnakeState_ChargingLeft,
    EndBossSnakeState_Waiting,
    EndBossSnakeState_Pursuing,
    EndBossSnakeState_Damaged,
    EndBossSnakeState_OpeningMouthRight,
    EndBossSnakeState_OpenMouthRight,
    EndBossSnakeState_ChargingRight,
    EndBossSnakeState_Dying,
    EndBossSnakeState_DeadSpiritReleasing,
    EndBossSnakeState_Dead
} EndBossSnakeState;

#endif /* defined(__nosfuratu__EndBossSnakeState__) */
