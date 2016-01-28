//
//  OpenGLESManager.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 11/19/14.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "OpenGLESManager.h"
#include "OpenGLESTextureGpuProgramWrapper.h"
#include "OpenGLESGeometryGpuProgramWrapper.h"
#include "OpenGLESFrameBufferToScreenGpuProgramWrapper.h"
#include "ColorProgram.h"
#include "TextureProgram.h"
#include "FrameBufferToScreenProgram.h"
#include "macros.h"

extern "C"
{
#include <assert.h>
}

OpenGLESManager * OpenGLESManager::getInstance()
{
    static OpenGLESManager *openGLESManager = new OpenGLESManager();
    return openGLESManager;
}

void OpenGLESManager::init(int width, int height, int maxBatchSize, int numFramebuffers)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_screenFBO);
    
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
    
    buildShaderPrograms();
    generateIndices(maxBatchSize);
    
    for (int i = 0; i < numFramebuffers; i++)
    {
        createFrameBufferObject(width, height);
    }
}

void OpenGLESManager::createMatrix(float left, float right, float bottom, float top)
{
    vec3 eye = { 0, 0, 1 };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    
    mat4x4 projectionMatrix;
    mat4x4 viewMatrix;
    
    mat4x4_ortho(projectionMatrix, left, right, bottom, top, -1, 1);
    mat4x4_look_at(viewMatrix, eye, center, up);
    
    mat4x4_mul(m_viewProjectionMatrix, projectionMatrix, viewMatrix);
}

void OpenGLESManager::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v)
{
    m_textureVertices.push_back(x);
    m_textureVertices.push_back(y);
    m_textureVertices.push_back(z);
    m_textureVertices.push_back(r);
    m_textureVertices.push_back(g);
    m_textureVertices.push_back(b);
    m_textureVertices.push_back(a);
    m_textureVertices.push_back(u);
    m_textureVertices.push_back(v);
}

void OpenGLESManager::addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    m_colorVertices.push_back(x);
    m_colorVertices.push_back(y);
    m_colorVertices.push_back(z);
    m_colorVertices.push_back(r);
    m_colorVertices.push_back(g);
    m_colorVertices.push_back(b);
    m_colorVertices.push_back(a);
}

#pragma mark <Private>

void OpenGLESManager::buildShaderPrograms()
{
    m_textureProgram = std::unique_ptr<OpenGLESTextureGpuProgramWrapper>(new OpenGLESTextureGpuProgramWrapper());
    m_colorProgram = std::unique_ptr<OpenGLESGeometryGpuProgramWrapper>(new OpenGLESGeometryGpuProgramWrapper());
    m_fbToScreenProgram = std::unique_ptr<OpenGLESFrameBufferToScreenGpuProgramWrapper>(new OpenGLESFrameBufferToScreenGpuProgramWrapper());
}

void OpenGLESManager::generateIndices(int maxBatchSize)
{
    m_indices.reserve(maxBatchSize * INDICES_PER_RECTANGLE);
    
    GLshort j = 0;
    for (int i = 0; i < maxBatchSize * INDICES_PER_RECTANGLE; i += INDICES_PER_RECTANGLE, j += VERTICES_PER_RECTANGLE)
    {
        m_indices.push_back(j);
        m_indices.push_back(j + 1);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 2);
        m_indices.push_back(j + 3);
        m_indices.push_back(j + 0);
    }
}

void OpenGLESManager::createFrameBufferObject(int width, int height)
{
    GLuint fbo_texture;
    GLuint fbo;
    
    // Texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &fbo_texture);
    glBindTexture(GL_TEXTURE_2D, fbo_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_screenFBO);
    
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    
    m_fbo_textures.push_back(fbo_texture);
    m_fbos.push_back(fbo);
}

OpenGLESManager::OpenGLESManager()
{
    // Hide Constructor for Singleton
}

OpenGLESManager::~OpenGLESManager()
{
    for (std::vector<GLuint>::iterator i = m_fbo_textures.begin(); i != m_fbo_textures.end(); )
    {
        glDeleteTextures(1, &(*i));
    }
    
    for (std::vector<GLuint>::iterator i = m_fbos.begin(); i != m_fbos.end(); )
    {
        glDeleteFramebuffers(1, &(*i));
    }
    
    m_textureProgram->cleanUp();
    m_colorProgram->cleanUp();
    m_fbToScreenProgram->cleanUp();
}
