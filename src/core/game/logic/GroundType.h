//
//  GroundType.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/3/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GroundType__
#define __nosfuratu__GroundType__

typedef enum
{
    GroundType_CaveExtraDeepEndLeft,
    GroundType_CaveExtraDeepSmall,
    GroundType_CaveExtraDeepMedium,
    GroundType_CaveExtraDeepLarge,
    GroundType_CaveExtraDeepEndRight,
    
    GroundType_CaveDeepEndLeft,
    GroundType_CaveDeepSmall,
    GroundType_CaveDeepMedium,
    GroundType_CaveDeepLarge,
    GroundType_CaveDeepEndRight,
    
    GroundType_CaveEndLeft,
    GroundType_CaveSmall,
    GroundType_CaveMedium,
    GroundType_CaveLarge,
    GroundType_CaveEndRight,
    
    GroundType_CaveRaisedEndLeft,
    GroundType_CaveRaisedSmall,
    GroundType_CaveRaisedMedium,
    GroundType_CaveRaisedLarge,
    GroundType_CaveRaisedEndRight,
    
    GroundType_GrassWithCaveEndLeft,
    GroundType_GrassWithCaveSmall,
    GroundType_GrassWithCaveMedium,
    GroundType_GrassWithCaveLarge,
    GroundType_GrassWithCaveEndRight,
    
    GroundType_GrassWithoutCaveEndLeft,
    GroundType_GrassWithoutCaveSmall,
    GroundType_GrassWithoutCaveMedium,
    GroundType_GrassWithoutCaveLarge,
    GroundType_GrassWithoutCaveEndRight,
    
    GroundType_GrassPitSmall,
    GroundType_GrassPitMedium,
    GroundType_GrassPitLarge,
    GroundType_GrassPitExtraLarge,
    
    GroundType_PitTunnelLeft,
    GroundType_PitTunnelCenter,
    GroundType_PitTunnelRight
} GroundType;

#endif /* defined(__nosfuratu__GroundType__) */
