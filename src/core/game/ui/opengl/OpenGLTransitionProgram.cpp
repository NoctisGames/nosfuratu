//
//  OpenGLTransitionProgram.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLTransitionProgram.h"

OpenGLTransitionProgramStruct OpenGLTransitionProgram::build(GLuint program)
{
    return (OpenGLTransitionProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_from"),
        glGetUniformLocation(program, "u_to"),
        glGetUniformLocation(program, "u_progress")
    };
}
