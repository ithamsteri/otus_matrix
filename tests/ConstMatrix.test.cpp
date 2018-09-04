#include <catch2/catch.hpp>
#include <otus/matrix.hpp>

TEST_CASE("Const two dismension Matrix operations", "[matrix][2D][const]") {
    constexpr long DEFAULT_VALUE = 1;
    using MatrixType = otus::Matrix<long, DEFAULT_VALUE>;

    MatrixType matrix{
        {std::make_tuple(100, 100), 314},
        {std::make_tuple(29, 62), 25253},
        {std::make_tuple(5, 3123), 1341},
        {std::make_tuple(634, 63), 6245},
    };

    const auto start_size = matrix.size();
    REQUIRE(matrix.size() == 4);

    // get const copy of matrix
    const MatrixType const_matrix = matrix;
    REQUIRE(const_matrix.size() == 4);

    SECTION("Check operator[] for getting values from the matrix") {
        REQUIRE(const_matrix[100][100] == matrix[100][100]);
        REQUIRE(const_matrix[29][62] == matrix[29][62]);
        REQUIRE(const_matrix[5][3123] == matrix[5][3123]);
        REQUIRE(const_matrix[634][63] == matrix[634][63]);
    }

    SECTION("Any empty element equal default value") {
        auto element = const_matrix[236][53668];
        REQUIRE(element == DEFAULT_VALUE);
        REQUIRE(const_matrix.size() == start_size);
    }

    SECTION("use for each loop with const matrix") {
        size_t counter = 0;
        for (const auto element : matrix) {

            size_t x, y;
            int value;

            std::tie(x, y, value) = element;
            REQUIRE(const_matrix[x][y] == value);

            ++counter;
        }
        REQUIRE(counter == start_size);
    }

    SECTION("Use iterators of matrix") {
        size_t counter = 0;
        for (auto iter = const_matrix.begin(); iter != const_matrix.end(); ++iter) {
            ++counter;

            size_t x, y;
            int value;

            std::tie(x, y, value) = *iter;
            REQUIRE(const_matrix[x][y] == value);
        }
        REQUIRE(counter == start_size);
    }
}
