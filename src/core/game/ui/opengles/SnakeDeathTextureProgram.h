//
//  SnakeDeathTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/28/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__SnakeDeathTextureProgram__
#define __gowengamedev__SnakeDeathTextureProgram__

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
} SnakeDeathTextureProgramStruct;

class SnakeDeathTextureProgram
{
public:
    static SnakeDeathTextureProgramStruct build(GLuint program);
    
private:
    SnakeDeathTextureProgram();
};

#endif /* defined(__gowengamedev__SnakeDeathTextureProgram__) */
