/*===--------------------------------------------------------------------------------------------===
 * app.h
 *
 * Created by Amy Parent <amy@amyparent.com>
 * Copyright (c) 2022 Amy Parent. All rights reserved
 *
 * Licensed under the MIT License
 *===--------------------------------------------------------------------------------------------===
*/
#ifndef _APP_H_
#define _APP_H_

#include <acfutils/geom.h>
#include <cairo.h>
#include <glutils/gl.h>
#include <glutils/renderer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char  *title;
    const char  *id;
    vect2_t     size;
    vect2_t     pos;
    int         scale;
    void        *init_ptr;
    void        *(*init)(void *ptr);
    void        (*fini)(void *ptr);
	
    void        (*crdraw)(void *ptr, cairo_t *cr, vect2_t size);
    void        (*draw)(void *ptr, target_t *target, vect2_t size);
	
    void        (*mouse)(void *ptr, int action, vect2_t pos, vect2_t size);
    void        (*key)(void *ptr, int action, bool ctrl, int key, int scancode);
    void        (*scroll)(void *ptr, vect2_t pos, vect2_t scroll);
} app_window_desc_t;

typedef void (*app_update_f)();

typedef enum {
    ACTION_PRESS    = GLFW_PRESS,
    ACTION_RELEASE  = GLFW_RELEASE,
    ACTION_MOVE     = 10
} app_action_t;

void app_close();
void app_run(unsigned num_windows, const app_window_desc_t *desc);


#ifdef __cplusplus
}
#endif


#endif /* ifndef _APP_H_ */

