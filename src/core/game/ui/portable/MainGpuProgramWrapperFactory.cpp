//
//  MainGpuProgramWrapperFactory.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "MainGpuProgramWrapperFactory.h"

MainGpuProgramWrapperFactory* MainGpuProgramWrapperFactory::getInstance()
{
    static MainGpuProgramWrapperFactory *instance = new MainGpuProgramWrapperFactory();
    return instance;
}

#if defined __APPLE__ || defined __ANDROID__

#include "OpenGLTransScreenGpuProgramWrapper.h"
#include "OpenGLFadeScreenGpuProgramWrapper.h"
#include "OpenGLPointTransScreenGpuProgramWrapper.h"
#include "OpenGLTextureGpuProgramWrapper.h"
#include "OpenGLSnakeDeathTextureGpuProgramWrapper.h"
#include "OpenGLEndBossSnakeTextureGpuProgramWrapper.h"
#include "OpenGLShockwaveTextureGpuProgramWrapper.h"
#include "OpenGLTransDeathGpuProgramWrapper.h"
#include "OpenGLFramebufferTintGpuProgramWrapper.h"
#include "OpenGLFramebufferObfuscationGpuProgramWrapper.h"
#include "OpenGLFramebufferRadialBlurGpuProgramWrapper.h"

TransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransScreenGpuProgramWrapper()
{
    return new OpenGLTransScreenGpuProgramWrapper();
}

TransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createFadeScreenGpuProgramWrapper()
{
    return new OpenGLFadeScreenGpuProgramWrapper();
}

PointTransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createPointTransScreenGpuProgramWrapper()
{
    return new OpenGLPointTransScreenGpuProgramWrapper();
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createBackgroundGpuTextureProgramWrapper()
{
    return new OpenGLTextureGpuProgramWrapper();
}

SnakeDeathTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createSnakeDeathTextureProgram()
{
    return new OpenGLSnakeDeathTextureGpuProgramWrapper();
}

EndBossSnakeTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createEndBossSnakeTextureProgram()
{
    return new OpenGLEndBossSnakeTextureGpuProgramWrapper();
}

ShockwaveTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createShockwaveTextureGpuProgramWrapper()
{
    return new OpenGLShockwaveTextureGpuProgramWrapper();
}

TransDeathGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransDeathInGpuProgramWrapper()
{
    return new OpenGLTransDeathGpuProgramWrapper(true);
}

TransDeathGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransDeathOutGpuProgramWrapper()
{
    return new OpenGLTransDeathGpuProgramWrapper(false);
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferTintGpuProgramWrapper()
{
    return new OpenGLFramebufferTintGpuProgramWrapper();
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferObfuscationGpuProgramWrapper()
{
    return new OpenGLFramebufferObfuscationGpuProgramWrapper();
}

FramebufferRadialBlurGpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferRadialBlurGpuProgramWrapper()
{
    return new OpenGLFramebufferRadialBlurGpuProgramWrapper();
}
#elif defined _WIN32

#include "Direct3DTransScreenGpuProgramWrapper.h"
#include "Direct3DFadeScreenGpuProgramWrapper.h"
#include "Direct3DPointTransScreenGpuProgramWrapper.h"
#include "Direct3DBackgroundTextureGpuProgramWrapper.h"
#include "Direct3DSnakeDeathTextureGpuProgramWrapper.h"
#include "Direct3DEndBossSnakeTextureGpuProgramWrapper.h"
#include "Direct3DShockwaveTextureGpuProgramWrapper.h"
#include "Direct3DTransDeathGpuProgramWrapper.h"
#include "Direct3DFramebufferTintGpuProgramWrapper.h"
#include "Direct3DFrameBufferObfuscationGpuProgramWrapper.h"
#include "Direct3DFramebufferRadialBlurGpuProgramWrapper.h"

TransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransScreenGpuProgramWrapper()
{
    return new Direct3DTransScreenGpuProgramWrapper();
}

TransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createFadeScreenGpuProgramWrapper()
{
    return new Direct3DFadeScreenGpuProgramWrapper();
}

PointTransitionGpuProgramWrapper* MainGpuProgramWrapperFactory::createPointTransScreenGpuProgramWrapper()
{
    return new Direct3DPointTransScreenGpuProgramWrapper();
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createBackgroundGpuTextureProgramWrapper()
{
    return new Direct3DBackgroundTextureGpuProgramWrapper();
}

SnakeDeathTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createSnakeDeathTextureProgram()
{
    return new Direct3DSnakeDeathTextureGpuProgramWrapper();
}

EndBossSnakeTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createEndBossSnakeTextureProgram()
{
    return new Direct3DEndBossSnakeTextureGpuProgramWrapper();
}

ShockwaveTextureGpuProgramWrapper* MainGpuProgramWrapperFactory::createShockwaveTextureGpuProgramWrapper()
{
    return new Direct3DShockwaveTextureGpuProgramWrapper();
}

TransDeathGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransDeathInGpuProgramWrapper()
{
    return new Direct3DTransDeathGpuProgramWrapper(true);
}

TransDeathGpuProgramWrapper* MainGpuProgramWrapperFactory::createTransDeathOutGpuProgramWrapper()
{
    return new Direct3DTransDeathGpuProgramWrapper(false);
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferTintGpuProgramWrapper()
{
    return new Direct3DFramebufferTintGpuProgramWrapper();
}

GpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferObfuscationGpuProgramWrapper()
{
    return new Direct3DFrameBufferObfuscationGpuProgramWrapper();
}

FramebufferRadialBlurGpuProgramWrapper* MainGpuProgramWrapperFactory::createFramebufferRadialBlurGpuProgramWrapper()
{
    return new Direct3DFramebufferRadialBlurGpuProgramWrapper();
}
#endif
