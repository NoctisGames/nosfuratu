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
    static TextureRegion tr = TextureRegion(0, 0, PIXEL_WIDTH_FOR_GAME, 769, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundSky.getX(), backgroundSky.getY(), PIXEL_WIDTH_FOR_GAME, 600, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundTrees& backgroundTrees)
{
    static TextureRegion tr = TextureRegion(0, 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundTrees.getX(), 776, PIXEL_WIDTH_FOR_GAME, 508, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(BackgroundCave& backgroundCave)
{
    static TextureRegion tr = TextureRegion(0, 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    tr.init(backgroundCave.getX(), 1284, PIXEL_WIDTH_FOR_GAME, 482, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Tree& tree)
{
    static TextureRegion tr1 = TextureRegion(0, 558, 270, 346, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion tr2 = TextureRegion(274, 558, 480, 439, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
    static TextureRegion tr3 = TextureRegion(758, 558, 339, 411, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static TextureRegion tr1 = TextureRegion(0, 0, 2048, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = TextureRegion(74, 1204, 1025, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = TextureRegion(1099, 1204, 513, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr4 = TextureRegion(0, 1204, 74, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr5 = TextureRegion(1612, 1204, 69, 113, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr6 = TextureRegion(0, 322, 2048, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr7 = TextureRegion(65, 763, 1025, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr8 = TextureRegion(1090, 763, 512, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr9 = TextureRegion(0, 763, 65, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr10 = TextureRegion(1602, 763, 69, 437, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr11 = TextureRegion(0, 116, 2048, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr12 = TextureRegion(67, 1320, 1025, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr13 = TextureRegion(1092, 1320, 513, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr14 = TextureRegion(0, 1320, 67, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr15 = TextureRegion(1605, 1320, 66, 202, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr16 = TextureRegion(0, 274, 2048, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr17 = TextureRegion(94, 0, 1025, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr18 = TextureRegion(1119, 0, 513, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr19 = TextureRegion(0, 0, 94, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr20 = TextureRegion(1632, 0, 94, 271, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static TextureRegion tr = TextureRegion(0, 1032, 265, 186, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(HoleCover& holeCover)
{
    static Animation anim = Animation(265, 1032, 265, 186, 1590, 372, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 12);
    
    return anim.getTextureRegion(holeCover.getStateTime());
}

TextureRegion& Assets::get(CaveExit& caveExit)
{
    static TextureRegion tr0 = TextureRegion(1, 0, 500, 288, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr1 = TextureRegion(1, 292, 500, 288, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static Animation anim = Animation(501, 0, 500, 288, 1500, 864, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 9);
    
    return anim.getTextureRegion(caveExit.getStateTime());
}

TextureRegion& Assets::get(LogVerticalTall& logVerticalTall)
{
    static TextureRegion tr = TextureRegion(1896, 824, 71, 91, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(LogVerticalShort& logVerticalShort)
{
    static TextureRegion tr = TextureRegion(1852, 1086, 65, 52, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Thorns& thorns)
{
    static TextureRegion tr = TextureRegion(1686, 1248, 122, 63, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Stump& stump)
{
    static TextureRegion tr = TextureRegion(1680, 1086, 168, 143, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(SideSpike& sideSpike)
{
    static TextureRegion tr = TextureRegion(1786, 1342, 48, 18, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(UpwardSpike& upwardSpike)
{
    static TextureRegion tr1 = TextureRegion(1686, 1316, 28, 44, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = TextureRegion(1718, 1316, 41, 46, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = TextureRegion(1764, 1316, 18, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static Animation anim1 = Animation(1852, 1142, 43, 43, 2048, 43, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 4);
    static Animation anim2 = Animation(1824, 1256, 52, 66, 2048, 66, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.05f, 4);
    
    switch (jumpSpring.getEnumType())
    {
        case JumpSpringType_Grass:
            return anim1.getTextureRegion(jumpSpring.getStateTime());
        case JumpSpringType_Cave:
        default:
            return anim2.getTextureRegion(jumpSpring.getStateTime());
    }
}

TextureRegion& Assets::get(Rock& rock)
{
    static Animation anim = Animation(0, 1528, 192, 184, 2048, 184, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 6);
    static TextureRegion tr = TextureRegion(1152, 1528, 192, 184, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static TextureRegion tr1 = TextureRegion(1680, 768, 117, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr2 = TextureRegion(1800, 768, 30, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr3 = TextureRegion(1830, 768, 116, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr4 = TextureRegion(1946, 768, 30, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr5 = TextureRegion(1680, 824, 34, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr6 = TextureRegion(1714, 824, 116, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    static TextureRegion tr7 = TextureRegion(1830, 824, 33, 50, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    
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
    static TextureRegion tr = TextureRegion(1990, 768, 34, 48, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(Carrot& carrot)
{
    static TextureRegion tr = TextureRegion(1680, 878, 102, 81, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
    return tr;
}

TextureRegion& Assets::get(GoldenCarrot& goldenCarrot)
{
    static Animation anim = Animation(0, 1716, 104, 98, 2048, 98, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 6, 0.68f, 0.06f, 0.06f, 0.06f, 0.06f, 0.06f);
    
    return anim.getTextureRegion(goldenCarrot.getStateTime());
}

TextureRegion& Assets::get(Jon& jon)
{
    static Animation jonIdleAnim = Animation(0, 1792, 256, 256, 1024, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.25f, 4);
    static Animation jonRunningAnim = Animation(0, 0, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.07f, 10);
    static Animation jonJumpingAnim = Animation(0, 512, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.09f, 7);
    static Animation jonDoubleJumpingAnim = Animation(0, 768, 256, 256, 2048, 512, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.07f, 9);
    static Animation jonFallingAnim = Animation(0, 1280, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 3);
    static Animation jonLandingAnim = Animation(0, 1536, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.05f, 4);
    static Animation jonSpinningBackFistAnimation = Animation(0, 0, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.06f, 7);
    static Animation jonBurrowAnimation = Animation(0, 256, 256, 256, 2048, 256, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, true, 0.06f, 5, 1);
    
    switch (jon.getAbilityState())
    {
        case ABILITY_SPINNING_BACK_FIST:
            return jonSpinningBackFistAnimation.getTextureRegion(jon.getAbilityStateTime());
        case ABILITY_BURROW:
            return jonBurrowAnimation.getTextureRegion(jon.getAbilityStateTime());
        case ABILITY_NONE:
        default:
            break;
    }
    
    switch (jon.getActionState())
    {
        case ACTION_JUMPING:
            return jonJumpingAnim.getTextureRegion(jon.getActionStateTime());
        case ACTION_DOUBLE_JUMPING:
            return jonDoubleJumpingAnim.getTextureRegion(jon.getActionStateTime());
        case ACTION_NONE:
        default:
            break;
    }
    
    if (jon.getPhysicalState() == PHYSICAL_GROUNDED)
    {
        if (jon.isLanding())
        {
            return jonLandingAnim.getTextureRegion(jon.getStateTime());
        }
    }
    else if (jon.getPhysicalState() == PHYSICAL_IN_AIR)
    {
        if (jon.isFalling())
        {
            return jonFallingAnim.getTextureRegion(jon.getStateTime());
        }
    }
    
    return jon.isMoving() ? jonRunningAnim.getTextureRegion(jon.getStateTime()) : jonIdleAnim.getTextureRegion(jon.getStateTime());
}

TextureRegion& Assets::get(DustCloud& dustCloud)
{
    static Animation anim = Animation(1200, 852, 115, 60, 2048, 60, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048, false, 0.08f, 5);
    
    return anim.getTextureRegion(dustCloud.getStateTime());
}

TextureRegion& Assets::get(BackButton& backButton)
{
    static TextureRegion tr = TextureRegion(1824, 48, 189, 61, TEXTURE_SIZE_2048, TEXTURE_SIZE_2048);
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

void Assets::setMusicId(short musicId)
{
    m_sMusicId = musicId;
}

short Assets::getMusicId()
{
    return m_sMusicId;
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

Assets::Assets()
{
    // Hide Constructor for Singleton
}