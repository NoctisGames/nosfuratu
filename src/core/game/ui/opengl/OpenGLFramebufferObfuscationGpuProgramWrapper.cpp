//
//  OpenGLFramebufferObfuscationGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLFramebufferObfuscationGpuProgramWrapper.h"

#include "OpenGLManager.h"
#include "macros.h"
#include "GpuTextureWrapper.h"
#include "AssetUtil.h"

OpenGLFramebufferObfuscationGpuProgramWrapper::OpenGLFramebufferObfuscationGpuProgramWrapper()
{
    m_program = OpenGLFramebufferToScreenProgram::build(AssetUtil::buildProgramFromAssets("frame_buffer_to_screen_shader.vsh", "pp_frame_buffer_obfuscation.fsh"));
}

OpenGLFramebufferObfuscationGpuProgramWrapper::~OpenGLFramebufferObfuscationGpuProgramWrapper()
{
    glDeleteProgram(m_program.program);
}

void OpenGLFramebufferObfuscationGpuProgramWrapper::bind()
{
    OGLManager->useNormalBlending();
    
    glUseProgram(m_program.program);
    
    glUniform1i(m_program.u_texture_unit_location, 0);
    
    glGenBuffers(1, &OGLManager->getSbVboObject());
    glBindBuffer(GL_ARRAY_BUFFER, OGLManager->getSbVboObject());
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLManager->getTextureVertices().size(), &OGLManager->getTextureVertices()[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(m_program.a_position_location);
}

void OpenGLFramebufferObfuscationGpuProgramWrapper::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLManager->getSbVboObject());
    
    glUseProgram(0);
}
