//
//  OpenGLShockwaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLShockwaveTextureGpuProgramWrapper.h"

#include "OpenGLManager.h"
#include "macros.h"

extern "C"
{
#include "asset_utils.h"
}

OpenGLShockwaveTextureGpuProgramWrapper::OpenGLShockwaveTextureGpuProgramWrapper()
{
    m_program = OpenGLShockwaveTextureProgram::build(build_program_from_assets("pp_shockwave_texture_shader.vsh", "pp_shockwave_texture_shader.fsh"));
}

OpenGLShockwaveTextureGpuProgramWrapper::~OpenGLShockwaveTextureGpuProgramWrapper()
{
    glDeleteProgram(m_program.program);
}

void OpenGLShockwaveTextureGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    glUseProgram(m_program.program);
    
    glUniformMatrix4fv(m_program.u_mvp_matrix_location, 1, GL_FALSE, (GLfloat*)OGLManager->getViewProjectionMatrix());
    glUniform1i(m_program.u_texture_unit_location, 0);
    glUniform1f(m_program.u_center_x_unit_location, m_center.getX());
    glUniform1f(m_program.u_center_y_unit_location, m_center.getY());
    glUniform1f(m_program.u_time_elapsed_unit_location, m_fTimeElapsed);
    glUniform1i(m_program.u_is_transforming, m_isTransforming ? 1 : 0);
    
    glGenBuffers(1, &OGLManager->getSbVboObject());
    glBindBuffer(GL_ARRAY_BUFFER, OGLManager->getSbVboObject());
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLManager->getTextureVertices().size(), &OGLManager->getTextureVertices()[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    glVertexAttribPointer(m_program.a_color_location, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(3 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(m_program.a_texture_coordinates_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(7 * sizeof(GL_FLOAT)));
    
    glEnableVertexAttribArray(m_program.a_position_location);
    glEnableVertexAttribArray(m_program.a_color_location);
    glEnableVertexAttribArray(m_program.a_texture_coordinates_location);
}

void OpenGLShockwaveTextureGpuProgramWrapper::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLManager->getSbVboObject());
    
    glUseProgram(0);
}
