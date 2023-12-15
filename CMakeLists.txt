cmake_minimum_required(VERSION 3.26)
project(physicssim)
include_directories(src/)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-g")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif ()
if (MACOSX)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif ()
include_directories(/usr/local/include)
add_executable(physicssim main.cpp box.hpp
        Keyboard.hpp
        Wall.h
        main.h
        config.h
        Color.h
        audio.h
        beeper.cpp
        beeper.h
        renderer.h)
target_link_libraries(physicssim SDL2 SDL2_image box2d SDL2_ttf)
