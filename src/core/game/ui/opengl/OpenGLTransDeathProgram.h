//
//  OpenGLTransDeathProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLTransDeathProgram__
#define __nosfuratu__OpenGLTransDeathProgram__

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
} OpenGLTransDeathProgramStruct;

class OpenGLTransDeathProgram
{
public:
    static OpenGLTransDeathProgramStruct build(GLuint program);
    
private:
    OpenGLTransDeathProgram();
};

#endif /* defined(__nosfuratu__OpenGLTransDeathProgram__) */
