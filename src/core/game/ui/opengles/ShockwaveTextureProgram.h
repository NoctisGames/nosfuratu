//
//  ShockwaveTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__ShockwaveTextureProgram__
#define __gowengamedev__ShockwaveTextureProgram__

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
    GLint u_center_x_unit_location;
    GLint u_center_y_unit_location;
    GLint u_time_elapsed_unit_location;
    GLint u_is_transforming;
} ShockwaveTextureProgramStruct;

class ShockwaveTextureProgram
{
public:
    static ShockwaveTextureProgramStruct build(GLuint program);
    
private:
    ShockwaveTextureProgram();
};

#endif /* defined(__gowengamedev__ShockwaveTextureProgram__) */
