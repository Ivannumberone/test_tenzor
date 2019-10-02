cmake_minimum_required(VERSION 3.5)

project(AnalyzerDependencies LANGUAGES CXX)
project(Console LANGUAGES CXX)
project(Test LANGUAGES CXX)


SET(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
ADD_DEFINITIONS( "-DHAS_BOOST" )

add_library(AnalyzerDependencies AnalyzerDependenciesProject/src/analyzer.cpp
        AnalyzerDependenciesProject/src/analyzer.h
        AnalyzerDependenciesProject/src/analyzer.cpp
        AnalyzerDependenciesProject/src/helpers/file_parser.h
        AnalyzerDependenciesProject/src/helpers/file_parser.cpp
        AnalyzerDependenciesProject/src/helpers/included_file.h
        AnalyzerDependenciesProject/src/helpers/included_file.cpp)

add_executable(Console Console/src/console.cpp)
include_directories(Console AnalyzerDependencies AnalyzerDependenciesProject/src/)
target_link_libraries(Console PUBLIC AnalyzerDependencies)
add_executable(Test Test/src/tests.cpp)
target_link_libraries(Test PUBLIC AnalyzerDependencies)
target_link_libraries(AnalyzerDependencies PRIVATE "stdc++fs")
target_link_libraries(Console PRIVATE "stdc++fs")
target_link_libraries(Test PRIVATE "stdc++fs")



