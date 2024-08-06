/*===--------------------------------------------------------------------------------------------===
 * main.c
 *
 * Created by Amy Parent <amy@amyparent.com>
 * Copyright (c) 2024 Amy Parent. All rights reserved
 *
 * Licensed under the MIT License
 *===--------------------------------------------------------------------------------------------===
*/
#include <glutils/app.h>
#include <stdio.h>
#include <acfutils/helpers.h>
#include <acfutils/time.h>
#include <acfutils/perf.h>

static void draw(void *user_ptr, cairo_t *cr, vect2_t size) {
    UNUSED(user_ptr);
    UNUSED(size);
    UNUSED(cr);
    
	// Draw whatever you want in cairo here! You get passed a void data pointer you gave
	// when starting the app, the cairo context to draw in, and its size in pixels.
}

static void log_fn(const char *msg) {
    fprintf(stderr, "%s", msg);
}

int main(int argc, const char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    
    log_init(log_fn, "playground");

    app_window_desc_t desc = {
        .title = "playground",
        .size = VECT2(600, 400),
        .crdraw = draw,
        .scale = 2,
    };
    app_run(1, &desc);
}
