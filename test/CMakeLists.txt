#
# Tests configuration
#

include(FetchContent)


if(FIND_GTEST_PACKAGE)
    find_package(GTest REQUIRED)
else()
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        GIT_SHALLOW 1
    )

    FetchContent_GetProperties(googletest)

    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    add_library(GTest::Main ALIAS gtest_main)
    target_compile_features(gtest PUBLIC cxx_std_17)
    target_compile_features(gtest_main PUBLIC cxx_std_17)
endif()

include_directories($<TARGET_PROPERTY:Corrosion::Core,INCLUDE_DIRECTORIES>)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#
# Function for setup basic test
#
function(SETUP_BASIC_TEST TEST_NAME TEST_SOURCES)
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE GTest::Main)
    target_link_libraries(${TEST_NAME} PRIVATE Corrosion::Core)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} )
endfunction()

#
# Tests
#
SETUP_BASIC_TEST(NUMBER_LITERALS lexer/number_literals.cpp)
SETUP_BASIC_TEST(STRING_LITERALS lexer/string_literals.cpp)

#
# Install Rust src files.
#
file(INSTALL rs/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/rs)


