cmake_minimum_required(VERSION 3.27)
project(learningOpenGL)

include(FetchContent)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8)

FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(glad)
add_subdirectory(engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} main.cpp)

FetchContent_MakeAvailable(
        glfw
        glm)

set(LINK_LIBRARY
        engine
        glad
        glfw
        glm)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LINK_LIBRARY})

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif (WIN32)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resource
        ${PROJECT_BINARY_DIR}/resource
        COMMENT "Copying resources into binary directory")

add_dependencies(${PROJECT_NAME} copy_resources)