//
//  OpenGLShockwaveTextureProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLShockwaveTextureProgram.h"

OpenGLShockwaveTextureProgramStruct OpenGLShockwaveTextureProgram::build(GLuint program)
{
    return (OpenGLShockwaveTextureProgramStruct)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_CenterX"),
        glGetUniformLocation(program, "u_CenterY"),
        glGetUniformLocation(program, "u_TimeElapsed"),
        glGetUniformLocation(program, "u_IsTransforming")
    };
}
