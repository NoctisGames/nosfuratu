//
//  GpuProgramWrapper.cpp
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/27/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#include "GpuProgramWrapper.h"

GpuProgramWrapper::GpuProgramWrapper() : m_isLoaded(false)
{
    // Empty
}

GpuProgramWrapper::~GpuProgramWrapper()
{
    cleanUp();
}

void GpuProgramWrapper::cleanUp()
{
	// Empty
}

bool GpuProgramWrapper::isLoaded()
{
	return m_isLoaded;
}