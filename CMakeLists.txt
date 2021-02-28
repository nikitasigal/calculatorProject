cmake_minimum_required(VERSION 3.17)
project(calculatorProject C)

set(CMAKE_C_STANDARD 11)

add_executable(calculatorProject main.c stacks/stackComplex.h libraries.h stacks/stackComplex.c stacks/stackVariable.c stacks/stackVariable.h stacks/stackOperator.c stacks/stackOperator.h maps/mapComplex.c maps/mapComplex.h maps/hashing.h maps/hashing.c maps/mapFunctions.c maps/mapFunctions.h "calculations.c" "calculations.h" GUI/GUI.c GUI/GUI.h errorHandler.c errorHandler.h)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

TARGET_LINK_LIBRARIES(calculatorProject ${GTK3_LIBRARIES})