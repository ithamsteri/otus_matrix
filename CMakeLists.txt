cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Set project name and version
project(otus_matrix VERSION 1.0.0 LANGUAGES CXX)

option(OTUS_MATRIX_BUILD_TESTING "Build the unit tests when BUILD_TESTING is enabled." ON)

set(OTUS_MATRIX_TARGET_NAME ${PROJECT_NAME})
set(OTUS_MATRIX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# set warning flags for popular compilers
function(set_warning_flags target)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_compile_options(${target} PRIVATE "-Wall" "-Wextra" "-pedantic-errors")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
                target_compile_options(${target} PRIVATE "/W4" "/wd4389" "/wd4309" "/wd4566" "/wd4996")
        endif()
endfunction()

# Add Interface library for header-only library
add_library(${OTUS_MATRIX_TARGET_NAME} INTERFACE)
# Add alias `otus_matrix::otus_matrix` for library
add_library(${PROJECT_NAME}::${OTUS_MATRIX_TARGET_NAME} ALIAS ${OTUS_MATRIX_TARGET_NAME})
# Add source files for targets. Specialy for IDE.
target_sources(${OTUS_MATRIX_TARGET_NAME} INTERFACE 
    "${OTUS_MATRIX_INCLUDE_DIR}/otus/matrix.hpp"
)
# Add include directory for the target
target_include_directories(${OTUS_MATRIX_TARGET_NAME} INTERFACE
    "${OTUS_MATRIX_INCLUDE_DIR}"
)
# Add requirement C++11 feature for library
target_compile_features(${OTUS_MATRIX_TARGET_NAME} INTERFACE cxx_std_14)

# --- Testing ---
if(BUILD_TESTING AND OTUS_MATRIX_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
