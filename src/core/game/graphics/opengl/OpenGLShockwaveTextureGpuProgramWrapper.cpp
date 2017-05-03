//
//  OpenGLShockwaveTextureGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "OpenGLShockwaveTextureGpuProgramWrapper.h"

#include "OpenGLShockwaveTextureProgram.h"
#include "OpenGLManager.h"
#include "Vector2D.h"
#include "FrameworkConstants.h"
#include "GameConstants.h"

OpenGLShockwaveTextureGpuProgramWrapper::OpenGLShockwaveTextureGpuProgramWrapper() : ShockwaveTextureGpuProgramWrapper(), m_program(new OpenGLShockwaveTextureProgram(SHOCKWAVE_VERTEX_SHADER, SHOCKWAVE_FRAGMENT_SHADER))
{
    // Empty
}

OpenGLShockwaveTextureGpuProgramWrapper::~OpenGLShockwaveTextureGpuProgramWrapper()
{
    delete m_program;
}

void OpenGLShockwaveTextureGpuProgramWrapper::bind()
{
    OGLManager->useScreenBlending();
    
    m_program->bind();
    
    glUniform1f(m_program->u_center_x_unit_location, m_center->getX());
    glUniform1f(m_program->u_center_y_unit_location, m_center->getY());
    glUniform1f(m_program->u_time_elapsed_unit_location, m_fTimeElapsed);
    glUniform1i(m_program->u_is_transforming, m_isTransforming ? 1 : 0);
}

void OpenGLShockwaveTextureGpuProgramWrapper::unbind()
{
    m_program->unbind();
}
