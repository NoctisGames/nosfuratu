//
//  OpenGLSinWaveTextureProgram.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/6/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLSinWaveTextureProgram__
#define __noctisgames__OpenGLSinWaveTextureProgram__

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

#endif /* defined(__noctisgames__OpenGLSinWaveTextureProgram__) */
