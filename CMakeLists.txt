cmake_minimum_required(VERSION 3.28)
project(NightScapeEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Find packages
find_package(Catch2 3 REQUIRED)

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

# include GLEW
include_directories(${PROJECT_SOURCE_DIR}/libs/GLEW/include)

# include entt
include_directories(${PROJECT_SOURCE_DIR}/libs/entt/include)

# Gather all .cpp files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/src/*.h*")

# Add library
add_library(${PROJECT_NAME}        OBJECT ${HEADERS} ${SOURCES})
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)

# Link SDL2
target_link_libraries(${PROJECT_NAME}        ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_shared ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_static ${SDL2_LIBRARIES})

# Link GLEW
target_link_libraries(${PROJECT_NAME}        ${PROJECT_SOURCE_DIR}/libs/GLEW/lib/x64/glew32.lib glu32 opengl32)
target_link_libraries(${PROJECT_NAME}_shared ${PROJECT_SOURCE_DIR}/libs/GLEW/lib/x64/glew32.lib glu32 opengl32)
target_link_libraries(${PROJECT_NAME}_static ${PROJECT_SOURCE_DIR}/libs/GLEW/lib/x64/glew32.lib glu32 opengl32)

# Link GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME}_shared glm::glm)
target_link_libraries(${PROJECT_NAME}_static glm::glm)

# Precompile pch.h
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/pch.h)

# Prepare lib export
add_custom_command(
        TARGET ${PROJECT_NAME}_static PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E rm -r -f
        ${PROJECT_SOURCE_DIR}/libs/NSE
        COMMENT "Removing old NSE export includes: ${PROJECT_SOURCE_DIR}/libs/NSE")
foreach(CurrentHeaderFile IN LISTS HEADERS)
    string(REPLACE ${PROJECT_SOURCE_DIR}/src/ "" CurrentFileSubPath ${CurrentHeaderFile})
    add_custom_command(
            TARGET ${PROJECT_NAME}_static PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E copy ${CurrentHeaderFile} ${PROJECT_SOURCE_DIR}/libs/NSE/include/${CurrentFileSubPath}
            COMMENT "Copying new NSE export includes: ${CurrentHeaderFile}")
endforeach()


#file(
#        REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/libs/NSE)
#file(
#        COPY ${PROJECT_SOURCE_DIR}/src/
#        DESTINATION ${PROJECT_SOURCE_DIR}/libs/NSE/include
#        FILES_MATCHING PATTERN "*.h*")
#file(
#        COPY ${CMAKE_BINARY_DIR}/
#        DESTINATION ${PROJECT_SOURCE_DIR}/libs/NSE/lib/
#        FILES_MATCHING PATTERN "*.dll")
#file(
#        COPY ${CMAKE_BINARY_DIR}/
#        DESTINATION ${PROJECT_SOURCE_DIR}/libs/NSE/lib/
#        FILES_MATCHING PATTERN "*.lib")

# Add tests
include(Catch)
enable_testing()
file(GLOB_RECURSE TESTS_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
add_executable(unit_tests ${TESTS_SOURCES} ${HEADERS})
#target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/libs/NSE/include)
#target_include_directories(unit_tests PRIVATE ${HEADERS})
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(unit_tests Catch2::Catch2WithMain) # Put this before SDL2 for the main function
target_link_libraries(unit_tests ${SDL2_LIBRARIES})
target_link_libraries(unit_tests ${PROJECT_SOURCE_DIR}/libs/GLEW/lib/x64/glew32.lib)
#target_link_libraries(unit_tests Catch2::Catch2WithMain)
target_link_libraries(unit_tests ${PROJECT_NAME}_static)
catch_discover_tests(unit_tests)
