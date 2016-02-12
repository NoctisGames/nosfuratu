//
//  FramebufferToScreenProgram.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__FramebufferToScreenProgram__
#define __gowengamedev__FramebufferToScreenProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_texture_unit_location;
    GLint u_direction_location;
} FramebufferToScreenProgramStruct;

class FramebufferToScreenProgram
{
public:
    static FramebufferToScreenProgramStruct build(GLuint program);
    
private:
    FramebufferToScreenProgram();
};

#endif /* defined(__gowengamedev__FramebufferToScreenProgram__) */
