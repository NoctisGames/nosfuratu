//
//  Ground.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/5/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Ground__
#define __nosfuratu__Ground__

#include "PhysicalEntity.h"

typedef enum
{
    GroundType_Grass_with_Cave_Large,
    GroundType_Grass_with_Cave_Medium,
    GroundType_Grass_with_Cave_Small,
    GroundType_Grass_with_Cave_End_Left,
    GroundType_Grass_with_Cave_End_Right,
    
    GroundType_Grass_without_Cave_Large,
    GroundType_Grass_without_Cave_Medium,
    GroundType_Grass_without_Cave_Small,
    GroundType_Grass_without_Cave_End_Left,
    GroundType_Grass_without_Cave_End_Right,
    
    GroundType_Cave_Large,
    GroundType_Cave_Medium,
    GroundType_Cave_Small,
    GroundType_Cave_End_Left,
    GroundType_Cave_End_Right,
    
    GroundType_Cave_Raised_Large,
    GroundType_Cave_Raised_Medium,
    GroundType_Cave_Raised_Small,
    GroundType_Cave_Raised_End_Left,
    GroundType_Cave_Raised_End_Right
} GroundType;

#define GROUND_LARGE_WIDTH 47.90643274853801f
#define GROUND_MEDIUM_WIDTH 23.976608187134502f
#define GROUND_SMALL_WIDTH 12.0f

#define GROUND_GRASS_WITH_CAVE_HEIGHT 2.643847487001733f
#define GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR 0.54867256637168f
#define GROUND_GRASS_WITH_CAVE_Y 8.9025129982669

#define GROUND_CAVE_HEIGHT 4.726169844020797f
#define GROUND_CAVE_BOUNDS_HEIGHT_FACTOR 0.25742574257426f

#define GROUND_GRASS_WITHOUT_CAVE_HEIGHT 10.224436741767764f
#define GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR 0.88329519450801f

#define GROUND_CAVE_RAISED_HEIGHT 6.34055459272097f
#define GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR 0.84444444444444f

class Ground : public PhysicalEntity
{
public:
    static Ground* create(float x, float y, int type);
    
    Ground(float x, float y, float width, float height, float boundsHeightFactor, GroundType type);
    
    virtual void updateBounds();
    
    bool isForegroundMore();
    
    GroundType getEnumType();
    
    int getType();
    
private:
    GroundType m_type;
    float m_fBoundsHeightFactor;
};

class GroundGrassWithCaveLarge : public Ground
{
public:
    GroundGrassWithCaveLarge(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = GROUND_LARGE_WIDTH, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_Large) {}
};

class GroundGrassWithCaveMedium : public Ground
{
public:
    GroundGrassWithCaveMedium(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = GROUND_MEDIUM_WIDTH, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_Medium) {}
};

class GroundGrassWithCaveSmall : public Ground
{
public:
    GroundGrassWithCaveSmall(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = GROUND_SMALL_WIDTH, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_Small) {}
};

class GroundGrassWithCaveEndLeft : public Ground
{
public:
    GroundGrassWithCaveEndLeft(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = 1.7309941520467835f, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_End_Left) {}
};

class GroundGrassWithCaveEndRight : public Ground
{
public:
    GroundGrassWithCaveEndRight(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = 1.6140350877192982f, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_End_Right) {}
};

class GroundGrassWithoutCaveLarge : public Ground
{
public:
    GroundGrassWithoutCaveLarge(float x, float y = 0, float width = GROUND_LARGE_WIDTH, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_Large) {}
};

class GroundGrassWithoutCaveMedium : public Ground
{
public:
    GroundGrassWithoutCaveMedium(float x, float y = 0, float width = GROUND_MEDIUM_WIDTH, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_Medium) {}
};

class GroundGrassWithoutCaveSmall : public Ground
{
public:
    GroundGrassWithoutCaveSmall(float x, float y = 0, float width = GROUND_SMALL_WIDTH, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_Small) {}
};

class GroundGrassWithoutCaveEndLeft : public Ground
{
public:
    GroundGrassWithoutCaveEndLeft(float x, float y = 0, float width = 1.5204678362573099f, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_End_Left) {}
};

class GroundGrassWithoutCaveEndRight : public Ground
{
public:
    GroundGrassWithoutCaveEndRight(float x, float y = 0, float width = 1.6140350877192982f, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_End_Right) {}
};

class GroundCaveLarge : public Ground
{
public:
    GroundCaveLarge(float x, float y = 0, float width = GROUND_LARGE_WIDTH, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Large) {}
};

class GroundCaveMedium : public Ground
{
public:
    GroundCaveMedium(float x, float y = 0, float width = GROUND_MEDIUM_WIDTH, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Medium) {}
};

class GroundCaveSmall : public Ground
{
public:
    GroundCaveSmall(float x, float y = 0, float width = GROUND_SMALL_WIDTH, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Small) {}
};

class GroundCaveEndLeft : public Ground
{
public:
    GroundCaveEndLeft(float x, float y = 0, float width = 1.567251461988304f, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_End_Left) {}
};

class GroundCaveEndRight : public Ground
{
public:
    GroundCaveEndRight(float x, float y = 0, float width = 1.543859649122807f, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_End_Right) {}
};

class GroundCaveRaisedLarge : public Ground
{
public:
    GroundCaveRaisedLarge(float x, float y = 0, float width = GROUND_LARGE_WIDTH, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_Large) {}
};

class GroundCaveRaisedMedium : public Ground
{
public:
    GroundCaveRaisedMedium(float x, float y = 0, float width = GROUND_MEDIUM_WIDTH, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_Medium) {}
};

class GroundCaveRaisedSmall : public Ground
{
public:
    GroundCaveRaisedSmall(float x, float y = 0, float width = GROUND_SMALL_WIDTH, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_Small) {}
};

class GroundCaveRaisedEndLeft : public Ground
{
public:
    GroundCaveRaisedEndLeft(float x, float y = 0, float width = 2.198830409356725f, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_End_Left) {}
};

class GroundCaveRaisedEndRight : public Ground
{
public:
    GroundCaveRaisedEndRight(float x, float y = 0, float width = 2.198830409356725f, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_End_Right) {}
};

#endif /* defined(__nosfuratu__Ground__) */
