cmake_minimum_required(VERSION 3.22)
project(DS_WET2)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(DS_WET2
        AVLTree.h
        Company.cpp
        Company.h
        Employee.cpp
        Employee.h
        exceptions.h
        hashTable.h
        library2.h
        library2.cpp
        RankInfo.h
        UF.cpp
        UF.h
        Workplace.cpp
        Workplace.h main.cpp)
