//
//  OpenGLSinWaveTextureProgram.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLSinWaveTextureProgram.h"

OpenGLSinWaveTextureProgramStruct OpenGLSinWaveTextureProgram::build(GLuint program)
{
    return (OpenGLSinWaveTextureProgramStruct)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_Offset")
    };
}
