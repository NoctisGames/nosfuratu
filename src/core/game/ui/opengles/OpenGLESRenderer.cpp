//
//  OpenGLESRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLESRenderer.h"
#include "OpenGLESSpriteBatcher.h"
#include "OpenGLESRectangleBatcher.h"
#include "OpenGLESLineBatcher.h"
#include "GpuTextureWrapper.h"
#include "GpuTextureDataWrapper.h"
#include "OpenGLESManager.h"
#include "GameConstants.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "TextureRegion.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Game.h"
#include "Jon.h"
#include "OpenGLESTransScreenGpuProgramWrapper.h"
#include "OpenGLESPointTransScreenGpuProgramWrapper.h"
#include "OpenGLESSinWaveTextureGpuProgramWrapper.h"
#include "OpenGLESSnakeDeathTextureGpuProgramWrapper.h"
#include "OpenGLESEndBossSnakeTextureGpuProgramWrapper.h"
#include "OpenGLESShockwaveTextureGpuProgramWrapper.h"
#include "OpenGLESFramebufferTintGpuProgramWrapper.h"
#include "OpenGLESFramebufferObfuscationGpuProgramWrapper.h"
#include "OpenGLESTransDeathGpuProgramWrapper.h"
#include "OpenGLESFramebufferRadialBlurGpuProgramWrapper.h"

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLESRenderer::OpenGLESRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLESSpriteBatcher>(new OpenGLESSpriteBatcher());
    m_boundsRectangleBatcher = std::unique_ptr<OpenGLESRectangleBatcher>(new OpenGLESRectangleBatcher());
    m_highlightRectangleBatcher = std::unique_ptr<OpenGLESRectangleBatcher>(new OpenGLESRectangleBatcher(true));
    m_lineBatcher = std::unique_ptr<OpenGLESLineBatcher>(new OpenGLESLineBatcher());
}

void OpenGLESRenderer::beginFrame(float deltaTime)
{
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer::beginFrame(deltaTime);
}

void OpenGLESRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLESRenderer::loadShaderPrograms()
{
    m_backgroundGpuTextureProgramWrapper = OGLESManager->m_textureProgram.get();
    m_transScreenGpuProgramWrapper = new OpenGLESTransScreenGpuProgramWrapper();
    m_pointTransScreenGpuProgramWrapper = new OpenGLESPointTransScreenGpuProgramWrapper();
    m_sinWaveTextureProgram = new OpenGLESSinWaveTextureGpuProgramWrapper();
    m_snakeDeathTextureProgram = new OpenGLESSnakeDeathTextureGpuProgramWrapper();
    m_endBossSnakeTextureProgram = new OpenGLESEndBossSnakeTextureGpuProgramWrapper();
    m_shockwaveTextureGpuProgramWrapper = new OpenGLESShockwaveTextureGpuProgramWrapper();
    m_framebufferToScreenGpuProgramWrapper = OGLESManager->m_fbToScreenProgram.get();
    m_framebufferTintGpuProgramWrapper = new OpenGLESFramebufferTintGpuProgramWrapper();
    m_framebufferObfuscationGpuProgramWrapper = new OpenGLESFramebufferObfuscationGpuProgramWrapper();
    m_transDeathInGpuProgramWrapper = new OpenGLESTransDeathGpuProgramWrapper(true);
    m_transDeathOutGpuProgramWrapper = new OpenGLESTransDeathGpuProgramWrapper(false);
    m_framebufferRadialBlurGpuProgramWrapper = new OpenGLESFramebufferRadialBlurGpuProgramWrapper();
}

bool OpenGLESRenderer::isLoaded()
{
	return Renderer::isLoaded() && OGLESManager->isLoaded();
}

void OpenGLESRenderer::addFramebuffers()
{
    for (std::vector<GLuint>::iterator i = OGLESManager->m_fbo_textures.begin(); i != OGLESManager->m_fbo_textures.end(); i++)
    {
        m_framebuffers.push_back(GpuTextureWrapper((*i)));
    }
}

GpuTextureDataWrapper* OpenGLESRenderer::loadTextureData(const char* textureName)
{
    size_t len = strlen(textureName);
    
    char* textureFileName = new char[len + 5];
    
    strcpy(textureFileName, textureName);
    textureFileName[len] = '.';
    textureFileName[len+1] = 'p';
    textureFileName[len+2] = 'n';
    textureFileName[len+3] = 'g';
    textureFileName[len+4] = '\0';
    
    GpuTextureDataWrapper* tdw = new GpuTextureDataWrapper(load_png_asset(textureFileName));
    
    delete[] textureFileName;
    
    return tdw;
}

GpuTextureWrapper* OpenGLESRenderer::loadTexture(GpuTextureDataWrapper* textureData, int repeatS)
{
    return new GpuTextureWrapper(load_png_asset_into_texture(textureData->raw_image_data, repeatS));
}

void OpenGLESRenderer::updateMatrix(float left, float right, float bottom, float top)
{
    OGLESManager->createMatrix(left, right, bottom, top);
}

void OpenGLESRenderer::bindToOffscreenFramebuffer(int index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_fbos.at(index));
}

void OpenGLESRenderer::clearFramebufferWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLESRenderer::bindToScreenFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_iScreenFBO);
}

void OpenGLESRenderer::destroyTexture(GpuTextureWrapper& textureWrapper)
{
    glDeleteTextures(1, &textureWrapper.texture);
}
