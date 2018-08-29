#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <otus/matrix.hpp>

TEST_CASE("Matrix created empty", "[matrix]") {
    otus::Matrix matrix;

    REQUIRE(matrix.size() == 0);
}
