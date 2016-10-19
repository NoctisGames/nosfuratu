//
//  OpenGLRenderer.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "OpenGLRenderer.h"
#include "OpenGLSpriteBatcher.h"
#include "OpenGLRectangleBatcher.h"
#include "OpenGLLineBatcher.h"
#include "GpuTextureWrapper.h"
#include "GpuTextureDataWrapper.h"
#include "OpenGLManager.h"
#include "GameConstants.h"
#include "Assets.h"
#include "PhysicalEntity.h"
#include "TextureRegion.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Game.h"
#include "Jon.h"
#include "OpenGLTransScreenGpuProgramWrapper.h"
#include "OpenGLFadeScreenGpuProgramWrapper.h"
#include "OpenGLPointTransScreenGpuProgramWrapper.h"
#include "OpenGLSinWaveTextureGpuProgramWrapper.h"
#include "OpenGLSnakeDeathTextureGpuProgramWrapper.h"
#include "OpenGLEndBossSnakeTextureGpuProgramWrapper.h"
#include "OpenGLShockwaveTextureGpuProgramWrapper.h"
#include "OpenGLFramebufferTintGpuProgramWrapper.h"
#include "OpenGLFramebufferObfuscationGpuProgramWrapper.h"
#include "OpenGLTransDeathGpuProgramWrapper.h"
#include "OpenGLFramebufferRadialBlurGpuProgramWrapper.h"

extern "C"
{
#include "asset_utils.h"
#include "platform_gl.h"
}

OpenGLRenderer::OpenGLRenderer() : Renderer()
{
    m_spriteBatcher = std::unique_ptr<OpenGLSpriteBatcher>(new OpenGLSpriteBatcher());
    m_boundsRectangleBatcher = std::unique_ptr<OpenGLRectangleBatcher>(new OpenGLRectangleBatcher());
    m_highlightRectangleBatcher = std::unique_ptr<OpenGLRectangleBatcher>(new OpenGLRectangleBatcher(true));
    m_lineBatcher = std::unique_ptr<OpenGLLineBatcher>(new OpenGLLineBatcher());
}

void OpenGLRenderer::beginFrame(float deltaTime)
{
    glEnable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Renderer::beginFrame(deltaTime);
}

void OpenGLRenderer::endFrame()
{
    glDisable(GL_BLEND);
    
    glDisable(GL_TEXTURE_2D);
}

void OpenGLRenderer::loadShaderPrograms()
{
    m_backgroundGpuTextureProgramWrapper = OGLESManager->m_textureProgram.get();
    m_transScreenGpuProgramWrapper = new OpenGLTransScreenGpuProgramWrapper();
    m_fadeScreenGpuProgramWrapper = new OpenGLFadeScreenGpuProgramWrapper();
    m_pointTransScreenGpuProgramWrapper = new OpenGLPointTransScreenGpuProgramWrapper();
    m_sinWaveTextureProgram = new OpenGLSinWaveTextureGpuProgramWrapper();
    m_snakeDeathTextureProgram = new OpenGLSnakeDeathTextureGpuProgramWrapper();
    m_endBossSnakeTextureProgram = new OpenGLEndBossSnakeTextureGpuProgramWrapper();
    m_shockwaveTextureGpuProgramWrapper = new OpenGLShockwaveTextureGpuProgramWrapper();
    m_framebufferToScreenGpuProgramWrapper = OGLESManager->m_fbToScreenProgram.get();
    m_framebufferTintGpuProgramWrapper = new OpenGLFramebufferTintGpuProgramWrapper();
    m_framebufferObfuscationGpuProgramWrapper = new OpenGLFramebufferObfuscationGpuProgramWrapper();
    m_transDeathInGpuProgramWrapper = new OpenGLTransDeathGpuProgramWrapper(true);
    m_transDeathOutGpuProgramWrapper = new OpenGLTransDeathGpuProgramWrapper(false);
    m_framebufferRadialBlurGpuProgramWrapper = new OpenGLFramebufferRadialBlurGpuProgramWrapper();
}

bool OpenGLRenderer::isLoaded()
{
	return Renderer::isLoaded() && OGLESManager->isLoaded();
}

void OpenGLRenderer::addFramebuffers()
{
    for (std::vector<GLuint>::iterator i = OGLESManager->m_fbo_textures.begin(); i != OGLESManager->m_fbo_textures.end(); i++)
    {
        m_framebuffers.push_back(GpuTextureWrapper((*i)));
    }
}

GpuTextureDataWrapper* OpenGLRenderer::loadTextureData(const char* textureName)
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

GpuTextureWrapper* OpenGLRenderer::loadTexture(GpuTextureDataWrapper* textureData, int repeatS)
{
    return new GpuTextureWrapper(load_png_asset_into_texture(textureData->raw_image_data, repeatS));
}

void OpenGLRenderer::updateMatrix(float left, float right, float bottom, float top)
{
    OGLESManager->createMatrix(left, right, bottom, top);
}

void OpenGLRenderer::bindToOffscreenFramebuffer(int index)
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_fbos.at(index));
}

void OpenGLRenderer::clearFramebufferWithColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::bindToScreenFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, OGLESManager->m_iScreenFBO);
}

void OpenGLRenderer::destroyTexture(GpuTextureWrapper& textureWrapper)
{
    glDeleteTextures(1, &textureWrapper.texture);
}
