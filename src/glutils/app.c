/*===--------------------------------------------------------------------------------------------===
 * glutils.c - OpenGL utils implementation
 *
 * Created by Amy Parent <amy@amyparent.com>
 * Copyright (c) 2022 Amy Parent. All rights reserved
 *
 * Licensed under the MIT License
 *===--------------------------------------------------------------------------------------------===
*/
#include <glutils/app.h>
#include <acfutils/safe_alloc.h>
#include <acfutils/conf.h>
#include <cairo.h>
#include <stdio.h>

typedef struct {
    GLFWwindow          *handle;
    app_window_desc_t   desc;
    target_t            *target;
    void                *user_data;
    int                 scale;
    bool                in_click;
    
    GLuint              tex;
    gl_quad_t           *quad;
} window_t;


_Noreturn void die(const char *msg) {
    fprintf(stderr, "fatal error: %s\n", msg);
    abort();
}

static void glfw_error(int code, const char *message) {
    fprintf(stderr, "glfw error [%d]: %s\n", code, message);
}

static vect2_t window_get_size(window_t *window) {
    int w = 0, h = 0;
    glfwGetWindowSize(window->handle, &w, &h);
    return VECT2((double)w, (double)h);
}

static void key_cb(GLFWwindow *handle, int key, int scancode, int action, int mods) {
    if(glfwWindowShouldClose(handle)) return;
    
    if(key == GLFW_KEY_UNKNOWN) return;
    if(mods & GLFW_MOD_CONTROL) return;
    bool ctrl = mods & (GLFW_MOD_CONTROL|GLFW_MOD_SUPER);
    if(!(mods & GLFW_MOD_SHIFT)) {
        key = tolower(key);
    }
	
    window_t *window = (window_t *)glfwGetWindowUserPointer(handle);
    if(!window->desc.key) return;
    switch(key) {
        case GLFW_KEY_BACKSPACE: window->desc.key(window->user_data, action, ctrl, 0x08, scancode); break;
        case GLFW_KEY_ENTER: window->desc.key(window->user_data, action, ctrl, '\n', scancode); break;
        default: window->desc.key(window->user_data, action, ctrl, key, scancode); break;
    }
    
}

static void mouse_pos_cb(GLFWwindow* handle, double xpos, double ypos) {
    if(glfwWindowShouldClose(handle)) return;
    window_t *window = (window_t *)glfwGetWindowUserPointer(handle);
    if(!window->in_click) return;
    if(window->desc.mouse) {
        window->desc.mouse(window->user_data, ACTION_MOVE, VECT2(xpos, ypos), window_get_size(window));
    }
}

static void mouse_cb(GLFWwindow* handle, int button, int action, int mods) {
    UNUSED(button);
    UNUSED(mods);
    if(glfwWindowShouldClose(handle)) return;
    
    window_t *window = (window_t *)glfwGetWindowUserPointer(handle);
    vect2_t pos;
    glfwGetCursorPos(handle, &pos.x, &pos.y);
    
    window->in_click = action == GLFW_PRESS;
    if(window->desc.mouse) {
        window->desc.mouse(window->user_data, action, pos, window_get_size(window));
    }
}

static void scroll_cb(GLFWwindow *handle, double x, double y) {
    if(glfwWindowShouldClose(handle)) return;
    
    window_t *window = (window_t *)glfwGetWindowUserPointer(handle);
    vect2_t scroll = VECT2(x, y);
    
    vect2_t pos;
    glfwGetCursorPos(handle, &pos.x, &pos.y);
    if(window->desc.scroll) {
        window->desc.scroll(window->user_data, pos, scroll);
    }
}

static void framebuffer_cb(GLFWwindow* window, int width, int height) {
    UNUSED(window);
    glViewport(0, 0, width, height);
}

static void window_init(window_t *window, const app_window_desc_t *desc, bool first_shot) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window->desc = *desc;
    window->handle = glfwCreateWindow(desc->size.x * desc->scale, desc->size.y * desc->scale, desc->title, NULL, NULL);
    if(!window->handle) die("could not create application window");
    glfwMakeContextCurrent(window->handle);
    
    if(desc->pos.x != 0 || desc->pos.y != 0) {
        glfwSetWindowPos(window->handle, desc->pos.x, desc->pos.y);
    }
    
    int w, h;
    glfwGetWindowSize(window->handle, &w, &h);
    window->scale = w / desc->size.x;

    if(first_shot) {
        glewExperimental=true;
        GLenum err = glewInit();
        if(err != GLEW_OK) {
            logMsg("fatal error: %s", glewGetErrorString(err));
        }
        render_init();
    }
    glfwSetWindowUserPointer(window->handle, (void *)window);
    glfwSetKeyCallback(window->handle, key_cb);
    glfwSetMouseButtonCallback(window->handle, mouse_cb);
    glfwSetFramebufferSizeCallback(window->handle, framebuffer_cb);
    glfwSetScrollCallback(window->handle, scroll_cb);
    glfwSetCursorPosCallback(window->handle, mouse_pos_cb);
    
    window->target = target_new(0, 0, desc->size.x, desc->size.y);
    if(window->desc.init) {
        window->user_data = window->desc.init(window->desc.init_ptr);
    }
    
    if(window->desc.crdraw) {
        window->tex = gl_create_tex(desc->size.x, desc->size.y);
        glBindTexture(GL_TEXTURE_2D, window->tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        window->quad = quad_new(window->tex, 0);
    }
}

static bool window_update(window_t *window) {
    if(glfwWindowShouldClose(window->handle)) return false;
    glfwMakeContextCurrent(window->handle);
    
    int w = 0, h = 0;
    glfwGetWindowSize(window->handle, &w, &h);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(window->desc.crdraw) {
        
        vect2_t size = window->desc.size;//window_get_size(window);
        
        cairo_surface_t *surf = cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32, size.x, size.y);
        
        cairo_t *cr = cairo_create(surf);
        cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
        cairo_paint(cr);
        cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
        
        window->desc.crdraw(window->user_data, cr, size);
        
        cairo_surface_flush(surf);
        void *data = cairo_image_surface_get_data(surf);
    
        glBindTexture(GL_TEXTURE_2D, window->tex);
    	glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            size.x, size.y,
            GL_BGRA, GL_UNSIGNED_BYTE, data
        );
    
        render_quad(window->target, window->quad, VECT2(0, 0), size, 1.0);
        cairo_surface_destroy(surf);
        cairo_destroy(cr);
        
    } else if(window->desc.draw) {
        window->desc.draw(window->user_data, window->target, window_get_size(window));
    }
    glfwSwapBuffers(window->handle);
    return true;
}

static void window_fini(window_t *window) {
    if(window->desc.fini) window->desc.fini(window->user_data);
    target_delete(window->target);
    glfwDestroyWindow(window->handle);
}

static bool quit = false;
void app_close() {
    quit = true;
}

void app_run(unsigned num_windows, const app_window_desc_t *desc) {
    if(!glfwInit()) die("could not initialise window system");
    glfwSetErrorCallback(glfw_error);
    
    window_t *windows = (window_t *)safe_calloc(num_windows, sizeof(*windows));
    for(unsigned i = 0; i < num_windows; ++i) {
        window_init(&windows[i], &desc[i], i == 0);
    }

    quit = false;
    while(!quit) {
        
        for(unsigned i = 0; i < num_windows; ++i) {
            quit |= !window_update(&windows[i]);
        }
        glfwPollEvents();
    }
    
    render_fini();
    
    for(unsigned i = 0; i < num_windows; ++i) {
        window_fini(&windows[i]);
    }
    free(windows);
}
