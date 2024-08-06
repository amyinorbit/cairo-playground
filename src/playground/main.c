/*===--------------------------------------------------------------------------------------------===
 * main.c
 *
 * Created by Amy Parent <amy@amyparent.com>
 * Copyright (c) 2024 Amy Parent. All rights reserved
 *
 * NOTICE:  All information contained herein is, and remains the property
 * of Amy Alex Parent. The intellectual and technical concepts contained
 * herein are proprietary to Amy Alex Parent and may be covered by U.S. and
 * Foreign Patents, patents in process, and are protected by trade secret
 * or copyright law. Dissemination of this information or reproduction of
 * this material is strictly forbidden unless prior written permission is
 * obtained from Amy Alex Parent.
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
