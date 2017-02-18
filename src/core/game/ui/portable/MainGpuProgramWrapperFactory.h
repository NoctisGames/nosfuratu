//
//  MainGpuProgramWrapperFactory.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__MainGpuProgramWrapperFactory__
#define __noctisgames__MainGpuProgramWrapperFactory__

class TransitionGpuProgramWrapper;
class PointTransitionGpuProgramWrapper;
class SinWaveTextureGpuProgramWrapper;
class GpuProgramWrapper;
class SnakeDeathTextureGpuProgramWrapper;
class EndBossSnakeTextureGpuProgramWrapper;
class ShockwaveTextureGpuProgramWrapper;
class TransDeathGpuProgramWrapper;
class FramebufferRadialBlurGpuProgramWrapper;

#define MAIN_GPU_PROGRAM_WRAPPER_FACTORY (MainGpuProgramWrapperFactory::getInstance())

class MainGpuProgramWrapperFactory
{
public:
	static MainGpuProgramWrapperFactory* getInstance();
    
    TransitionGpuProgramWrapper* createTransScreenGpuProgramWrapper();
    
    TransitionGpuProgramWrapper* createFadeScreenGpuProgramWrapper();
    
    PointTransitionGpuProgramWrapper* createPointTransScreenGpuProgramWrapper();
    
    SinWaveTextureGpuProgramWrapper* createSinWaveTextureProgram();
    
    GpuProgramWrapper* createBackgroundGpuTextureProgramWrapper();
    
    SnakeDeathTextureGpuProgramWrapper* createSnakeDeathTextureProgram();
    
    EndBossSnakeTextureGpuProgramWrapper* createEndBossSnakeTextureProgram();
    
    ShockwaveTextureGpuProgramWrapper* createShockwaveTextureGpuProgramWrapper();
    
    TransDeathGpuProgramWrapper* createTransDeathInGpuProgramWrapper();
    
    TransDeathGpuProgramWrapper* createTransDeathOutGpuProgramWrapper();
    
    GpuProgramWrapper* createFramebufferToScreenGpuProgramWrapper();
    
    GpuProgramWrapper* createFramebufferTintGpuProgramWrapper();
    
    GpuProgramWrapper* createFramebufferObfuscationGpuProgramWrapper();
    
    FramebufferRadialBlurGpuProgramWrapper* createFramebufferRadialBlurGpuProgramWrapper();

private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    MainGpuProgramWrapperFactory() {}
    MainGpuProgramWrapperFactory(const MainGpuProgramWrapperFactory&);
    MainGpuProgramWrapperFactory& operator=(const MainGpuProgramWrapperFactory&);
};

#endif /* defined(__noctisgames__MainGpuProgramWrapperFactory__) */
