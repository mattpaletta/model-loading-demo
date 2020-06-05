cmake_minimum_required(VERSION 3.11)

project(model-loading LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

## These two things are needed for YCM
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

# Make the engine follow the game.
set(ENGINE_EXTRA_COMPILER_CHECKS ON)
set(ENGINE_WERROR OFF)

set(ENGINE_ENABLE_MULTITHREADED OFF)
set(ENGINE_ENABLE_AUDIO OFF)
set(ENGINE_ENABLE_ANIMATION OFF)
set(ENGINE_ENABLE_VR OFF)
set(ENGINE_ENABLE_JSON OFF)
set(ENGINE_ENABLE_TEXT OFF)
set(ENGINE_MIN_GAME_OBJECT ON)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# For now, get the latest version
add_subdirectory(external/engine)

file(GLOB VERTEX_SHADERS shaders/*.vert)
file(GLOB FRAGMENT_SHADERS shaders/*.frag)
file(GLOB GEOMETRY_SHADERS shaders/*.geom)

set(ALL_SHADERS ${VERTEX_SHADERS} ${FRAGMENT_SHADERS} ${GEOMETRY_SHADERS})

# This will compile all our shaders, and create a new target `test_loading_shaders` that depends on all tests.
test_shaders_func("${ALL_SHADERS}" "test_loading_shaders")

file(GLOB SRC_FILES src/*.cpp)
file(GLOB INC_FILES include/loading/*.hpp)

add_library(loading STATIC ${SRC_FILES} ${INC_FILES} ${ALL_SHADERS})
target_include_directories(loading PUBLIC include)
target_link_libraries(loading PUBLIC engine stb_image)
target_compile_definitions(loading PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

add_dependencies(loading test_loading_shaders)

###################
# start samples
###################
add_executable(nanosuit samples/nanosuit.cpp)
target_link_libraries(nanosuit PUBLIC loading)

add_executable(backpack samples/backpack.cpp)
target_link_libraries(backpack PUBLIC loading)
