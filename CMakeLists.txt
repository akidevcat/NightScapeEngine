cmake_minimum_required(VERSION 3.26)
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
        src/engine/servers/ShaderCompiler.cpp
        src/engine/servers/ShaderCompiler.h
        src/engine/entity/VisualEntity.cpp
        src/engine/entity/VisualEntity.h
        src/engine/render/Material.cpp
        src/engine/render/Material.h
        src/engine/entity/TriangleVisual.cpp
        src/engine/entity/TriangleVisual.h
        src/engine/data/Mesh.cpp
        src/engine/data/Mesh.h
#        src/engine/render/ShaderProgramTemplate.cpp
        src/engine/render/ShaderProgramTemplate.h
        src/engine/render/VertexShader.cpp
        src/engine/render/VertexShader.h
        src/engine/render/ShaderUtils.cpp
        src/engine/render/ShaderUtils.h
        src/engine/render/PixelShader.cpp
        src/engine/render/PixelShader.h
        src/engine/render/Shader.cpp
        src/engine/render/Shader.h
        src/engine/render/ConstBufferData.cpp
        src/engine/render/ConstBufferData.h
        src/engine/servers/TimeServer.cpp
        src/engine/servers/TimeServer.h
        src/engine/entity/FullscreenQuad.cpp
        src/engine/entity/FullscreenQuad.h
)

target_link_libraries(src d3d9 d3d11 d3dcompiler dinput8 dxguid)

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets/"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
        COMMENT "Copying data"
)
add_dependencies(${PROJECT_NAME} copy_data)