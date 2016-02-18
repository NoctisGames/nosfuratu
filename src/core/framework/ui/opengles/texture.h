//
//  texture.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef Texture_H
#define Texture_H

#include "platform_gl.h"

GLuint load_texture(const GLsizei width, const GLsizei height, const GLenum type, const GLvoid* pixels, int repeat_s, int mipmap);

#endif /* Texture_H */