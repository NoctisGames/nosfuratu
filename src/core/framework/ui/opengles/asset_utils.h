//
//  asset_utils.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef Asset_Utils_H
#define Asset_Utils_H

#include "platform_gl.h"

GLuint load_png_asset_into_texture(const char* relative_path, int repeat_s);

GLuint build_program_from_assets(const char* vertex_shader_path, const char* fragment_shader_path);

#endif /* Asset_Utils_H */