//
//  FramebufferToScreenProgram.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "FramebufferToScreenProgram.h"

FramebufferToScreenProgramStruct FramebufferToScreenProgram::build(GLuint program)
{
    return (FramebufferToScreenProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_Direction")
    };
}