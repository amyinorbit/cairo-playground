# Use static libc/libgcc etc
function(use_static_libc)
    if(WIN32)
        set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} -static-libgcc"
            PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++"
            PARENT_SCOPE)
        set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS
            "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s"
            PARENT_SCOPE)
        set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
            "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS}  -static-libgcc -static-libstdc++ -s"
            PARENT_SCOPE)
    endif()
    
endfunction(use_static_libc)

# Import acfutils libraries and create an imported target
function(find_acfutils dir)
    if(WIN32)
        set(ARCH "win64")
        set(LONG_ARCH "win-64")
    elseif(APPLE)
        set(ARCH "mac64")
        set(LONG_ARCH "mac-64")
    else()
        set(ARCH "lin64")
        set(LONG_ARCH "linux-64")
    endif()

    set(ACFLIBROOT "${dir}/qmake/${ARCH}")
    set(ACFLIB "${ACFLIBROOT}/libacfutils.a")
    
    execute_process(COMMAND ${dir}/pkg-config-deps ${LONG_ARCH} --no-link-acfutils --cflags
        OUTPUT_VARIABLE LACF_DEP_CFLAGS)
    execute_process(COMMAND ${dir}/pkg-config-deps ${LONG_ARCH} --no-link-acfutils --libs
        OUTPUT_VARIABLE LACF_DEP_LDFLAGS)
    string(REGEX REPLACE "\n$" "" LACF_DEP_LDFLAGS "${LACF_DEP_LDFLAGS}")
    string(REGEX REPLACE "\n$" "" LACF_DEP_CFLAGS "${LACF_DEP_CFLAGS}")
    
    # Does this look inane? Yes! But if we want to use target_compile_options, we need
    # something that doesn't have spaces in it, but instead a list of flags, so we
    # split the compiler flags to give it just that
    string(REPLACE " " ";" LACF_DEP_CFLAGS "${LACF_DEP_CFLAGS}")

    add_library(acfutils STATIC IMPORTED GLOBAL)
    target_include_directories(acfutils INTERFACE "${dir}/src")
    set_target_properties(acfutils PROPERTIES IMPORTED_LOCATION "${ACFLIB}")
    
    target_compile_options(acfutils INTERFACE ${LACF_DEP_CFLAGS})
    target_link_libraries(acfutils INTERFACE ${LACF_DEP_LDFLAGS} ${OPENGL_LIBRARIES})

    if(APPLE)
        target_compile_definitions(acfutils INTERFACE -DAPL=1 -DIBM=0 -DLIN=0)
    elseif(WIN32)
        target_compile_definitions(acfutils INTERFACE -DAPL=0 -DIBM=1 -DLIN=0)
    else()
        target_compile_definitions(acfutils INTERFACE -DAPL=0 -DIBM=0 -DLIN=1)
    endif()
    
    target_compile_definitions(acfutils INTERFACE
        -DCURL_STATICLIB -DGLEW_STATIC -D_LACF_WITHOUT_XPLM)
endfunction(find_acfutils)
