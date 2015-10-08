//
//  SinWaveTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__SinWaveTextureProgram__
#define __gowengamedev__SinWaveTextureProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint u_mvp_matrix_location;
    GLint a_position_location;
    GLint a_color_location;
    GLint a_texture_coordinates_location;
    GLint u_texture_unit_location;
    GLint u_offset_unit_location;
} SinWaveTextureProgramStruct;

class SinWaveTextureProgram
{
public:
    static SinWaveTextureProgramStruct build(GLuint program);
    
private:
    SinWaveTextureProgram();
};

#endif /* defined(__gowengamedev__SinWaveTextureProgram__) */
