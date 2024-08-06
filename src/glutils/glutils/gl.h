//===--------------------------------------------------------------------------------------------===
// gl.h - GL defines and stuff
//
// Created by Amy Parent <amy@amyparent.com>
// Copyright (c) 2019 Amy Parent
// Licensed under the MIT License
// =^•.•^=
//===--------------------------------------------------------------------------------------------===
#pragma once
// #include "glad.h"
#include <acfutils/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#if IBM
#include <GL/gl.h>
// #include <GL/glu.h>
#elif LIN
#define TRUE 1
#define FALSE 0
#include <GL/gl.h>
// #include <GL/glu.h>
#else
#define TRUE 1
#define FALSE 0
#if __GNUC__
#include <OpenGL/gl.h>
// #include <OpenGL/glu.h>
#else
#include <gl.h>
// #include <glu.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

GLuint gl_create_program(const char *vertex, const char *fragment);
GLuint gl_load_shader(const char *source, int type);
GLuint gl_load_tex(const char *path, int *w, int *h);
GLuint gl_create_tex(unsigned width, unsigned height);
void gl_ortho(float proj[16], float x, float y, float width, float height);

void check_gl(const char *where, int line);

#ifdef GL_DEBUG
#define CHECK_GL() check_gl(__FUNCTION__, __LINE__)
#else
#define CHECK_GL()
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif
