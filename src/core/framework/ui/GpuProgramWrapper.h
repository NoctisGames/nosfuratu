//
//  GpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/27/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__GpuProgramWrapper__
#define __gowengamedev__GpuProgramWrapper__

class GpuProgramWrapper
{
public:
    GpuProgramWrapper();
    
    virtual ~GpuProgramWrapper();
    
    virtual void bind() = 0;
    
    virtual void unbind() = 0;

	virtual void cleanUp();

	bool isLoaded();

protected:
	bool m_isLoaded;
};

#endif /* defined(__gowengamedev__GpuProgramWrapper__) */
