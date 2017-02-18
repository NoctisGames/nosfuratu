//
//  OpenGLShockwaveTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLShockwaveTextureProgram__
#define __nosfuratu__OpenGLShockwaveTextureProgram__

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
} OpenGLShockwaveTextureProgramStruct;

class OpenGLShockwaveTextureProgram
{
public:
    static OpenGLShockwaveTextureProgramStruct build(GLuint program);
    
private:
    OpenGLShockwaveTextureProgram();
};

#endif /* defined(__nosfuratu__OpenGLShockwaveTextureProgram__) */
