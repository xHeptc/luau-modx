cmake_minimum_required(VERSION 3.11)
project(luau_modx CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/arch:IA32)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/plugin.def\"")
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(-m32)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

add_library(luau_modx SHARED src/main.cpp)
set_target_properties(luau_modx PROPERTIES PREFIX "")

target_include_directories(luau_modx PRIVATE
    sdk/metamod/metamod
    sdk/hlsdk/dlls
    sdk/hlsdk/public
    sdk/hlsdk/common
    sdk/hlsdk/engine
)

add_custom_command(TARGET luau_modx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/dist/addons/luau-modx/dlls
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:luau_modx> ${CMAKE_SOURCE_DIR}/dist/addons/luau-modx/dlls/
    COMMENT "Copying compiled plugin to dist/ directory..."
)

##
## RE-BUILD EVERYTIME YOU CHANGE ANYTHING IN HERE
##

## TO BUILD   
## cmake -S . -B build -A Win32

## TO COMPILE 
## cmake --build build --config Release

## IF GETTING CMakeCache.txt is different than the directory ...
## rmdir build
## Then A (Yes To All) 