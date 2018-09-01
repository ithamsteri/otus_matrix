#include <catch2/catch.hpp>
#include <otus/matrix.hpp>

SCENARIO("2D Matrix operations", "[matrix][2D]") {
    GIVEN("A Matrix with some elements") {
        constexpr int DEFAULT_VALUE = -1;
        otus::Matrix<int, DEFAULT_VALUE> matrix;

        matrix[14][68] = 52;
        matrix[139][1] = 871;
        matrix[71][89] = 51;

        REQUIRE(matrix[14][68] == 52);
        REQUIRE(matrix[139][1] == 871);
        REQUIRE(matrix[71][89] == 51);
        REQUIRE(matrix.size() == 3);

        const auto start_size = matrix.size();

        WHEN("get any empty element") {
            auto element = matrix[26][8];
            THEN("empty element equal default value and no change size") {
                REQUIRE(element == DEFAULT_VALUE);
                REQUIRE(matrix.size() == start_size);
            }
        }

        WHEN("clear matrix") {
            matrix.clear();
            THEN("delete all elements in matrix") {
                REQUIRE(matrix[14][68] == DEFAULT_VALUE);
                REQUIRE(matrix[139][1] == DEFAULT_VALUE);
                REQUIRE(matrix[71][89] == DEFAULT_VALUE);
                REQUIRE(matrix.size() == 0);
            }
        }

        WHEN("assign value for empty element") {
            matrix[100][100] = 314;
            THEN("add new element and increment size") {
                REQUIRE(matrix[100][100] == 314);
                REQUIRE(matrix.size() == start_size + 1);
            }
        }

        WHEN("modify an added element") {
            matrix[139][1] = 178;
            THEN("change value and don't change size") {
                REQUIRE(matrix[139][1] == 178);
                REQUIRE(matrix.size() == start_size);
            }
        }

        WHEN("assign default value for an added element") {
            int value = matrix[14][68];
            matrix[14][68] = DEFAULT_VALUE;
            THEN("delete element from matrix, get value before not change and decrement size") {
                REQUIRE(value != DEFAULT_VALUE);
                REQUIRE(matrix[14][68] == DEFAULT_VALUE);
                REQUIRE(matrix.size() == start_size - 1);
            }
        }

        WHEN("use the canonical assignment operator for empty element") {
            ((matrix[51][47] = 23) = 161) = 78;
            THEN("add new one element to matrix") {
                REQUIRE(matrix[51][47] == 78);
                REQUIRE(matrix.size() == start_size + 1);
            }
        }
    }
}
