//
//    +-+-+-+-+
//    |O|T|U|S|
//  +-+-+-+-+-+-+
//  |M|A|T|R|I|X|
//  +-+-+-+-+-+-+
//
// Filename: matrix.hpp
// Require:  C++14 Standart
//

#ifndef OTUS_MATRIX_HPP
#define OTUS_MATRIX_HPP

#include <cstddef>
#include <map>
#include <tuple>
#include <type_traits>

namespace otus {

/// Class of the dimensionless matrix.
template <typename T, T DefaultValue, size_t Dimension = 2>
class Matrix {
  private:
    static_assert(Dimension >= 2, "The dimension of the matrix must be equal or greater than 2");

    template <typename TupleType, unsigned N, typename... Types>
    struct generate_tuple_type {
        using type = typename generate_tuple_type<TupleType, N - 1, TupleType, Types...>::type;
    };

    template <typename TupleType, typename... Types>
    struct generate_tuple_type<TupleType, 0, Types...> {
        using type = std::tuple<Types...>;
    };

    /// Class Layout provide access to other Layouts in the matrix.
    template <size_t N, typename... Types>
    class Layout;
    /// Class Layout with parameter N = 1 for access to element of the matrix.
    template <typename... Types>
    class Layout<0, Types...>;

    using Contanter = std::map<typename generate_tuple_type<size_t, Dimension>::type, T>;
    using NextLayout = Layout<Dimension - 1, size_t>;

    Contanter elements_;
    const T defaultValue_{DefaultValue};

  public:
    auto operator[](size_t idx) { return NextLayout(idx, elements_); }
    const auto operator[](size_t idx) const { return NextLayout(idx, elements_); }

    /// Get real count of elements in matrix
    /// @return count of elements
    size_t size() const noexcept { return elements_.size(); }

    /// Clears the mapped matrix.
    void clear() { elements_.clear(); }
};

template <typename T, T DefaultValue, size_t Dimension>
template <size_t N, typename... Types>
class Matrix<T, DefaultValue, Dimension>::Layout {
    using NextLayout = Layout<N - 1, Types..., size_t>;

    const Matrix::Contanter &elements_;
    std::tuple<Types...> tuple_;

  public:
    Layout(std::tuple<Types...> tuple, const Matrix::Contanter &elements)
        : elements_{elements}, tuple_{tuple} {}

    auto operator[](size_t idx) {
        return NextLayout(std::tuple_cat(tuple_, std::tie(idx)), elements_);
    }
    const auto operator[](size_t idx) const {
        return NextLayout(std::tuple_cat(tuple_, std::tie(idx)), elements_);
    }
};

template <typename T, T DefaultValue, size_t Dimension>
template <typename... Types>
class Matrix<T, DefaultValue, Dimension>::Layout<0, Types...> {
    const Matrix::Contanter &elements_;
    std::tuple<Types...> tuple_;
    T value_;

  public:
    Layout(std::tuple<Types...> tuple, const Matrix::Contanter &elements)
        : elements_{elements}, tuple_{tuple} {
        auto iter = elements_.find(tuple_);
        value_ = (iter != elements_.cend()) ? iter->second : DefaultValue;
    }

    auto operator=(const T &value) {
        if (value != DefaultValue) {
            const_cast<Matrix::Contanter &>(elements_)[tuple_] = value;
        } else {
            auto iter = elements_.find(tuple_);
            if (iter != elements_.cend())
                const_cast<Matrix::Contanter &>(elements_).erase(iter);
        }
        value_ = value;
        return *this;
    }

    operator const T &() const noexcept { return value_; }
};

} // namespace otus

#endif // OTUS_MATRIX_HPP