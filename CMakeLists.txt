cmake_minimum_required(VERSION 3.12)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src WIN32
        src/main.cpp
        src/application.cpp
        src/application.h
        src/game/game.cpp
        src/game/game.h
        src/engine/servers/InputServer.cpp
        src/engine/servers/InputServer.h
        src/engine/servers/RenderServer.cpp
        src/engine/servers/RenderServer.h
        src/engine/servers/AssetServer.cpp
        src/engine/servers/AssetServer.h
        src/engine/engine.cpp
        src/engine/engine.h
        src/engine/math.cpp
        src/engine/math.h
        src/engine/servers/SoundServer.cpp
        src/engine/servers/SoundServer.h
        src/engine/entity/SceneEntity.cpp
        src/engine/entity/SceneEntity.h
        src/engine/entity/Camera.cpp
        src/engine/entity/Camera.h
        src/engine/entity/Entity.cpp
        src/engine/entity/Entity.h
        src/engine/scene/Scene.cpp
        src/engine/scene/Scene.h
        src/engine/servers/SceneServer.cpp
        src/engine/servers/SceneServer.h
)

target_link_libraries(src d3d9 d3d11 d3dcompiler dinput8 dxguid)