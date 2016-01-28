//
//  TransDeathProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__TransDeathProgram__
#define __gowengamedev__TransDeathProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_texture_unit_location;
    GLint u_texture_unit_gray_map_location;
    GLint u_time_elapsed_unit_location;
} TransDeathProgramStruct;

class TransDeathProgram
{
public:
    static TransDeathProgramStruct build(GLuint program);
    
private:
    TransDeathProgram();
};

#endif /* defined(__gowengamedev__TransDeathProgram__) */
