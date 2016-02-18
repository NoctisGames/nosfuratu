//
//  asset_utils.c
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#include "asset_utils.h"
#include "image.h"
#include "platform_asset_utils.h"
#include "shader.h"
#include "texture.h"
#include <assert.h>
#include <stdlib.h>

GLuint load_png_asset_into_texture(const char* relative_path, int repeat_s)
{
	assert(relative_path != NULL);

	const FileData png_file = get_asset_data(relative_path);
	const RawImageData raw_image_data = get_raw_image_data_from_png(png_file.data, (int)png_file.data_length);
	const GLuint texture_object_id = load_texture(raw_image_data.width, raw_image_data.height, raw_image_data.gl_color_format, raw_image_data.data, repeat_s, 0);

	release_raw_image_data(&raw_image_data);
	release_asset_data(&png_file);

	return texture_object_id;
}

GLuint build_program_from_assets(const char* vertex_shader_path, const char* fragment_shader_path)
{
    assert(vertex_shader_path != NULL);
    assert(fragment_shader_path != NULL);
    
    const FileData vertex_shader_source = get_asset_data(vertex_shader_path);
    const FileData fragment_shader_source = get_asset_data(fragment_shader_path);
    const GLuint program_object_id = build_program(vertex_shader_source.data, (GLint)vertex_shader_source.data_length, fragment_shader_source.data, (GLint)fragment_shader_source.data_length);
    
    release_asset_data(&vertex_shader_source);
    release_asset_data(&fragment_shader_source);
    
    return program_object_id;
}