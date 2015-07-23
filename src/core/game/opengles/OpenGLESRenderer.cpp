//
//  OpenGLESRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESRenderer.h"
#include "OpenGLESSpriteBatcher.h"
#include "TextureWrapper.h"
#include "OpenGLESManager.h"

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLESRenderer::OpenGLESRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLESSpriteBatcher>(new OpenGLESSpriteBatcher());
    
    m_backgroundTexture = std::unique_ptr<TextureWrapper>(new TextureWrapper(load_png_asset_into_texture("background_texture.png")));
}

void OpenGLESRenderer::clearScreenWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::beginFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->fbo);
    
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLESRenderer::endFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_screenFBO);
    
    static float stateTime = 0;
    
    stateTime += 0.01666666666667f;
    
    static GLfloat fbo_vertices[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    
    glGenBuffers(1, &OGLESManager->fbo_vbo_object);
    glBindBuffer(GL_ARRAY_BUFFER, OGLESManager->fbo_vbo_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
    
    glUseProgram(OGLESManager->m_postProcessingSinWaveProgram.program);
    glBindTexture(GL_TEXTURE_2D, OGLESManager->fbo_texture);
    glUniform1i(OGLESManager->m_postProcessingSinWaveProgram.u_fbo_texture_location, 0);
    glUniform1f(OGLESManager->m_postProcessingSinWaveProgram.u_offset_location, stateTime);
    glVertexAttribPointer(OGLESManager->m_postProcessingSinWaveProgram.a_v_coord_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(OGLESManager->m_postProcessingSinWaveProgram.a_v_coord_location);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::cleanUp()
{
    // TODO
}