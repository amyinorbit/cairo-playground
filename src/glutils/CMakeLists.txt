set(SRC gl.c renderer.c app.c)
set(HDR glutils/gl.h glutils/renderer.h glutils/app.h)
set(ALL_SRC ${SRC} ${HDR})

add_library(glutils STATIC ${ALL_SRC})
target_compile_options(glutils PRIVATE -Wall -Wextra  -Werror)
target_link_libraries(glutils PUBLIC m glfw acfutils)
target_include_directories(glutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(glutils
PROPERTIES
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON
    LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-stack-protector"
)


