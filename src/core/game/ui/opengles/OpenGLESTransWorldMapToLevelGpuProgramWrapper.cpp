//
//  OpenGLESTransWorldMapToLevelGpuProgramWrapper.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESTransWorldMapToLevelGpuProgramWrapper.h"
#include "OpenGLESManager.h"
#include "macros.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

extern "C"
{
#include "asset_utils.h"
}

OpenGLESTransWorldMapToLevelGpuProgramWrapper::OpenGLESTransWorldMapToLevelGpuProgramWrapper() : TransitionGpuProgramWrapper()
{
    /* initialize random seed: */
    srand (time(NULL));
    
    /* generate secret number between 1 and 10: */
    int random = rand() % 2;
    
    m_program = TransitionProgram::build(build_program_from_assets("frame_buffer_to_screen_shader.vsh", random == 1 ? "trans_world_map_to_level_shader.fsh" : "trans_world_map_to_level_shader_alt.fsh"));
    m_isLoaded = true;
}

void OpenGLESTransWorldMapToLevelGpuProgramWrapper::bind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_to->texture);
    
    glUseProgram(m_program.program);
    
    glUniform1i(m_program.u_from_location, 0);
    glUniform1i(m_program.u_to_location, 1);
    glUniform1f(m_program.u_progress_location, m_fProgress);
    
    glGenBuffers(1, &OGLESManager->sb_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->sb_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * OGLESManager->m_textureVertices.size(), &OGLESManager->m_textureVertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(m_program.a_position_location, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 9, BUFFER_OFFSET(0));
    
    glEnableVertexAttribArray(m_program.a_position_location);
}

void OpenGLESTransWorldMapToLevelGpuProgramWrapper::unbind()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &OGLESManager->sb_vbo_object);
    
    glUseProgram(0);
}

void OpenGLESTransWorldMapToLevelGpuProgramWrapper::cleanUp()
{
    glDeleteProgram(m_program.program);
}