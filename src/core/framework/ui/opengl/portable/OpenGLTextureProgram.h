//
//  OpenGLTextureProgram.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/4/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLTextureProgram__
#define __noctisgames__OpenGLTextureProgram__

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
} OpenGLTextureProgramStruct;

class OpenGLTextureProgram
{
public:
    static OpenGLTextureProgramStruct build(GLuint program);
    
private:
    OpenGLTextureProgram();
};

#endif /* defined(__noctisgames__OpenGLTextureProgram__) */
