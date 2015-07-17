//
//  OpenGLESProgram.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__OpenGLESProgram__
#define __gowengamedev__OpenGLESProgram__

extern "C"
{
#include "platform_gl.h"
#include "linmath.h"
}

typedef struct
{
    GLuint program;
    
    GLint u_mvp_matrix_location;
    GLint a_position_location;
    GLint a_color_location;
} ColorProgram;

typedef struct
{
    GLuint program;
    
    GLint u_mvp_matrix_location;
    GLint a_position_location;
    GLint a_color_location;
    GLint a_texture_coordinates_location;
    GLint u_texture_unit_location;
} TextureProgram;

typedef struct
{
    GLuint program;
    
    GLint u_fbo_texture_location;
    GLint u_offset_location;
    GLint a_v_coord_location;
} PostProcessingSinWaveProgram;

typedef struct
{
    GLuint program;
    
    GLint u_fbo_texture_location;
    GLint u_vx_offset;
    GLint u_hatch_y_offset;
    GLint u_lum_threshold_1;
    GLint u_lum_threshold_2;
    GLint u_lum_threshold_3;
    GLint u_lum_threshold_4;
    GLint a_v_coord_location;
} PostProcessingCrossHatchingProgram;

class OpenGLESProgram
{
public:
    static ColorProgram get_color_program(GLuint program);
    
    static TextureProgram get_texture_program(GLuint program);
    
    static PostProcessingSinWaveProgram get_post_processing_sin_wave_program(GLuint program);
    
    static PostProcessingCrossHatchingProgram get_post_processing_cross_hatching_program(GLuint program);
    
private:
    OpenGLESProgram();
};

#endif /* defined(__gowengamedev__OpenGLESProgram__) */
