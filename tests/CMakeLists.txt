# Build Google Test Framework
add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

set(OTUS_MATRIX_TEST_TARGET_NAME "${PROJECT_NAME}_test")

add_executable(${OTUS_MATRIX_TEST_TARGET_NAME} Matrix.test.cpp)
target_link_libraries(${OTUS_MATRIX_TEST_TARGET_NAME} otus_matrix::otus_matrix Catch2::Catch2)
set_warning_flags(${OTUS_MATRIX_TEST_TARGET_NAME})

# Make target as CMake Test
add_test(
    NAME MatrixTest
    COMMAND ${OTUS_MATRIX_TEST_TARGET_NAME} ${CATCH_TEST_FILTER}
)