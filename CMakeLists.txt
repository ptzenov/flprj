cmake_minimum_required(VERSION 3.0) 
project(flprj)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--std=c++11")
set(CMAKE_CXX "g++")

#Bring the headers, such as Student.h into the project
include_directories(
	"include"
	)

link_directories(
	"lib"
	)


file(GLOB SOURCES "src/*.cpp")
add_executable(flprj ${SOURCES})

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

find_package(OpenGL)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INDLUCE_DIRS})
	target_link_libraries (flprj ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

find_package (GLUT)
if (GLUT_FOUND)
	include_directories(${GLUT_INCLUDE_DIRS})
	target_link_libraries (flprj ${GLUT_LIBRARIES})
endif (GLUT_FOUND)

# not necessary...
#find_package (GLEW)
#if (GLEW_FOUND)
#	include_directories(${GLEW_INCLUDE_DIRS})
#	target_link_libraries (flprj ${GLEW_LIBRARIES})
#endif (GLEW_FOUND)

target_link_libraries(flprj 
	papi
	)


