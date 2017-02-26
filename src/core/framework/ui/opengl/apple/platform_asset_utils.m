//
//  platform_asset_utils.m
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "platform_asset_utils.h"
#include "platform_file_utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if TARGET_OS_IPHONE
// Empty
#elif TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#endif

FileData get_asset_data(const char *relative_path)
{
	assert(relative_path != NULL);
    
    NSString *path = [[NSString alloc] initWithCString:relative_path encoding:NSASCIIStringEncoding];
    const char *bundlePath = [[[NSBundle mainBundle] pathForResource:path ofType:nil] fileSystemRepresentation];
    
    return get_file_data(bundlePath);
}

void release_asset_data(const FileData *file_data)
{
    assert(file_data != NULL);
	release_file_data(file_data);
}
