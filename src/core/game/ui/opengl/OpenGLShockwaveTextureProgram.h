//
//  OpenGLShockwaveTextureProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLShockwaveTextureProgram__
#define __nosfuratu__OpenGLShockwaveTextureProgram__

#include "OpenGLProgram.h"

class OpenGLShockwaveTextureProgram : public OpenGLProgram
{
public:
    GLint u_mvp_matrix_location;
    GLint u_texture_unit_location;
    GLint u_center_x_unit_location;
    GLint u_center_y_unit_location;
    GLint u_time_elapsed_unit_location;
    GLint u_is_transforming;
    GLint a_position_location;
    GLint a_color_location;
    GLint a_texture_coordinates_location;
    
    OpenGLShockwaveTextureProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
    
    virtual void bind();
    
    virtual void unbind();
};

#endif /* defined(__nosfuratu__OpenGLShockwaveTextureProgram__) */
