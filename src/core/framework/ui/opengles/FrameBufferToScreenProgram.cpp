//
//  FrameBufferToScreenProgram.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "FrameBufferToScreenProgram.h"

FrameBufferToScreenProgramStruct FrameBufferToScreenProgram::build(GLuint program)
{
    return (FrameBufferToScreenProgramStruct)
    {
        program,
        glGetAttribLocation(program, "a_Position"),
        glGetUniformLocation(program, "u_TextureUnit")
    };
}