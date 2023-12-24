project(engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXTENSIONS OFF)

list(APPEND Library_SOURCES
        src/openGL.cpp
        src/shader.cpp
        src/window.cpp
        src/texture.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${Library_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
)

SET(LINK_LIBRARY
        glad
        glfw
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${LINK_LIBRARY}
)