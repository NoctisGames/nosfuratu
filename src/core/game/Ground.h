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

#define GROUND_LARGE_WIDTH 23.970037453183522f
#define GROUND_MEDIUM_WIDTH 11.985018726591761f
#define GROUND_SMALL_WIDTH 5.992509363295881f

#define GROUND_GRASS_WITH_CAVE_HEIGHT 2.612516644474035f
#define GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR 0.53669724770642f
#define GROUND_GRASS_WITH_CAVE_Y 8.916111850865512f

#define GROUND_CAVE_HEIGHT 4.494007989347537f
#define GROUND_CAVE_BOUNDS_HEIGHT_FACTOR 0.25742574257426f

#define GROUND_GRASS_WITHOUT_CAVE_HEIGHT 10.198402130492676f
#define GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR 0.88366627497062f

#define GROUND_CAVE_RAISED_HEIGHT 6.327563249001331f
#define GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR 0.84444444444444f

class Ground : public PhysicalEntity
{
public:
    static Ground* create(float x, float y, int type);
    
    Ground(float x, float y, float width, float height, float boundsHeightFactor, GroundType type);
    
    virtual void updateBounds();
    
    bool is_world_1_cave();
    
    bool is_world_1_ground_w_cave();
    
    bool is_world_1_ground_wo_cave();
    
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
    GroundGrassWithCaveEndLeft(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = 1.7258426966292135f, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_End_Left) {}
};

class GroundGrassWithCaveEndRight : public Ground
{
public:
    GroundGrassWithCaveEndRight(float x, float y = GROUND_GRASS_WITH_CAVE_Y, float width = 1.6059925093632959f, float height = GROUND_GRASS_WITH_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITH_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_with_Cave_End_Right) {}
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
    GroundGrassWithoutCaveEndLeft(float x, float y = 0, float width = 1.4861423220973782f, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_End_Left) {}
};

class GroundGrassWithoutCaveEndRight : public Ground
{
public:
    GroundGrassWithoutCaveEndRight(float x, float y = 0, float width = 1.641947565543071f, float height = GROUND_GRASS_WITHOUT_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_GRASS_WITHOUT_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Grass_without_Cave_End_Right) {}
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
    GroundCaveEndLeft(float x, float y = 0, float width = 1.5340823970037454f, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_End_Left) {}
};

class GroundCaveEndRight : public Ground
{
public:
    GroundCaveEndRight(float x, float y = 0, float width = 1.5340823970037454f, float height = GROUND_CAVE_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_End_Right) {}
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
    GroundCaveRaisedEndLeft(float x, float y = 0, float width = 2.1932584269662923f, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_End_Left) {}
};

class GroundCaveRaisedEndRight : public Ground
{
public:
    GroundCaveRaisedEndRight(float x, float y = 0, float width = 2.1932584269662923f, float height = GROUND_CAVE_RAISED_HEIGHT) : Ground(x, y, width, height, GROUND_CAVE_RAISED_BOUNDS_HEIGHT_FACTOR, GroundType_Cave_Raised_End_Right) {}
};

#endif /* defined(__nosfuratu__Ground__) */
