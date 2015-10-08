//
//  SinWaveTextureProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "SinWaveTextureProgram.h"

SinWaveTextureProgramStruct SinWaveTextureProgram::build(GLuint program)
{
    return (SinWaveTextureProgramStruct)
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