//
//  OpenGLTransDeathProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLTransDeathProgram.h"

OpenGLTransDeathProgramStruct OpenGLTransDeathProgram::build(GLuint program)
{
    return (OpenGLTransDeathProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_TextureUnitGrayMap"),
        glGetUniformLocation(program, "u_TimeElapsed")
    };
}
