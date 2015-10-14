//
//  CaveSkeleton.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/30/15.
//  Copyright © 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__CaveSkeleton__
#define __nosfuratu__CaveSkeleton__

#include "PhysicalEntity.h"

typedef enum
{
    CaveSkeletonType_One,
    CaveSkeletonType_Two,
    CaveSkeletonType_Three
} CaveSkeletonType;

class CaveSkeleton : public PhysicalEntity
{
public:
    static CaveSkeleton* create(float x, float y, int type);
    
    CaveSkeleton(float x, float y, float width, float height, CaveSkeletonType type);
    
    CaveSkeletonType getEnumType();
    
    int getType();
    
private:
    CaveSkeletonType m_type;
};

class CaveSkeletonOne : public CaveSkeleton
{
public:
    CaveSkeletonOne(float x, float y, float width = 2.9239766081871346f, float height = 1.7781629116117852f) : CaveSkeleton(x, y, width, height, CaveSkeletonType_One) {}
};

class CaveSkeletonTwo : public CaveSkeleton
{
public:
    CaveSkeletonTwo(float x, float y, float width = 3.064327485380117f, float height = 2.3864818024263434f) : CaveSkeleton(x, y, width, height, CaveSkeletonType_Two) {}
};

class CaveSkeletonThree : public CaveSkeleton
{
public:
    CaveSkeletonThree(float x, float y, float width = 3.064327485380117f, float height = 2.3864818024263434f) : CaveSkeleton(x, y, width, height, CaveSkeletonType_Three) {}
};

#endif /* defined(__nosfuratu__CaveSkeleton__) */
