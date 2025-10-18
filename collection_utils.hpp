#ifndef COLLECTION_UTILS_HPP
#define COLLECTION_UTILS_HPP

#include <functional>
#include <vector>
#include <algorithm>

namespace collection_utils {

/**
 * @brief Concatenate two vectors into a single vector.
 *
 * @tparam T Type of elements in the vectors.
 * @param v1 First vector.
 * @param v2 Second vector.
 * @return A new vector containing all elements from v1 followed by all elements from v2.
 */
template <typename T> std::vector<T> join_vectors(const std::vector<T> &v1, const std::vector<T> &v2) {
    std::vector<T> result;
    result.reserve(v1.size() + v2.size());
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
    return result;
}

/**
 * @brief Apply a function to each element of a modifiable vector.
 *
 * @tparam T Type of elements in the vector.
 * @tparam Func Type of the function to apply. Must be callable with T&.
 * @param vec Vector whose elements will be processed.
 * @param func Function to apply to each element.
 */
template <typename T, typename Func> void for_each_in_vector(std::vector<T> &vec, Func func) {
    for (auto &elem : vec) {
        func(elem);
    }
}

/**
 * @brief Check if any element in the container evaluates to true.
 *
 * Elements are cast to bool before evaluation.
 *
 * @tparam Container Type of container supporting begin() and end().
 * @param c Container to check.
 * @return true if at least one element is truthy, false otherwise.
 */
template <typename Container> bool any_of(const Container &c) {
    return std::any_of(c.begin(), c.end(), [](auto v) { return static_cast<bool>(v); });
}

/**
 * @brief Check if all elements in the container evaluate to true.
 *
 * Elements are cast to bool before evaluation.
 *
 * @tparam Container Type of container supporting begin() and end().
 * @param c Container to check.
 * @return true if all elements are truthy, false otherwise.
 */
template <typename Container> bool all_of(const Container &c) {
    return std::all_of(c.begin(), c.end(), [](auto v) { return static_cast<bool>(v); });
}

/**
 * @brief Apply a function to each element of a read-only vector.
 *
 * @tparam T Type of elements in the vector.
 * @tparam Func Type of the function to apply. Must be callable with const T&.
 * @param vec Vector whose elements will be processed.
 * @param func Function to apply to each element.
 */
template <typename T, typename Func> void for_each_in_vector(const std::vector<T> &vec, Func func) {
    for (const auto &elem : vec) {
        func(elem);
    }
}

/**
 * @brief Concatenate a list of vectors into a single vector.
 *
 * @tparam T Type of elements in the vectors.
 * @param vectors A vector of vectors to join.
 * @return A new vector containing all elements from all input vectors in order.
 */
template <typename T> std::vector<T> join_all_vectors(const std::vector<std::vector<T>> &vectors) {

    size_t total_size = 0;
    for (const auto &v : vectors) {
        total_size += v.size();
    }

    std::vector<T> result;
    result.reserve(total_size);

    for (const auto &v : vectors) {
        result.insert(result.end(), v.begin(), v.end());
    }

    return result;
}

/**
 * @brief Transform a vector by applying a function to each element.
 *
 * @tparam T Type of input elements.
 * @tparam Func Type of the function to apply. Must be callable with const T&.
 * @param vec Input vector.
 * @param func Function to apply to each element.
 * @return A new vector where each element is the result of applying func to the corresponding input element.
 */
template <typename T, typename Func> auto map_vector(const std::vector<T> &vec, Func func) {
    using U = decltype(func(std::declval<const T &>()));
    std::vector<U> result;
    result.reserve(vec.size());
    for (const auto &elem : vec) {
        result.push_back(func(elem));
    }
    return result;
}

/**
 * @brief Transform the values of an unordered_map by applying a function to each value.
 *
 * @tparam K Type of the keys in the map.
 * @tparam V Type of the input values in the map.
 * @tparam Func Type of the function to apply. Must be callable with const V&.
 * @param input_map Input unordered_map.
 * @param func Function to apply to each value.
 * @return A new unordered_map with the same keys and transformed values.
 */
template <typename K, typename V, typename Func> auto map_values(const std::unordered_map<K, V> &input_map, Func func) {
    using U = decltype(func(std::declval<const V &>()));
    std::unordered_map<K, U> result;
    result.reserve(input_map.size());
    for (const auto &[key, value] : input_map) {
        // NOTE: move is fine here because func(value) is a temporary that's about to go out of scope
        // the reason why we prefer move is that if the result is not copy constructible we get errors from this
        result.emplace(key, std::move(func(value)));
    }
    return result;
}

/**
 * @brief Filter an unordered_map based on a predicate applied to key-value pairs.
 *
 * @tparam K Type of keys in the map.
 * @tparam V Type of values in the map.
 * @tparam Pred Type of the predicate. Must be callable with (const K&, const V&).
 * @param input_map Input unordered_map to filter.
 * @param pred Predicate function that returns true to keep an element, false to remove it.
 * @return A new unordered_map containing only the key-value pairs for which pred(key, value) is true.
 */
template <typename K, typename V, typename Pred>
std::unordered_map<K, V> filter_map(const std::unordered_map<K, V> &input_map, Pred pred) {
    std::unordered_map<K, V> result;
    result.reserve(input_map.size());

    for (const auto &[key, value] : input_map) {
        if (pred(key, value)) {
            result.emplace(key, value);
        }
    }

    return result;
}

/**
 * @brief Filter an unordered_map based on a predicate applied to its keys.
 *
 * @tparam K Type of keys in the map.
 * @tparam V Type of values in the map.
 * @tparam Pred Type of the predicate. Must be callable with (const K&).
 * @param input_map Input unordered_map to filter.
 * @param pred Predicate function that returns true to keep an element, false to remove it.
 * @return A new unordered_map containing only the key-value pairs for which pred(key) is true.
 */
template <typename K, typename V, typename Pred>
std::unordered_map<K, V> filter_map_by_keys(const std::unordered_map<K, V> &input_map, Pred pred) {
    std::unordered_map<K, V> result;
    result.reserve(input_map.size());

    for (const auto &[key, value] : input_map) {
        if (pred(key)) {
            result.emplace(key, value);
        }
    }

    return result;
}

/**
 * @brief Filter an unordered_map based on a predicate applied to its values.
 *
 * @tparam K Type of keys in the map.
 * @tparam V Type of values in the map.
 * @tparam Pred Type of the predicate. Must be callable with (const V&).
 * @param input_map Input unordered_map to filter.
 * @param pred Predicate function that returns true to keep an element, false to remove it.
 * @return A new unordered_map containing only the key-value pairs for which pred(value) is true.
 */
template <typename K, typename V, typename Pred>
std::unordered_map<K, V> filter_map_by_values(const std::unordered_map<K, V> &input_map, Pred pred) {
    std::unordered_map<K, V> result;
    result.reserve(input_map.size());

    for (const auto &[key, value] : input_map) {
        if (pred(value)) {
            result.emplace(key, value);
        }
    }

    return result;
}

}; // namespace collection_utils

#endif // COLLECTION_UTILS_HPP
