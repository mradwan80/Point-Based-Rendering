cmake_minimum_required(VERSION 2.8)
project(pbr)

set (CMAKE_CXX_STANDARD 14)

#set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(external/)  #for glm

#PCL
find_package(PCL REQUIRED COMPONENTS common io )
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

#CGAL
find_package(CGAL)
include_directories(${CGAL_INCLUDE_DIRS})
link_directories(${CGAL_LIBRARY_DIRS})

if(WIN32)
    include_directories(external/glew-1.10.0/include/)
    include_directories(external/glfw/include/)
endif()


#### use glew ####
find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIRS})

#### use glfw ####
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

ADD_EXECUTABLE(
pbr 
src/main.h
src/main.cpp
src/Shader.cpp
src/FloatTexture.cpp
src/FloatFrameBuffer.cpp
)

target_link_libraries(pbr ${GLFW_STATIC_LIBRARIES} GLEW GLU GL ${GLEW_LIBRARIES} ${PCL_LIBRARIES} ${CGAL_LIBRARIES})
