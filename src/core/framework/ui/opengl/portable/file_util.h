//
//  file_util.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__file_util__
#define __noctisgames__file_util__

typedef struct {
    const long data_length;
    const void *data;
    const void *file_handle;
} FileData;

FileData get_file_data(const char* path);

void release_file_data(const FileData* file_data);

#endif /* defined(__noctisgames__file_util__) */
