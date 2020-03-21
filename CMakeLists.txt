cmake_minimum_required(VERSION 3.5)


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

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "src/")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#
# Qt core 
#
find_package(Qt5Core REQUIRED)
qt5_use_modules(${PROJECT_NAME} Core)

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
