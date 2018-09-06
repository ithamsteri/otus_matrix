#include <catch2/catch.hpp>
#include <otus/matrix.hpp>
#include <tuple>

TEST_CASE("2D Matrix operations", "[matrix][2D]") {
    constexpr int DEFAULT_VALUE = -1;
    otus::Matrix<int, DEFAULT_VALUE> matrix{
        {std::make_tuple(14, 68), 52},
        {std::make_tuple(139, 1), 871},
        {std::make_tuple(71, 89), 51},
    };

    const auto start_size = matrix.size();
    REQUIRE(matrix.size() == 3);

    SECTION("Check constructors") {
        SECTION("Default contructor") {
            otus::Matrix<bool, false> boolMatrix;
            REQUIRE(boolMatrix.size() == 0);
            REQUIRE(boolMatrix[23][62] == false);
        }

        SECTION("Copy constructor") {
            auto copyMatrix = matrix;
            REQUIRE(copyMatrix.size() == matrix.size());
            REQUIRE(copyMatrix == matrix);

            SECTION("Copy assign operator") {
                decltype(copyMatrix) assignMatrix;
                REQUIRE(assignMatrix.size() == 0);

                assignMatrix = copyMatrix;
                REQUIRE(assignMatrix.size() == copyMatrix.size());
                REQUIRE(assignMatrix == copyMatrix);
            }
        }

        SECTION("Move constructor") {
            auto moveMatrix = std::move(matrix);
            REQUIRE(moveMatrix.size() == start_size);

            SECTION("Move assign operator") {
                decltype(moveMatrix) moveAssignMatrix;
                REQUIRE(moveAssignMatrix.size() == 0);

                size_t moveMatrixSize = moveMatrix.size();
                moveAssignMatrix = std::move(moveMatrix);
                REQUIRE(moveAssignMatrix.size() == moveMatrixSize);
            }
        }
    }

    SECTION("Check operator[] for getting values from the matrix") {
        REQUIRE(matrix[14][68] == 52);
        REQUIRE(matrix[139][1] == 871);
        REQUIRE(matrix[71][89] == 51);
    }

    SECTION("Clear elements in the matrix") {
        matrix.clear();
        REQUIRE(matrix[14][68] == DEFAULT_VALUE);
        REQUIRE(matrix[139][1] == DEFAULT_VALUE);
        REQUIRE(matrix[71][89] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == 0);
    }

    SECTION("Any empty element equal default value") {
        auto element = matrix[26][8];
        REQUIRE(element == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign value for empty element add new element in the matrix") {
        matrix[100][100] = 314;
        REQUIRE(matrix[100][100] == 314);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Modify an added element only change the element") {
        matrix[139][1] = 178;
        REQUIRE(matrix[139][1] == 178);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign the default value to an existing element deletes it") {
        int value = matrix[14][68];
        matrix[14][68] = DEFAULT_VALUE;
        REQUIRE(value != DEFAULT_VALUE);
        REQUIRE(matrix[14][68] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size - 1);
    }

    SECTION("Use the canonical assignment operator for empty element") {
        ((matrix[51][47] = 23) = 161) = 78;
        REQUIRE(matrix[51][47] == 78);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Use for-range loop with the matrix") {
        size_t counter = 0;
        for (const auto element : matrix) {
            size_t x, y;
            int value;

            std::tie(x, y, value) = element;
            REQUIRE(matrix[x][y] == value);

            ++counter;
        }
        REQUIRE(counter == start_size);
    }

    SECTION("Use iterators of matrix") {
        size_t counter = 0;
        for (auto iter = matrix.begin(); iter != matrix.end(); ++iter) {
            ++counter;

            size_t x, y;
            int value;

            std::tie(x, y, value) = *iter;
            REQUIRE(matrix[x][y] == value);
        }
        REQUIRE(counter == start_size);
    }
}
