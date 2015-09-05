//
//  FrameBufferToScreenProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__FrameBufferToScreenProgram__
#define __nosfuratu__FrameBufferToScreenProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_texture_unit_location;
} FrameBufferToScreenProgramStruct;

class FrameBufferToScreenProgram
{
public:
    static FrameBufferToScreenProgramStruct build(GLuint program);
    
private:
    FrameBufferToScreenProgram();
};

#endif /* defined(__nosfuratu__FrameBufferToScreenProgram__) */
