//
//  OpenGLPointTransitionProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLPointTransitionProgram.h"

OpenGLPointTransitionProgramStruct OpenGLPointTransitionProgram::build(GLuint program)
{
    return (OpenGLPointTransitionProgramStruct)
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
