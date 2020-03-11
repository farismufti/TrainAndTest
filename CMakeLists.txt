cmake_minimum_required(VERSION 3.15)
project(CourseWork2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CourseWork2
        CMakeLists.txt
        Example_main.c
        ExampleData.h
        SolutionListOperations.c
        SolutionListOperations.h
        StructureDefinitions.h
        TrainAndTest.c
        TrainAndTest.h)
