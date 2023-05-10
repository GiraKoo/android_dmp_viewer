cmake_minimum_required(VERSION 3.8)

project(dmp_viewer)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

# 如果是Linux系统
if (UNIX)
    # 设置编译器
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    # 设置编译选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

    find_package(OpenGL REQUIRED)
    message(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")

    find_package(GLUT REQUIRED)
    message(STATUS "GLUT_LIBRARIES = ${GLUT_LIBRARIES}")

# 如果是Windows系统
elseif (WIN32)

    # 设置编译器
    set(CMAKE_MAKE_PROGRAM "D:/Programs/CMake/bin/cmake.exe")

    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")

    # 设置编译选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

    # 设置OpenGL
    set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um")
    set(OPENGL_LIBRARY "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64")

    # 设置GLUT
    set(GLUT_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um")
    set(GLUT_LIBRARY "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64")

    include_directories(${OPENGL_INCLUDE_DIR})
    include_directories(${GLUT_INCLUDE_DIR})

    link_directories(${OPENGL_LIBRARY})
    link_directories(${GLUT_LIBRARY})

endif ()

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/imgui)

# 源码
FILE(GLOB_RECURSE SRC_FILES 
    app/src/*.cpp)

set(SRC_FILES 
    ${SRC_FILES}
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glut.cpp)

message(STATUS "SRC_FILES = ${SRC_FILES}")

include_directories(third-party)
include_directories(${IMGUI_DIR})

add_definitions(
    -std=c++11
    -Wall
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARIES})