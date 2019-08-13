cmake_minimum_required(VERSION 3.10)

# -------------------------------------------------------------------------------------- Compiler options
set(PROJECT_NAME simplePlot)
project(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")


# -------------------------------------------------------------------------------------- Setup paths
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN_DIR ${ROOT_DIR}/bin)
set(INC_DIR ${ROOT_DIR}/include)
set(SRC_DIR ${ROOT_DIR}/src)
set(LIB_DIR ${ROOT_DIR}/lib)

# -------------------------------------------------------------------------------------- Source files
file(GLOB SOURCE src/*.cpp
        include/*.h
        include/*.hpp)

include_directories(
        include
        src)

# -------------------------------------------------------------------------------------- gnuplot
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${BIN_DIR})

add_library(${PROJECT_NAME} ${SOURCE})


# -------------------------------------------------------------------------------------- main
add_executable(main ${SOURCE})
target_link_libraries(main ${PROJECT_NAME})
