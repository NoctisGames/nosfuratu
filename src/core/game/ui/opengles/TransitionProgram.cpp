//
//  TransitionProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "TransitionProgram.h"

TransitionProgramStruct TransitionProgram::build(GLuint program)
{
    return (TransitionProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_from"),
        glGetUniformLocation(program, "u_to"),
        glGetUniformLocation(program, "u_progress")
    };
}