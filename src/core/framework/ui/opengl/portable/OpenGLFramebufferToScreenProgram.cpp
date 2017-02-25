//
//  OpenGLFramebufferToScreenProgram.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferToScreenProgram.h"

OpenGLFramebufferToScreenProgramStruct OpenGLFramebufferToScreenProgram::build(GLuint program)
{
    return (OpenGLFramebufferToScreenProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_Direction")
    };
}
