# Build Google Test Framework
add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

add_library(catch_main OBJECT
    "Main.test.cpp"
)
target_link_libraries(catch_main PRIVATE Catch2::Catch2)

set(tests
    "ConstMatrix.test.cpp"
    "Matrix.test.cpp"
)

foreach(file ${tests})
    get_filename_component(test_name ${file} NAME_WE)
    set(testcase "matrix_test_${test_name}")

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
    target_link_libraries(${testcase} PRIVATE Catch2::Catch2 otus_matrix::otus_matrix)
    set_warning_flags(${testcase})

    # Make target as CMake Test
    add_test(
        NAME ${test_name}
        COMMAND ${testcase} ${CATCH_TEST_FILTER}
    )

endforeach()