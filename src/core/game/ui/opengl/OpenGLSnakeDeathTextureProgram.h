//
//  OpenGLSnakeDeathTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLSnakeDeathTextureProgram__
#define __nosfuratu__OpenGLSnakeDeathTextureProgram__

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
} OpenGLSnakeDeathTextureProgramStruct;

class OpenGLSnakeDeathTextureProgram
{
public:
    static OpenGLSnakeDeathTextureProgramStruct build(GLuint program);
    
private:
    OpenGLSnakeDeathTextureProgram();
};

#endif /* defined(__nosfuratu__OpenGLSnakeDeathTextureProgram__) */
