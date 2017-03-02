//
//  asset_data_handler.m
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "asset_data_handler.h"

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
    
    assert(bundlePath != NULL);
    
    FILE* stream = fopen(bundlePath, "r");
    assert (stream != NULL);
    
    fseek(stream, 0, SEEK_END);
    long stream_size = ftell(stream);
    fseek(stream, 0, SEEK_SET);
    
    void* buffer = malloc(stream_size);
    fread(buffer, stream_size, 1, stream);
    
    assert(ferror(stream) == 0);
    fclose(stream);
    
    return (FileData)
    {
        stream_size, buffer, NULL
    };
}

void release_asset_data(const FileData *file_data)
{
    assert(file_data != NULL);
    assert(file_data->data != NULL);
    
    free((void *)file_data->data);
}
