cmake_minimum_required(VERSION 3.12.4)


#
# Building in-tree is not allowed.
#

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the source code and call cmake from there. Thank you.")
endif()



project(
        Corrosion
        DESCRIPTION "Rust ASM Compiler made with C++"
        HOMEPAGE_URL "https://github.com/alohaeee/Corrosion"
        LANGUAGES CXX
)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


message("*")
message("* ${PROJECT_NAME} (${CMAKE_BUILD_TYPE})")
message("* ${PROJECT_DESCRIPTION}")
message("* url: ${PROJECT_HOMEPAGE_URL}")
message("*")


#
# Target
#
include(GNUInstallDirs)

add_library(Corrosion_Core
        src/span/symbol.cpp
        src/ast/token.cpp
        src/parser/parse/expr.cpp
        src/parser/parse/diagnostics.cpp
        src/ast/token_tree.cpp)
add_library(Corrosion::Core ALIAS Corrosion_Core)

target_include_directories(Corrosion_Core PUBLIC "${Corrosion_SOURCE_DIR}/src")

target_compile_features(Corrosion_Core PUBLIC cxx_std_20)

#
# Third-Party
#

find_package(spdlog REQUIRED)
find_path(PATH_TO_SPDLOG spdlog)
target_include_directories(Corrosion_Core PUBLIC ${PATH_TO_SPDLOG})
target_link_libraries(Corrosion_Core PUBLIC spdlog::spdlog)

#
# Executable
#

add_executable(Corrosion src/main.cpp)
target_link_libraries(Corrosion PUBLIC Corrosion_Core)
target_compile_features(Corrosion PUBLIC cxx_std_20)


#
# Tests
#

option(BUILD_TESTING "Enable testing with ctest." OFF)

if(BUILD_TESTING)
    option(FIND_GTEST_PACKAGE "Enable finding gtest package." OFF)
    enable_testing()
    add_subdirectory(test)
endif()

#
# Documentation
#

option(BUILD_DOCS "Enable building with documentation." OFF)

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    elseif()
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif()
endif()

unset(BUILD_DOCS CACHE)







