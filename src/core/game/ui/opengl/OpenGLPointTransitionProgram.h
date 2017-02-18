//
//  OpenGLPointTransitionProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLPointTransitionProgram__
#define __nosfuratu__OpenGLPointTransitionProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_mvp_matrix_location;
    GLint u_center_x_unit_location;
    GLint u_center_y_unit_location;
    GLint u_from_location;
    GLint u_to_location;
    GLint u_progress_location;
} OpenGLPointTransitionProgramStruct;

class OpenGLPointTransitionProgram
{
public:
    static OpenGLPointTransitionProgramStruct build(GLuint program);
    
private:
    OpenGLPointTransitionProgram();
};

#endif /* defined(__nosfuratu__OpenGLPointTransitionProgram__) */
