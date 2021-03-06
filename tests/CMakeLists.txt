# Include Catch2 header-only library
add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

add_library(catch_main OBJECT
    "Main.test.cpp"
)
target_compile_features(catch_main PRIVATE
    $<TARGET_PROPERTY:Catch2::Catch2,INTERFACE_COMPILE_FEATURES>
)
target_include_directories(catch_main PRIVATE
    $<TARGET_PROPERTY:Catch2::Catch2,INTERFACE_INCLUDE_DIRECTORIES>
)

# List of tests
set(tests
    "ConstMatrix.test.cpp"
    "Matrix.test.cpp"
    "Matrix1D.test.cpp"
    "Matrix3D.test.cpp"
)

foreach(file ${tests})
    get_filename_component(test_name ${file} NAME_WE)
    set(testcase "matrix_test_${test_name}")

    add_executable(${testcase} $<TARGET_OBJECTS:catch_main> ${file})
    target_compile_features(${testcase} PRIVATE
        $<TARGET_PROPERTY:otus_matrix::otus_matrix,INTERFACE_COMPILE_FEATURES>
    )
    target_include_directories(${testcase} PRIVATE
        $<TARGET_PROPERTY:Catch2::Catch2,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:otus_matrix::otus_matrix,INTERFACE_INCLUDE_DIRECTORIES>
    )
    set_warning_flags(${testcase})

    # Make target as CMake Test
    add_test(
        NAME ${test_name}
        COMMAND ${testcase} ${CATCH_TEST_FILTER}
    )

endforeach()