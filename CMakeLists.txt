# Copyright 2019 Anders Madsen, Emil Jørgensen Njor, Emil Stenderup Bækdahl, Frederik Baymler
# Mathiesen, Nikolaj Jensen Ulrik, Simon Mejlby Virenfeldt
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER clang++)

project(p7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_FLAGS "-g -fno-omit-frame-pointer -Wall")

include(ExternalProject)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(EXT_LIBRARY_INSTALL_PATH ${PROJECT_SOURCE_DIR}/build)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(GetDependencies "Fetch external dependencies from web." ON)
if (GetDependencies)
    ExternalProject_add(json-ext
            URL https://github.com/open-source-parsers/jsoncpp/archive/1.9.2.zip
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DJSONCPP_WITH_WARNING_AS_ERROR=OFF -DJSONCPP_WITH_STRICT_ISO=OFF -DJSONCPP_WITH_PKGCONFIG_SUPPORT=OFF -DJSONCPP_WITH_CMAKE_PACKAGE=OFF -DBUILD_SHARED_LIBS=OFF
            )

    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
endif (GetDependencies)



include_directories(
    ${PROJECT_SOURCE_DIR}/lib/jsoncpp/include
    ${PROJECT_SOURCE_DIR}/tcp/include
    ${PROJECT_SOURCE_DIR}/robot/include
    ${PROJECT_SOURCE_DIR}/wbt-translator/include
    ${PROJECT_SOURCE_DIR}/scheduling/include
    ${PROJECT_SOURCE_DIR}/util/include
    ${PROJECT_SOURCE_DIR}/config/include
    ${PROJECT_SOURCE_DIR}/order/include
)

# Libraries
#add_subdirectory(lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#add_subdirectory(tcp)
add_subdirectory(config)
#add_subdirectory(scheduling)
#add_subdirectory(robot)
#add_subdirectory(order)

# Executables
#add_subdirectory(tests)
#add_subdirectory(port_discovery)
#add_subdirectory(communication)
#add_subdirectory(wbt-translator)
