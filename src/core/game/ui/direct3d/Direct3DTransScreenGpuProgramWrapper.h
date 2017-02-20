//
//  Direct3DTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DTransScreenGpuProgramWrapper__
#define __nosfuratu__Direct3DTransScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"

#include "pch.h"

class Direct3DProgram;

class Direct3DTransScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
	Direct3DTransScreenGpuProgramWrapper();
    
    virtual ~Direct3DTransScreenGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
    Direct3DProgram* m_program;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_isWindowsMobileConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;
	
    bool m_isWindowsMobile;
};

#endif /* defined(__nosfuratu__Direct3DTransScreenGpuProgramWrapper__) */
