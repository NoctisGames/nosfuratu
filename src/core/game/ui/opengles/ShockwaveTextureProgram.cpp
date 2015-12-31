//
//  ShockwaveTextureProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "ShockwaveTextureProgram.h"

ShockwaveTextureProgramStruct ShockwaveTextureProgram::build(GLuint program)
{
    return (ShockwaveTextureProgramStruct)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_CenterX"),
        glGetUniformLocation(program, "u_CenterY"),
        glGetUniformLocation(program, "u_TimeElapsed")
    };
}