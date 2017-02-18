//
//  Direct3DFramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DFramebufferRadialBlurGpuProgramWrapper__
#define __noctisgames__Direct3DFramebufferRadialBlurGpuProgramWrapper__

#include "FramebufferRadialBlurGpuProgramWrapper.h"

#include "pch.h"

class Direct3DProgram;

class Direct3DFramebufferRadialBlurGpuProgramWrapper : public FramebufferRadialBlurGpuProgramWrapper
{
public:
    Direct3DFramebufferRadialBlurGpuProgramWrapper();
    
    virtual ~Direct3DFramebufferRadialBlurGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    Direct3DProgram* m_program;
    
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_isWindowsMobileConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;
    
    bool m_isWindowsMobile;
    
    void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DFramebufferRadialBlurGpuProgramWrapper__) */
