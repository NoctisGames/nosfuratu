//
//  MathUtil.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef MathUtil_h
#define MathUtil_h

#include <math.h>

inline float clamp(float x, float upper, float lower)
{
    return fminf(upper, fmaxf(x, lower));
}

#endif /* MathUtil_h */
