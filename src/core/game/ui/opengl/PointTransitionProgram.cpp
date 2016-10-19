//
//  PointTransitionProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "PointTransitionProgram.h"

PointTransitionProgramStruct PointTransitionProgram::build(GLuint program)
{
    return (PointTransitionProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetUniformLocation(program, "u_CenterX"),
        glGetUniformLocation(program, "u_CenterY"),
        glGetUniformLocation(program, "u_from"),
        glGetUniformLocation(program, "u_to"),
        glGetUniformLocation(program, "u_progress")
    };
}