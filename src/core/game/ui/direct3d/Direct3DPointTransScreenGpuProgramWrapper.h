//
//  Direct3DPointTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DPointTransScreenGpuProgramWrapper__
#define __nosfuratu__Direct3DPointTransScreenGpuProgramWrapper__

#include "PointTransitionGpuProgramWrapper.h"

#include "pch.h"

class Direct3DProgram;

class Direct3DPointTransScreenGpuProgramWrapper : public PointTransitionGpuProgramWrapper
{
public:
	Direct3DPointTransScreenGpuProgramWrapper();
    
    virtual ~Direct3DPointTransScreenGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
    Direct3DProgram* m_program;
    
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerXConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerYConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;

	void createConstantBuffers();
};

#endif /* defined(__nosfuratu__Direct3DPointTransScreenGpuProgramWrapper__) */
