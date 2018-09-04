#include <catch2/catch.hpp>
#include <otus/matrix.hpp>
#include <tuple>

TEST_CASE("One dismension Matrix (vector) operations", "[matrix][1D]") {
    constexpr int DEFAULT_VALUE = -1;
    otus::Matrix<int, DEFAULT_VALUE, 1> matrix{
        {std::make_tuple(1411), 2}, {std::make_tuple(139), 71}, {std::make_tuple(393), 27},
        {std::make_tuple(92), 840}, {std::make_tuple(199), 37}, {std::make_tuple(5), 3421},
    };

    const auto start_size = matrix.size();
    REQUIRE(matrix.size() == 6);

    SECTION("Check constructors") {
        SECTION("Default contructor") {
            otus::Matrix<bool, false, 1> boolMatrix;
            REQUIRE(boolMatrix.size() == 0);
            REQUIRE(boolMatrix[23] == false);
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
        REQUIRE(matrix[1411] == 2);
        REQUIRE(matrix[139] == 71);
        REQUIRE(matrix[393] == 27);
        REQUIRE(matrix[92] == 840);
        REQUIRE(matrix[199] == 37);
        REQUIRE(matrix[5] == 3421);
    }

    SECTION("Clear elements in the matrix") {
        matrix.clear();
        REQUIRE(matrix[1411] == DEFAULT_VALUE);
        REQUIRE(matrix[139] == DEFAULT_VALUE);
        REQUIRE(matrix[393] == DEFAULT_VALUE);
        REQUIRE(matrix[92] == DEFAULT_VALUE);
        REQUIRE(matrix[199] == DEFAULT_VALUE);
        REQUIRE(matrix[5] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == 0);
    }

    SECTION("Any empty element equal default value") {
        auto element = matrix[8];
        REQUIRE(element == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign value for empty element add new element in the matrix") {
        matrix[100] = 314;
        REQUIRE(matrix[100] == 314);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Modify an added element only change the element") {
        matrix[139] = 671;
        REQUIRE(matrix[139] == 671);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign the default value to an existing element deletes it") {
        int value = matrix[92];
        matrix[92] = DEFAULT_VALUE;
        REQUIRE(value != DEFAULT_VALUE);
        REQUIRE(matrix[92] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size - 1);
    }

    SECTION("Use the canonical assignment operator for empty element") {
        ((matrix[51] = 23) = 161) = 78;
        REQUIRE(matrix[51] == 78);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Use for-range loop with the matrix") {
        size_t counter = 0;
        for (const auto element : matrix) {
            size_t idx;
            int value;

            std::tie(idx, value) = element;
            REQUIRE(matrix[idx] == value);

            ++counter;
        }
        REQUIRE(counter == start_size);
    }

    SECTION("Use iterators of matrix") {
        size_t counter = 0;
        for (auto iter = matrix.begin(); iter != matrix.end(); ++iter) {
            ++counter;

            size_t idx;
            int value;

            std::tie(idx, value) = *iter;
            REQUIRE(matrix[idx] == value);
        }
        REQUIRE(counter == start_size);
    }
}
