//
//  Direct3DFramebufferRadialBlurGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DFramebufferRadialBlurGpuProgramWrapper__
#define __nosfuratu__Direct3DFramebufferRadialBlurGpuProgramWrapper__

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
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_directionConstantBuffer;
    
    bool m_isWindowsMobile;
};

#endif /* defined(__nosfuratu__Direct3DFramebufferRadialBlurGpuProgramWrapper__) */
