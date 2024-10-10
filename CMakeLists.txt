cmake_minimum_required(VERSION 3.12)
project(TextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
if(WIN32)
    add_definitions(-DWIN=1)
    add_compile_options(/W1)
endif(WIN32)
if(APPLE)
    add_compile_options(-fcolor-diagnostics -Wall -fansi-escape-codes -g -Wno-deprecated)   
endif(APPLE)



# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/glm
    ${CMAKE_SOURCE_DIR}/includes/freetype

    ${CMAKE_SOURCE_DIR}/src/WindowSystem
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Clock
    
)

# Find required packages
find_package(OpenGL REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/WindowSystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/backend/CharacterSystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/backend/Utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/backend/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Clock/*.cpp"

    "${CMAKE_SOURCE_DIR}/glad.c"
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)
# Create executable
add_executable(app ${SOURCES})

# Link libraries
if(APPLE)
    target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/libglfw3.a
    ${CMAKE_SOURCE_DIR}/libs/libfreetype.6.dylib
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)

endif(APPLE)
if(WIN32)
    target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/libs/Windows/glfw3.lib
    opengl32
    )
endif(WIN32)


# Set output directory
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Ensure consistency across build types
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}"
)


# Handle runtime library conflicts if any
if(WIN32)
    target_link_options(app PRIVATE /NODEFAULTLIB:MSVCRT)
endif(WIN32)