//
//  PointTransitionProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__PointTransitionProgram__
#define __noctisgames__PointTransitionProgram__

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
} PointTransitionProgramStruct;

class PointTransitionProgram
{
public:
    static PointTransitionProgramStruct build(GLuint program);
    
private:
    PointTransitionProgram();
};

#endif /* defined(__noctisgames__PointTransitionProgram__) */
