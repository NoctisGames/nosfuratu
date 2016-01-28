//
//  TransTitleToWorldProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__TransTitleToWorldProgram__
#define __gowengamedev__TransTitleToWorldProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_from_location;
    GLint u_to_location;
    GLint u_progress_location;
} TransTitleToWorldProgramStruct;

class TransTitleToWorldProgram
{
public:
    static TransTitleToWorldProgramStruct build(GLuint program);
    
private:
    TransTitleToWorldProgram();
};

#endif /* defined(__gowengamedev__TransTitleToWorldProgram__) */
