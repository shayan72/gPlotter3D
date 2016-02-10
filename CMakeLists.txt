cmake_minimum_required(VERSION 2.8)
project(gPlotter3D C)

set(PACKAGE_VERSION "0.01")

set(gplotter_SRC
    Matrix.c
    Vector.c
    analyze_expression.c
)

set(gplotter_HEAD
    Matrix.h
    Vector.h
    analyze_expression.h
)

set(gplotter_SRCS
   ${gplotter_HEAD}
   ${robotoos_SRC}
   main.c
)

find_package(GTK2 REQUIRED)
include_directories(${CMAKE_BINARY_DIR} ${CMAKE_TOP_SRC_DIR})

if(GTK2_FOUND)
	include_directories(${GTK2_INCLUDE_DIRS})
	add_executable(gPlotter3D ${gplotter_SRCS})
	target_link_libraries(gPlotter3D ${GTK2_LIBRARIES} m)
else()
	message( FATAL_ERROR "GTK2 not found" )
endif()
