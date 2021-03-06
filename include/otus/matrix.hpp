//
//    +-+-+-+-+
//    |O|T|U|S|
//  +-+-+-+-+-+-+
//  |M|A|T|R|I|X|
//  +-+-+-+-+-+-+
//
// Filename:    matrix.hpp
// Require:     C++14 Standard
// Author:      Alexander Yashkin
// Description: The multi-dimensional sparse matrix.
//

#ifndef OTUS_MATRIX_HPP
#define OTUS_MATRIX_HPP

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace otus {

/// Class of The multi-dimensional sparse matrix
template <typename T, T DefaultValue, size_t Dimension = 2>
class Matrix {
  private:
    static_assert(Dimension > 0, "The dimension of the matrix must be greater than 0");

    template <typename TupleType, unsigned N, typename... Types>
    struct generate_tuple_type {
        using type = typename generate_tuple_type<TupleType, N - 1, TupleType, Types...>::type;
    };

    template <typename TupleType, typename... Types>
    struct generate_tuple_type<TupleType, 0, Types...> {
        using type = std::tuple<Types...>;
    };

    /// Iterator used as adaptor for concatanate key and value from map
    class Iterator;
    /// TupleHash used as Hash function for std::unordered_map
    class TupleHash;
    /// Using these Layouts for access to other Layouts in the matrix
    template <size_t N, typename... Types>
    class Layout;
    /// Using this Layout as Smart Object for get/set values of the matrix
    template <typename... Types>
    class Layout<0, Types...>;

    using TupleKey = typename generate_tuple_type<size_t, Dimension>::type;
    using Contanter = std::unordered_map<TupleKey, T, TupleHash>;
    using NextLayout = Layout<Dimension - 1, size_t>;

    Contanter elements_;
    const T defaultValue_{DefaultValue};

  public:
    Matrix() = default;
    ~Matrix() = default;
    Matrix(const Matrix &other) noexcept : elements_(other.elements_) {}
    Matrix(Matrix &&other) noexcept : elements_(std::move(other.elements_)) {}

    Matrix(std::initializer_list<std::pair<const TupleKey, T>> list) {
        elements_.reserve(list.size());
        for (const auto &element : list) {
            elements_.emplace(element);
        }
    }

    Matrix &operator=(const Matrix &other) {
        elements_ = other.elements_;
        return *this;
    }
    Matrix &operator=(Matrix &&other) noexcept {
        elements_ = std::move(other.elements_);
        return *this;
    }

    bool operator==(const Matrix &other) const { return elements_ == other.elements_; }
    bool operator!=(const Matrix &other) const { return !(*this == other); }

    NextLayout operator[](size_t idx) { return NextLayout(idx, elements_); }
    const NextLayout operator[](size_t idx) const { return NextLayout(idx, elements_); }

    /// Return an input iterator to the beginning
    /// @return Input iterator to the begining
    auto begin() const noexcept { return Iterator(elements_.cbegin()); }

    /// Return an input iterator to the end
    /// @return Input iterator to the end
    auto end() const noexcept { return Iterator(elements_.cend()); }

    /// Get real count of elements in matrix
    /// @return count of elements
    size_t size() const noexcept { return elements_.size(); }

    /// Clears the mapped matrix.
    void clear() noexcept { elements_.clear(); }
};

// ****************************
// * Struct Matrix::TupleHash *
// ****************************
// Boost for combining hash values
// https://www.boost.org/doc/libs/1_38_0/doc/html/hash/reference.html#boost.hash_combine
template <typename T, T DefaultValue, size_t Dimension>
class Matrix<T, DefaultValue, Dimension>::TupleHash {
    inline size_t get_hash(size_t &seed, size_t value) const {
        return std::hash<size_t>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <std::size_t... Indices>
    void combine_hash(size_t &seed, const TupleKey &tuple, std::index_sequence<Indices...>) const {
        using swallow = size_t[];
        (void)swallow{(seed ^= get_hash(seed, std::get<Indices>(tuple)))...};
    }

  public:
    size_t operator()(const TupleKey &tuple) const {
        size_t seed = 0;
        combine_hash(seed, tuple, std::make_index_sequence<Dimension>{});

        return seed;
    }
};

// **************************
// * Class Matrix::Iterator *
// **************************
template <typename T, T DefaultValue, size_t Dimension>
class Matrix<T, DefaultValue, Dimension>::Iterator {
    using MapIteratorType = typename Contanter::const_iterator;
    MapIteratorType map_iterator_;

    inline auto get_value() const {
        return std::tuple_cat((*map_iterator_).first, std::tie((*map_iterator_).second));
    }

  public:
    using value_type =
        decltype(std::tuple_cat((*map_iterator_).first, std::tie((*map_iterator_).second)));
    using iterator_category = std::input_iterator_tag;

    explicit Iterator(MapIteratorType map_iterator) : map_iterator_(map_iterator) {}

    Iterator &operator++() {
        map_iterator_++;
        return *this;
    }
    Iterator operator++(int) {
        Iterator retval = *this;
        ++(*this);
        return retval;
    }
    bool operator==(Iterator other) const { return map_iterator_ == other.map_iterator_; }
    bool operator!=(Iterator other) const { return !(*this == other); }

    value_type operator*() const { return get_value(); }
};

// ************************
// * Class Matrix::Layout *
// ************************
template <typename T, T DefaultValue, size_t Dimension>
template <size_t N, typename... Types>
class Matrix<T, DefaultValue, Dimension>::Layout {
    using NextLayout = Layout<N - 1, Types..., size_t>;

    const Matrix::Contanter &elements_;
    std::tuple<Types...> tuple_;

  public:
    Layout(std::tuple<Types...> tuple, const Matrix::Contanter &elements)
        : elements_{elements}, tuple_{tuple} {}

    NextLayout operator[](size_t idx) {
        return NextLayout(std::tuple_cat(tuple_, std::tie(idx)), elements_);
    }
    const NextLayout operator[](size_t idx) const {
        return NextLayout(std::tuple_cat(tuple_, std::tie(idx)), elements_);
    }
};

// *************************************
// * Class Matrix::Layout<0, Types...> *
// *************************************
template <typename T, T DefaultValue, size_t Dimension>
template <typename... Types>
class Matrix<T, DefaultValue, Dimension>::Layout<0, Types...> {
    std::tuple<Types...> tuple_;
    const Matrix::Contanter &elements_;
    const T default_{DefaultValue};

  public:
    Layout(std::tuple<Types...> tuple, const Matrix::Contanter &elements)
        : tuple_{tuple}, elements_{elements} {}

    auto &operator=(const T &value) { // NOLINT
        if (value != DefaultValue) {
            const_cast<Matrix::Contanter &>(elements_)[tuple_] = value;
        } else {
            auto iter = elements_.find(tuple_);
            if (iter != elements_.cend()) {
                const_cast<Matrix::Contanter &>(elements_).erase(iter);
            }
        }
        return *this;
    }

    operator const T &() const noexcept { // NOLINT
        auto iter = elements_.find(tuple_);
        return (iter != elements_.cend()) ? iter->second : default_;
    }
};

} // namespace otus

#endif // OTUS_MATRIX_HPP