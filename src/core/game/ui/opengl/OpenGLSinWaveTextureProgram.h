//
//  OpenGLSinWaveTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLSinWaveTextureProgram__
#define __nosfuratu__OpenGLSinWaveTextureProgram__

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
} OpenGLSinWaveTextureProgramStruct;

class OpenGLSinWaveTextureProgram
{
public:
    static OpenGLSinWaveTextureProgramStruct build(GLuint program);
    
private:
    OpenGLSinWaveTextureProgram();
};

#endif /* defined(__nosfuratu__OpenGLSinWaveTextureProgram__) */
