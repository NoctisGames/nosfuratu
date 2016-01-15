//
//  Assets.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Assets.h"

Assets * Assets::getInstance()
{
    static Assets *instance = new Assets();
    return instance;
}

TextureRegion& Assets::get(BackgroundSky& backgroundSky)
{
    static TextureRegion tr = TextureRegion(0, 0, PIXEL_WIDTH_FOR_BACKGROUND, 769, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundSky.getX(), backgroundSky.getY(), PIXEL_WIDTH_FOR_BACKGROUND, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundTrees& backgroundTrees)
{
    static TextureRegion tr = TextureRegion(0, 776, PIXEL_WIDTH_FOR_BACKGROUND, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundTrees.getX(), 776, PIXEL_WIDTH_FOR_BACKGROUND, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundCave& backgroundCave)
{
    static TextureRegion tr = TextureRegion(0, 1284, PIXEL_WIDTH_FOR_BACKGROUND, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundCave.getX(), 1284, PIXEL_WIDTH_FOR_BACKGROUND, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Tree& tree)
{
    static TextureRegion tr1 = createTextureRegion(TOP_RIGHT, 1536, 1212, 510, 676, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = createTextureRegion(TOP_RIGHT, 400, 760, 936, 856, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = createTextureRegion(TOP_RIGHT, 1386, 392, 660, 804, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (tree.getEnumType())
    {
        case TreeType_One:
            return tr1;
        case TreeType_Two:
            return tr2;
        case TreeType_Three:
        default:
            return tr3;
    }
}

TextureRegion& Assets::get(Ground& ground)
{
    static TextureRegion tr1 = createTextureRegion(TOP_LEFT, 1, 0, 2000, 218, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = createTextureRegion(TOP_LEFT, 144, 235, 1000, 218, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = createTextureRegion(TOP_LEFT, 1144, 235, 500, 218, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr4 = createTextureRegion(TOP_LEFT, 0, 235, 144, 218, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr5 = createTextureRegion(TOP_LEFT, 1644, 235, 134, 218, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr6 = createTextureRegion(TOP_RIGHT, 1, 0, 2000, 851, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr7 = createTextureRegion(TOP_RIGHT, 125, 875, 1000, 851, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr8 = createTextureRegion(TOP_RIGHT, 1125, 875, 500, 851, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr9 = createTextureRegion(TOP_RIGHT, 0, 875, 124, 851, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr10 = createTextureRegion(TOP_RIGHT, 1625, 875, 137, 851, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr11 = createTextureRegion(BOTTOM_LEFT, 1, 1084, 2000, 375, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr12 = createTextureRegion(BOTTOM_LEFT, 128, 1484, 1000, 375, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr13 = createTextureRegion(BOTTOM_LEFT, 1128, 1484, 500, 375, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr14 = createTextureRegion(BOTTOM_LEFT, 0, 1484, 128, 375, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr15 = createTextureRegion(BOTTOM_LEFT, 1628, 1484, 128, 375, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr16 = createTextureRegion(BOTTOM_LEFT, 1, 0, 2000, 528, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr17 = createTextureRegion(BOTTOM_LEFT, 183, 536, 1000, 528, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr18 = createTextureRegion(BOTTOM_LEFT, 1183, 536, 500, 528, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr19 = createTextureRegion(BOTTOM_LEFT, 0, 536, 183, 528, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr20 = createTextureRegion(BOTTOM_LEFT, 1683, 536, 183, 528, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (ground.getEnumType())
    {
        case GroundType_Grass_with_Cave_Large:
            return tr1;
        case GroundType_Grass_with_Cave_Medium:
            return tr2;
        case GroundType_Grass_with_Cave_Small:
            return tr3;
        case GroundType_Grass_with_Cave_End_Left:
            return tr4;
        case GroundType_Grass_with_Cave_End_Right:
            return tr5;
        case GroundType_Grass_without_Cave_Large:
            return tr6;
        case GroundType_Grass_without_Cave_Medium:
            return tr7;
        case GroundType_Grass_without_Cave_Small:
            return tr8;
        case GroundType_Grass_without_Cave_End_Left:
            return tr9;
        case GroundType_Grass_without_Cave_End_Right:
            return tr10;
        case GroundType_Cave_Large:
            return tr11;
        case GroundType_Cave_Medium:
            return tr12;
        case GroundType_Cave_Small:
            return tr13;
        case GroundType_Cave_End_Left:
            return tr14;
        case GroundType_Cave_End_Right:
            return tr15;
        case GroundType_Cave_Raised_Large:
            return tr16;
        case GroundType_Cave_Raised_Medium:
            return tr17;
        case GroundType_Cave_Raised_Small:
            return tr18;
        case GroundType_Cave_Raised_End_Left:
            return tr19;
        case GroundType_Cave_Raised_End_Right:
        default:
            return tr20;
    }
}

TextureRegion& Assets::get(Hole& hole)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 0, 1532, 266, 186, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(HoleCover& holeCover)
{
    static Animation anim = createAnimation(TOP_LEFT, 266, 1532, 266, 186, 1596, 372, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 12);
    
    return anim.getTextureRegion(holeCover.getStateTime());
}

TextureRegion& Assets::get(CaveExit& caveExit)
{
    static TextureRegion tr0 = createTextureRegion(TOP_LEFT, 20, 468, 500, 288, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr1 = createTextureRegion(TOP_LEFT, 20, 768, 500, 288, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (caveExit.getEnumType())
    {
        case CaveExitType_End:
            return tr0;
        case CaveExitType_Mid:
        default:
            return tr1;
    }
}

TextureRegion& Assets::get(CaveExitCover& caveExit)
{
    static Animation anim = createAnimation(TOP_LEFT, 530, 468, 500, 288, 1500, 864, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 9);
    
    return anim.getTextureRegion(caveExit.getStateTime());
}

TextureRegion& Assets::get(LogVerticalTall& logVerticalTall)
{
    static Animation anim = createAnimation(TOP_LEFT, 0, 1172, 258, 256, 1548, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 6);
    
    return anim.getTextureRegion(logVerticalTall.getStateTime());
}

TextureRegion& Assets::get(LogVerticalShort& logVerticalShort)
{
    static Animation anim = createAnimation(TOP_LEFT, 0, 1470, 305, 112, 1830, 224, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.04f, 7);
    
    return anim.getTextureRegion(logVerticalShort.getStateTime());
}

TextureRegion& Assets::get(Thorns& thorns)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 9, 1734, 236, 121, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Stump& stump)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 473, 1758, 325, 280, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(SideSpike& sideSpike)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 358, 1764, 93, 35, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(UpwardSpike& upwardSpike)
{
    static TextureRegion tr1 = createTextureRegion(TOP_LEFT, 370, 1859, 55, 85, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = createTextureRegion(TOP_LEFT, 269, 1745, 81, 90, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = createTextureRegion(TOP_LEFT, 320, 1849, 35, 93, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (upwardSpike.getEnumType())
    {
        case UpwardSpikeType_MetalGrass:
            return tr1;
        case UpwardSpikeType_WoodGrass:
            return tr2;
        case UpwardSpikeType_MetalCave:
        default:
            return tr3;
    }
}

TextureRegion& Assets::get(JumpSpring& jumpSpring)
{
    switch (jumpSpring.getEnumType())
    {
        case JumpSpringType_Light:
        {
            static Animation anim = createAnimation(TOP_LEFT, 931, 1958, 120, 85, 480, 85, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.04f, 3);
            return anim.getTextureRegion(jumpSpring.getStateTime());
        }
        case JumpSpringType_Medium:
        {
            static Animation anim = createAnimation(TOP_LEFT, 0, 745, 275, 141, 1925, 141, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.04f, 7);
            return anim.getTextureRegion(jumpSpring.getStateTime());
        }
        case JumpSpringType_Heavy:
        default:
        {
            static Animation anim = createAnimation(TOP_LEFT, 265, 936, 265, 219, 1855, 219, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.04f, 6);
            return anim.getTextureRegion(jumpSpring.getStateTime());
        }
    }
}

TextureRegion& Assets::get(Rock& rock)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 0, 0, 357, 357, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static Animation anim = createAnimation(TOP_LEFT, 370, 0, 357, 357, 1428, 714, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 6);
    
    if (rock.isBlowingUp())
    {
        return anim.getTextureRegion(rock.getStateTime());
    }
    else if(rock.isCracked())
    {
        return anim.getTextureRegion(0);
    }
    else
    {
        return tr;
    }
}

TextureRegion& Assets::get(GroundPlatform& platform)
{
    static TextureRegion tr1 = createTextureRegion(TOP_LEFT, 861, 1854, 227, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = createTextureRegion(TOP_LEFT, 811, 1745, 57, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = createTextureRegion(TOP_LEFT, 867, 1745, 224, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr4 = createTextureRegion(TOP_LEFT, 1091, 1745, 57, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr5 = createTextureRegion(TOP_LEFT, 1176, 1754, 64, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr6 = createTextureRegion(TOP_LEFT, 1240, 1754, 224, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr7 = createTextureRegion(TOP_LEFT, 1464, 1754, 64, 97, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    switch (platform.getEnumType())
    {
        case GroundPlatformType_GrassDefault:
            return tr1;
        case GroundPlatformType_GrassEndLeft:
            return tr2;
        case GroundPlatformType_GrassCenter:
            return tr3;
        case GroundPlatformType_GrassEndRight:
            return tr4;
        case GroundPlatformType_CaveEndLeft:
            return tr5;
        case GroundPlatformType_CaveCenter:
            return tr6;
        case GroundPlatformType_CaveEndRight:
        default:
            return tr7;
    }
}

TextureRegion& Assets::get(EndSign& endSign)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 31, 1944, 66, 93, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Carrot& carrot)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 1278, 1862, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(GoldenCarrot& goldenCarrot)
{
    static Animation anim = createAnimation(TOP_LEFT, 1418, 1946, 104, 98, 624, 98, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 6);
    if (!anim.hasFrameTimes())
    {
        anim.setFrameTimes(6, 0.68f, 0.06f, 0.06f, 0.06f, 0.06f, 0.06f);
    }
    
    return anim.getTextureRegion(goldenCarrot.getStateTime());
}

TextureRegion& Assets::get(SnakeGrunt& snakeEnemy)
{
    static Animation snakeGruntAnim = createAnimation(BOTTOM_LEFT, 0, 4, 256, 256, 1024, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 4);
    
    return snakeGruntAnim.getTextureRegion(snakeEnemy.getStateTime());
}

TextureRegion& Assets::get(SnakeHorned& snakeEnemy)
{
    static Animation snakeHornedAnim = createAnimation(BOTTOM_LEFT, 0, 772, 256, 360, 1536, 360, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 6);
    static Animation snakeHornedRiseAnim = createAnimation(BOTTOM_LEFT, 0, 1132, 256, 360, 2048, 720, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 11);
    
    if (snakeEnemy.isRising())
    {
        return snakeHornedRiseAnim.getTextureRegion(snakeEnemy.getStateTime());
    }
    
    return snakeHornedAnim.getTextureRegion(snakeEnemy.getStateTime());
}

TextureRegion& Assets::get(SnakeSpirit& snakeSpirit)
{
    static Animation deathAnim = createAnimation(BOTTOM_LEFT, 0, 260, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.10f, 10);
    
    return deathAnim.getTextureRegion(snakeSpirit.getStateTime());
}

TextureRegion& Assets::get(Jon& jon)
{
    if (jon.isTransformingIntoVampire())
    {
        static Animation transformingIntoVampire = createAnimation(BOTTOM_RIGHT, 0, 0, 256, 256, 2048, 768, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 19);
        if (!transformingIntoVampire.hasFrameTimes())
        {
            transformingIntoVampire.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return transformingIntoVampire.getTextureRegion(jon.getTransformStateTime());
    }
    else if (jon.isRevertingToRabbit())
    {
        static Animation revertingToRabbit = createAnimation(BOTTOM_RIGHT, 0, 768, 256, 256, 2048, 768, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 19);
        if (!revertingToRabbit.hasFrameTimes())
        {
            revertingToRabbit.setFrameTimes(19, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.015625f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f, 0.04f);
        }
        
        return revertingToRabbit.getTextureRegion(jon.getTransformStateTime());
    }
    
    if (jon.isVampire())
    {
        static Animation pushedBackAnim = createAnimation(TOP_LEFT, 512, 256, 256, 256, 1536, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 6);
        static Animation idleAnim = createAnimation(TOP_LEFT, 0, 1792, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.125f, 8);
        static Animation runningAnim = createAnimation(TOP_LEFT, 0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 10);
        static Animation jumpingAnim = createAnimation(TOP_LEFT, 768, 512, 256, 512, 1280, 1024, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 10);
        static Animation doubleJumpingAnim = createAnimation(TOP_LEFT, 0, 512, 256, 256, 768, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.09f, 6);
        static Animation glidingAnim = createAnimation(TOP_LEFT, 0, 1024, 256, 256, 512, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 2);
        static Animation fallingAnim = createAnimation(TOP_LEFT, 0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(TOP_LEFT, 0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.066f, 3);
        
        switch (jon.getAbilityState())
        {
            case ABILITY_GLIDE:
                return glidingAnim.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon.getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon.isLanding())
            {
                return landingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon.isFalling())
            {
                return fallingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        
        if (jon.isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon.getStateTime());
            
            if (keyFrameNumber == 1 && !jon.isRightFoot())
            {
                jon.setRightFoot(true);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon.isRightFoot())
            {
                jon.setRightFoot(false);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon.isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon.getStateTime());
        }
        
        return idleAnim.getTextureRegion(jon.getStateTime());
    }
    else
    {
        static Animation warmUpAnim = createAnimation(BOTTOM_RIGHT, 0, 0, 256, 256, 2048, 1024, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.10f, 28);
        static Animation pushedBackAnim = createAnimation(TOP_LEFT, 512, 1024, 256, 256, 1536, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.07f, 6);
        static Animation idleAnim = createAnimation(TOP_LEFT, 0, 1792, 256, 256, 1024, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.25f, 4);
        static Animation runningAnim = createAnimation(TOP_LEFT, 0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.07f, 10);
        static Animation jumpingAnim = createAnimation(TOP_LEFT, 0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.09f, 7);
        static Animation doubleJumpingAnim = createAnimation(TOP_LEFT, 0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.07f, 9);
        static Animation fallingAnim = createAnimation(TOP_LEFT, 0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 3);
        static Animation landingAnim = createAnimation(TOP_LEFT, 0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 4);
        static Animation spinningBackFistAnimation = createAnimation(TOP_RIGHT, 0, 0, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 7);
        static Animation burrowAnimation = createAnimation(TOP_RIGHT, 0, 256, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 5, 1);
        
        switch (jon.getAbilityState())
        {
            case ABILITY_SPINNING_BACK_FIST:
                return spinningBackFistAnimation.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_BURROW:
                return burrowAnimation.getTextureRegion(jon.getAbilityStateTime());
            case ABILITY_NONE:
            default:
                break;
        }
        
        switch (jon.getActionState())
        {
            case ACTION_JUMPING:
                return jumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_DOUBLE_JUMPING:
                return doubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
            case ACTION_NONE:
            default:
                break;
        }
        
        if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
        {
            if (jon.isLanding())
            {
                return landingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
        {
            if (jon.isFalling())
            {
                return fallingAnim.getTextureRegion(jon.getStateTime());
            }
        }
        
        if (jon.isMoving())
        {
            int keyFrameNumber = runningAnim.getKeyFrameNumber(jon.getStateTime());
            
            if (keyFrameNumber == 1 && !jon.isRightFoot())
            {
                jon.setRightFoot(true);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_RIGHT_CAVE);
                }
            }
            else if (keyFrameNumber == 6 && jon.isRightFoot())
            {
                jon.setRightFoot(false);
                
                if (jon.getGroundSoundType() == GROUND_SOUND_GRASS)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_GRASS);
                }
                else if (jon.getGroundSoundType() == GROUND_SOUND_CAVE)
                {
                    Assets::getInstance()->addSoundIdToPlayQueue(SOUND_FOOTSTEP_LEFT_CAVE);
                }
            }
            
            return runningAnim.getTextureRegion(keyFrameNumber);
        }
        else if (jon.isPushedBack())
        {
            return pushedBackAnim.getTextureRegion(jon.getStateTime());
        }
        
        return warmUpAnim.getTextureRegion(jon.getStateTime());
    }
}

TextureRegion& Assets::get(DustCloud& dustCloud)
{
    static Animation anim = createAnimation(TOP_LEFT, 1100, 1643, 115, 60, 575, 60, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.08f, 5);
    
    return anim.getTextureRegion(dustCloud.getStateTime());
}

TextureRegion& Assets::get(BackButton& backButton)
{
    static TextureRegion tr = createTextureRegion(TOP_LEFT, 1824, 48, 189, 61, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LevelEditorEntitiesPanel& levelEditorEntitiesPanel)
{
    static TextureRegion tr = TextureRegion(0, 33, 593, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LevelEditorActionsPanel& levelEditorActionsPanel)
{
    static TextureRegion tr = TextureRegion(840, 33, 593, 1338, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(TrashCan& trashCan)
{
    static TextureRegion trashCanTr = TextureRegion(0, 1442, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion trashCanHighlightedTr = TextureRegion(128, 1442, 128, 128, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    return trashCan.isHighlighted() ? trashCanHighlightedTr : trashCanTr;
}

TextureRegion& Assets::get(LevelSelectorPanel& levelSelectorPanel)
{
    static TextureRegion tr = TextureRegion(1538, 0, 510, 510, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

short Assets::getFirstSoundId()
{
    return m_sSoundIds.size() > 0 ? m_sSoundIds.front() : 0;
}

void Assets::addSoundIdToPlayQueue(short soundId)
{
    if (m_sSoundIds.size() < MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        m_sSoundIds.push_back(soundId);
    }
}

void Assets::eraseFirstSoundId()
{
    if (m_sSoundIds.size() > 0)
    {
        m_sSoundIds.erase(m_sSoundIds.begin());
    }
}

void Assets::setMusicId(short musicId)
{
    m_sMusicId = musicId;
}

short Assets::getMusicId()
{
    return m_sMusicId;
}

bool Assets::isUsingCompressedTextureSet()
{
    return m_isUsingCompressedTextureSet;
}

void Assets::setUsingCompressedTextureSet(bool isUsingCompressedTextureSet)
{
    m_isUsingCompressedTextureSet = isUsingCompressedTextureSet;
}

Animation Assets::createAnimation(TextureRegionQuad q, int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames)
{
    if (x % 2 != 0 || y % 2 != 0 || regionWidth % 2 != 0 || regionHeight % 2 != 0 || animationWidth % 2 != 0 || animationHeight % 2 != 0)
    {
        bool error = true;
    }
    
    if (m_isUsingCompressedTextureSet)
    {
        x = x / 2.0;
        y = y / 2.0;
        regionWidth = regionWidth / 2.0;
        regionHeight = regionHeight / 2.0;
        animationWidth = animationWidth / 2.0;
        animationHeight = animationHeight / 2.0;
        
        if (q == 1 || q == 3)
        {
            x += 1024;
        }
        
        if (q >= 2)
        {
            y += 1024;
        }
    }
    
    return Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, numFrames);
}

Animation Assets::createAnimation(TextureRegionQuad q, int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame)
{
    if (x % 2 != 0 || y % 2 != 0 || regionWidth % 2 != 0 || regionHeight % 2 != 0 || animationWidth % 2 != 0 || animationHeight % 2 != 0)
    {
        bool error = true;
    }
        
    if (m_isUsingCompressedTextureSet)
    {
        x = x / 2.0;
        y = y / 2.0;
        regionWidth = regionWidth / 2.0;
        regionHeight = regionHeight / 2.0;
        animationWidth = animationWidth / 2.0;
        animationHeight = animationHeight / 2.0;
        
        if (q == 1 || q == 3)
        {
            x += 1024;
        }
        
        if (q >= 2)
        {
            y += 1024;
        }
    }
    
    return Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, frameTime, numFrames, firstLoopingFrame);
}

TextureRegion Assets::createTextureRegion(TextureRegionQuad q, int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
{
    if (x % 2 != 0 || y % 2 != 0 || regionWidth % 2 != 0 || regionHeight % 2 != 0)
    {
        bool error = true;
    }
    
    if (m_isUsingCompressedTextureSet)
    {
        x = x / 2.0;
        y = y / 2.0;
        regionWidth = regionWidth / 2.0;
        regionHeight = regionHeight / 2.0;
        
        if (q == 1 || q == 3)
        {
            x += 1024;
        }
        
        if (q >= 2)
        {
            y += 1024;
        }
    }
    
    return TextureRegion(x, y, regionWidth, regionHeight, textureWidth, textureHeight);
}

Assets::Assets() : m_sMusicId(0), m_isUsingCompressedTextureSet(false)
{
    // Hide Constructor for Singleton
}