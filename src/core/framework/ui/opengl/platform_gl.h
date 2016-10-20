//
//  platform_gl.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__platform_gl_h
#define __noctisgames__platform_gl_h

#ifdef NG_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined NG_MAC
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#elif defined NG_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#endif /* defined(__noctisgames__platform_gl_h) */
