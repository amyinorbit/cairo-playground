/*===--------------------------------------------------------------------------------------------===
 * renderer_impl.h - Renderer implementation details
 *
 * Created by Amy Parent <amy@amyparent.com>
 * Copyright (c) 2022 Amy Parent. All rights reserved
 *
 * Licensed under the MIT License
 *===--------------------------------------------------------------------------------------------===
*/
#ifndef _RENDERER_IMPL_H_
#define _RENDERER_IMPL_H_

#include <glutils/gl.h>
#include <glutils/renderer.h>
#include <acfutils/geom.h>
#include <acfutils/assert.h>

struct gl_quad_t {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint shader;
    GLuint tex;
    
    struct {
        int vtx_pos;
        int vtx_tex0;
        int pvm;
        int tex;
        int alpha;
    } loc;
    
    vect2_t last_pos;
    vect2_t last_size;
};

struct target_t {
    vect2_t size;
    vect2_t offset;
    float proj[16];
};

typedef struct {
    float x;
    float y;
} vec2f_t;

typedef struct {
    vec2f_t pos;
    vec2f_t tex;
} vertex_t;

#endif /* ifndef _RENDERER_IMPL_H_ */

