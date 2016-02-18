//
//  DummyGpuProgramWrapper.cpp
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/27/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "DummyGpuProgramWrapper.h"

DummyGpuProgramWrapper * DummyGpuProgramWrapper::getInstance()
{
    static DummyGpuProgramWrapper *instance = new DummyGpuProgramWrapper();
    return instance;
}

void DummyGpuProgramWrapper::bind()
{
    // Empty
}

void DummyGpuProgramWrapper::unbind()
{
    // Empty
}

void DummyGpuProgramWrapper::cleanUp()
{
	// Empty
}

DummyGpuProgramWrapper::DummyGpuProgramWrapper()
{
    // Hide Constructor for Singleton
}