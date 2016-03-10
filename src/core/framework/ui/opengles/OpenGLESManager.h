//
//  OpenGLESManager.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 11/19/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESManager__
#define __noctisgames__OpenGLESManager__

#define VERTICES_PER_LINE 2
#define VERTICES_PER_RECTANGLE 4
#define INDICES_PER_RECTANGLE 6

#define OGLESManager (OpenGLESManager::getInstance())

extern "C"
{
#include "platform_gl.h"
#include "linmath.h"
}

#include <memory>
#include <vector>

class GpuProgramWrapper;

class OpenGLESManager
{
public:
    static OpenGLESManager * getInstance();
    
    std::vector<GLfloat> m_textureVertices;
    std::vector<GLfloat> m_colorVertices;
    
    std::vector<GLshort> m_indices;
    
    std::unique_ptr<GpuProgramWrapper> m_textureProgram;
    std::unique_ptr<GpuProgramWrapper> m_colorProgram;
	std::unique_ptr<GpuProgramWrapper> m_fbToScreenProgram;
    
    GLuint sb_vbo_object; // For Sprite Batcher
    GLuint gb_vbo_object; // For Geometry Batcher
    
    std::vector<GLuint> m_fbos;
    std::vector<GLuint> m_fbo_textures;
    
    GLint m_iScreenFBO;
    GLint m_iMaxTextureSize;
    
    mat4x4 m_viewProjectionMatrix;
    
    void init(int width, int height, int maxBatchSize, int numFramebuffers = 1);
    
    void cleanUp();
    
    void createMatrix(float left, float right, float bottom, float top);
    
    // Called by Batchers
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat u, GLfloat v);
    void addVertexCoordinate(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    
    bool isLoaded();
    
private:
    void buildShaderPrograms();
    void generateIndices(int maxBatchSize);
    void createFramebufferObject(int width, int height);
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    OpenGLESManager();
    OpenGLESManager(const OpenGLESManager&);
    OpenGLESManager& operator=(const OpenGLESManager&);
    ~OpenGLESManager();
};

#endif /* defined(__noctisgames__OpenGLESManager__) */
