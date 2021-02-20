cmake_minimum_required(VERSION 3.17)
project(calculatorProject C)

set(CMAKE_C_STANDARD 11)

add_executable(calculatorProject main.c stacks/stackComplex.h libraries.h stacks/stackComplex.c stacks/stackVariable.c stacks/stackVariable.h stacks/stackOperator.c stacks/stackOperator.h)