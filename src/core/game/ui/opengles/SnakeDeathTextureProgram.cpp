//
//  SnakeDeathTextureProgram.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "SnakeDeathTextureProgram.h"

SnakeDeathTextureProgramStruct SnakeDeathTextureProgram::build(GLuint program)
{
    return (SnakeDeathTextureProgramStruct)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit"),
        glGetUniformLocation(program, "u_ColorAdditive")
    };
}