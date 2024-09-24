cmake_minimum_required(VERSION 3.28)
project(NightScapeEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# include SDL2
set(SDL2MAIN_LIBRARY "")
set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/SDL2/include)

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2.lib;${PROJECT_SOURCE_DIR}/libs/SDL2/lib/x64/SDL2main.lib")
else ()
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/libs/SDL2/lib/x86/SDL2.lib;${PROJECT_SOURCE_DIR}/libs/SDL2/lib/x86/SDL2main.lib")
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

# include SDL2
include_directories(${SDL2_INCLUDE_DIR})

# include freeglut
#include_directories(${PROJECT_SOURCE_DIR}/libs/freeglut/include)

# include GLEW
include_directories(${PROJECT_SOURCE_DIR}/libs/GLEW/include)

#include entt
include_directories(${PROJECT_SOURCE_DIR}/libs/entt/include)

# Gather all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/src/*.h*")

# Add library
add_library(${PROJECT_NAME} SHARED ${SOURCES}
        src/NSE.cpp
        src/pch.h
        src/engine/runtime/core/main/EngineConfiguration.h
        src/engine/runtime/core/render/RenderServer_OpenGLImpl.cpp
        src/engine/runtime/core/render/RenderServer.h
        src/engine/runtime/core/main/EngineServer.cpp
        src/engine/runtime/core/main/EngineServer.h
        src/engine/runtime/core/scene/SceneServer.h
        src/engine/runtime/core/scene/Entity.h
        src/engine/runtime/core/main/Ref.h)

# Include private source code
#target_include_directories(NightScapeEngine PRIVATE "${PROJECT_SOURCE_DIR}/src")

# Link SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Link freeglut
#target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libs/freeglut/lib/x64/freeglut.lib)

# Link GLEW
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libs/GLEW/lib/x64/glew32.lib)

# Prepare lib export
file(
        REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/libs/NSE)
file(
        COPY ${PROJECT_SOURCE_DIR}/src/
        DESTINATION ${PROJECT_SOURCE_DIR}/libs/NSE/include
        FILES_MATCHING PATTERN "*.h*")