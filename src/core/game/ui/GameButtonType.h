//
//  GameButtonType.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/3/17.
//  Copyright © 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__GameButtonType__
#define __nosfuratu__GameButtonType__

typedef enum
{
    GameButtonType_ToggleMusic,
    GameButtonType_ToggleSound,
    GameButtonType_BackToTitle,
    GameButtonType_ViewOpeningCutscene,
    GameButtonType_GameController,
    GameButtonType_Leaderboards,
    GameButtonType_Achievements,
    GameButtonType_SignInOut,
    GameButtonType_BackToLevelSelect,
    GameButtonType_LevelEditor,
    GameButtonType_Replay,
    GameButtonType_ContinueToLevelSelect,
    GameButtonType_PostScore
} GameButtonType;

#endif /* defined(__nosfuratu__GameButtonType__) */
