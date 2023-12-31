cmake_minimum_required(VERSION 3.21)
project(physicssim)
include_directories(src/)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-g")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif ()
if (WIN32)
    include_directories(C:/.lib/include)
    link_directories(C:/.lib)
    # add box2d sub project here
    add_subdirectory(box2d)
endif ()
if (MACOSX)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif ()
include_directories(/usr/local/include)
include_directories(libraries)
add_subdirectory(libraries)

add_executable(physicssim main.cpp box.h
        box.cpp
        Keyboard.hpp
        Wall.h
        main.h
        config.h
        Color.h
        audio.h
        beeper.cpp
        beeper.h
        simrenderer.h
        config.cpp
        simrenderer.cpp
        blendmodes.h
        LineModes.h
        Layer.h
        Entity.cpp
        Entity.h
        )
#target_link_libraries(physicssim SDL2 SDL2_image box2d SDL2_ttf)
target_link_libraries(physicssim SDL2 SDL2_image box2d math)
