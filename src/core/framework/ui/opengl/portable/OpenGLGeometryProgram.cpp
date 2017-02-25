//
//  OpenGLGeometryProgram.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/4/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLGeometryProgram.h"

OpenGLGeometryProgramStruct OpenGLGeometryProgram::build(GLuint program)
{
    return (OpenGLGeometryProgramStruct)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color")
    };
}
