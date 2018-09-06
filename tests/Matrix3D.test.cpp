#include <catch2/catch.hpp>
#include <otus/matrix.hpp>
#include <tuple>

TEST_CASE("Three dismension Matrix operations", "[matrix][3D]") {
    constexpr long DEFAULT_VALUE = 1;
    otus::Matrix<long, DEFAULT_VALUE, 3> matrix{
        {std::make_tuple(1, 10, 123), 10},
        {std::make_tuple(13, 42, 18), 70},
        {std::make_tuple(52, 1, 132), 46},
        {std::make_tuple(92, 32, 7), 840},
    };

    const auto start_size = matrix.size();
    REQUIRE(matrix.size() == 4);

    SECTION("Check constructors") {
        SECTION("Default contructor") {
            otus::Matrix<bool, false, 3> boolMatrix;
            REQUIRE(boolMatrix.size() == 0);
            REQUIRE(boolMatrix[23][56][17] == false);
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
        REQUIRE(matrix[1][10][123] == 10);
        REQUIRE(matrix[13][42][18] == 70);
        REQUIRE(matrix[52][1][132] == 46);
        REQUIRE(matrix[92][32][7] == 840);
    }

    SECTION("Clear elements in the matrix") {
        matrix.clear();
        REQUIRE(matrix[1][10][123] == DEFAULT_VALUE);
        REQUIRE(matrix[13][42][18] == DEFAULT_VALUE);
        REQUIRE(matrix[52][1][132] == DEFAULT_VALUE);
        REQUIRE(matrix[92][32][7] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == 0);
    }

    SECTION("Any empty element equal default value") {
        auto element = matrix[8][234][8613];
        REQUIRE(element == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign value for empty element add new element in the matrix") {
        matrix[100][236][350] = 314;
        REQUIRE(matrix[100][236][350] == 314);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Modify an added element only change the element") {
        matrix[92][32][7] = 671;
        REQUIRE(matrix[92][32][7] == 671);
        REQUIRE(matrix.size() == start_size);
    }

    SECTION("Assign the default value to an existing element deletes it") {
        int value = matrix[13][42][18];
        matrix[13][42][18] = DEFAULT_VALUE;
        REQUIRE(value != DEFAULT_VALUE);
        REQUIRE(matrix[13][42][18] == DEFAULT_VALUE);
        REQUIRE(matrix.size() == start_size - 1);
    }

    SECTION("Use the canonical assignment operator for empty element") {
        ((matrix[63][434][826] = 5533) = 731) = 32;
        REQUIRE(matrix[63][434][826] == 32);
        REQUIRE(matrix.size() == start_size + 1);
    }

    SECTION("Use for-range loop with the matrix") {
        size_t counter = 0;
        for (const auto element : matrix) {
            size_t x, y, z;
            int value;

            std::tie(x, y, z, value) = element;
            REQUIRE(matrix[x][y][z] == value);

            ++counter;
        }
        REQUIRE(counter == start_size);
    }

    SECTION("Use iterators of matrix") {
        size_t counter = 0;
        for (auto iter = matrix.begin(); iter != matrix.end(); ++iter) {
            ++counter;

            size_t x, y, z;
            int value;

            std::tie(x, y, z, value) = *iter;
            REQUIRE(matrix[x][y][z] == value);
        }
        REQUIRE(counter == start_size);
    }
}
