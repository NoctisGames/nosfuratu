//
//  ForegroundObjectType.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/3/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__ForegroundObjectType__
#define __nosfuratu__ForegroundObjectType__

typedef enum
{
    ForegroundObjectType_GrassPlatformLeft,
    ForegroundObjectType_GrassPlatformCenter,
    ForegroundObjectType_GrassPlatformRight,
    
    ForegroundObjectType_CavePlatformLeft,
    ForegroundObjectType_CavePlatformCenter,
    ForegroundObjectType_CavePlatformRight,
    
    ForegroundObjectType_RockLarge,
    ForegroundObjectType_RockMedium,
    ForegroundObjectType_RockSmall,
    ForegroundObjectType_RockSmallCracked,
    
    ForegroundObjectType_StumpBig,
    ForegroundObjectType_StumpSmall,
    
    ForegroundObjectType_EndSign,
    
    ForegroundObjectType_ThornsLeft,
    ForegroundObjectType_ThornsCenterSmall,
    ForegroundObjectType_ThornsCenterBig,
    ForegroundObjectType_ThornsRight,
    
    ForegroundObjectType_LogVerticalTall,
    ForegroundObjectType_LogVerticalShort,
    
    ForegroundObjectType_JumpSpringLight,
    ForegroundObjectType_JumpSpringMedium,
    ForegroundObjectType_JumpSpringHeavy,
    
    ForegroundObjectType_SpikeGrassSingle,
    ForegroundObjectType_SpikeGrassFour,
    ForegroundObjectType_SpikeGrassEight,
    
    ForegroundObjectType_SpikeCaveSingle,
    ForegroundObjectType_SpikeCaveFour,
    ForegroundObjectType_SpikeCaveEight,
    
    ForegroundObjectType_SpikeCaveCeilingSingle,
    ForegroundObjectType_SpikeCaveCeilingFour,
    ForegroundObjectType_SpikeCaveCeilingEight,
    
    ForegroundObjectType_SpikeWallSingle,
    ForegroundObjectType_SpikeWallFour,
    ForegroundObjectType_SpikeWallEight,
    
    ForegroundObjectType_SpikeStar,
    
    ForegroundObjectType_VerticalSaw,
    
    ForegroundObjectType_GiantTree,
    ForegroundObjectType_GiantShakingTree,
    ForegroundObjectType_GiantPerchTree,
    
    ForegroundObjectType_SpikeTower,
    ForegroundObjectType_SpikeTowerBg,
    
    ForegroundObjectType_JumpSpringLightFlush,
    
    ForegroundObjectType_SpikedBallRollingLeft,
    ForegroundObjectType_SpikedBallRollingRight,
    
    ForegroundObjectType_SpikedBall,
    ForegroundObjectType_SpikedBallChain,
    
    ForegroundObjectType_MetalGrassPlatform,
    ForegroundObjectType_MetalGrassPlatformLeft,
    ForegroundObjectType_MetalGrassPlatformCenter,
    ForegroundObjectType_MetalGrassPlatformRight,
    
    ForegroundObjectType_WoodPlatform,
    ForegroundObjectType_WoodBoxTop,
    ForegroundObjectType_WoodBox,
    
    ForegroundObjectType_GreenThornsLeft,
    ForegroundObjectType_GreenThornsCenterSmall,
    ForegroundObjectType_GreenThornsCenterBig,
    ForegroundObjectType_GreenThornsRight,
    
    ForegroundObjectType_Logs,
    
    ForegroundObjectType_Stone_Bottom,
    ForegroundObjectType_Stone_Middle,
    ForegroundObjectType_Stone_Top,
    ForegroundObjectType_Stone_Platform,
    
    ForegroundObjectType_Floating_Platform,
    
    ForegroundObjectType_Stone_Square,
    
    ForegroundObjectType_JumpSpringLightFlushNew
} ForegroundObjectType;

#endif /* defined(__nosfuratu__ForegroundObjectType__) */
