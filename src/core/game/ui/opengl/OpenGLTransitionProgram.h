//
//  OpenGLTransitionProgram.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLTransitionProgram__
#define __noctisgames__OpenGLTransitionProgram__

#include "OpenGLProgram.h"

class OpenGLTransitionProgram : public OpenGLProgram
{
public:
    GLint u_from_location;
    GLint u_to_location;
    GLint u_progress_location;
    GLint a_position_location;
    
    OpenGLTransitionProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
    
    virtual void bind();
    
    virtual void unbind();
};

#endif /* defined(__noctisgames__OpenGLTransitionProgram__) */
