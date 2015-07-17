//
//  OpenGLESProgram.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESProgram.h"

ColorProgram OpenGLESProgram::get_color_program(GLuint program)
{
    return (ColorProgram)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color")
    };
}

TextureProgram OpenGLESProgram::get_texture_program(GLuint program)
{
    return (TextureProgram)
    {
        program,
        glGetUniformLocation(program, "u_MvpMatrix"),
        glGetAttribLocation(program, "a_Position"),
        glGetAttribLocation(program, "a_Color"),
        glGetAttribLocation(program, "a_TextureCoordinates"),
        glGetUniformLocation(program, "u_TextureUnit")
    };
}

PostProcessingSinWaveProgram OpenGLESProgram::get_post_processing_sin_wave_program(GLuint program)
{
    return (PostProcessingSinWaveProgram)
    {
        program,
        glGetUniformLocation(program, "fbo_texture"),
        glGetUniformLocation(program, "offset"),
        glGetAttribLocation(program, "v_coord")
    };
}

PostProcessingCrossHatchingProgram OpenGLESProgram::get_post_processing_cross_hatching_program(GLuint program)
{
    return (PostProcessingCrossHatchingProgram)
    {
        program,
        glGetUniformLocation(program, "fbo_texture"),
        glGetUniformLocation(program, "vx_offset"),
        glGetUniformLocation(program, "hatch_y_offset"),
        glGetUniformLocation(program, "lum_threshold_1"),
        glGetUniformLocation(program, "lum_threshold_2"),
        glGetUniformLocation(program, "lum_threshold_3"),
        glGetUniformLocation(program, "lum_threshold_4"),
        glGetAttribLocation(program, "v_coord"),
    };
}

OpenGLESProgram::OpenGLESProgram()
{
    // Hide Constructor for Singleton
}