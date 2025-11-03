#ifndef COLLECTION_UTILS_HPP
#define COLLECTION_UTILS_HPP

#include <functional>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>

namespace collection_utils {

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

// startfold vectors

/**
 * @brief Check if a value exists in a vector.
 *
 * @tparam T Type of elements in the vector.
 * @param vec The vector to search within.
 * @param value The value to search for.
 * @return true if the value exists in the vector, false otherwise.
 */
template <typename T> bool contains(const std::vector<T> &vec, const T &value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

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
// endfold

// startfold unordered maps

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
 * @brief Apply a function to each key of a modifiable unordered_map.
 *
 * @tparam Key Type of the keys in the unordered_map.
 * @tparam Value Type of the values in the unordered_map.
 * @tparam Func Type of the function to apply. Must be callable with Key&.
 * @param map The unordered_map whose keys will be processed.
 * @param func Function to apply to each key.
 */
template <typename Key, typename Value, typename Func>
void for_each_key_in_map(std::unordered_map<Key, Value> &map, Func func) {
    for (auto &pair : map) {
        func(pair.first);
    }
}

/**
 * @brief Apply a function to each key-value pair of a modifiable unordered_map.
 *
 * @tparam Key Type of the keys in the unordered_map.
 * @tparam Value Type of the values in the unordered_map.
 * @tparam Func Type of the function to apply. Must be callable with (Key&, Value&).
 * @param map The unordered_map to process.
 * @param func Function to apply to each key-value pair.
 */
template <typename Key, typename Value, typename Func>
void for_each_pair_in_map(std::unordered_map<Key, Value> &map, Func func) {
    for (auto &pair : map) {
        func(pair.first, pair.second);
    }
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
 * @brief Keep only the entries in an unordered_map whose keys are in a specified set.
 *
 * @tparam K Type of keys in the map and the set.
 * @tparam V Type of values in the map.
 * @param input_map The unordered_map to filter.
 * @param key_set The set of keys to keep.
 * @return std::unordered_map<K, V> A new unordered_map containing only entries with keys in key_set.
 */
template <typename K, typename V>
std::unordered_map<K, V> filter_map_by_key_set(const std::unordered_map<K, V> &input_map,
                                               const std::unordered_set<K> &key_set) {
    return filter_map_by_keys(input_map, [&](const K &key) { return key_set.find(key) != key_set.end(); });
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

/**
 * @brief Build an unordered_map from a vector of objects, using a member or attribute as the key.
 *
 * @tparam Key Type of the key to use in the map.
 * @tparam Value Type of the objects in the vector.
 * @tparam KeyFunc Callable type that takes a const Value& and returns a Key.
 * @param vec Vector of objects to convert to a map.
 * @param key_func Function that extracts the key from a Value.
 * @return std::unordered_map<Key, Value> The resulting map.
 *
 * @note If multiple objects in the vector produce the same key, only the first one encountered will be inserted into
 * the map. Subsequent objects with duplicate keys will be ignored. Therefore if there is such duplicate data then
 * information will be lost
 */
template <typename Key, typename Value, typename KeyFunc>
std::unordered_map<Key, Value> build_map_from_vector(const std::vector<Value> &vec, KeyFunc key_func) {
    std::unordered_map<Key, Value> map;
    for (const auto &item : vec) {
        map.emplace(key_func(item), item); // first occurrence wins
    }
    return map;
}

/**
 * @brief Combine two unordered_maps with the same keyset using a provided binary function.
 *
 * @tparam Key Type of the keys in the maps.
 * @tparam Value1 Type of the values in the first map.
 * @tparam Value2 Type of the values in the second map.
 * @tparam ResultType Type of the values in the resulting map.
 * @tparam Func Callable type that takes (const Value1&, const Value2&) and returns ResultType.
 * @param map1 First unordered_map.
 * @param map2 Second unordered_map.
 * @param func Function to combine corresponding values from map1 and map2.
 * @return std::unordered_map<Key, ResultType> Resulting map after applying func to each value pair.
 *
 * @throws std::invalid_argument if the keysets of the two maps do not match.
 */
template <typename Key, typename Value1, typename Value2,
          typename ResultType = decltype(std::declval<Value1>() + std::declval<Value2>()), typename Func>
std::unordered_map<Key, ResultType> combine_maps(const std::unordered_map<Key, Value1> &map1,
                                                 const std::unordered_map<Key, Value2> &map2, Func func) {
    if (map1.size() != map2.size()) {
        throw std::invalid_argument("Maps do not have the same number of elements");
    }

    std::unordered_map<Key, ResultType> result;

    for (const auto &pair : map1) {
        const Key &key = pair.first;
        auto it2 = map2.find(key);
        if (it2 == map2.end()) {
            throw std::invalid_argument("Keysets of the maps do not match");
        }
        result[key] = func(pair.second, it2->second);
    }

    return result;
}

/**
 * @brief Extracts all values from an unordered_map into a vector.
 *
 * @tparam Key Type of the keys in the map.
 * @tparam Value Type of the values in the map.
 * @param map The unordered_map to extract values from.
 * @return std::vector<Value> A vector containing all values from the map.
 *
 * @note The order of values in the resulting vector is unspecified because
 *       unordered_map does not guarantee ordering.
 */
template <typename Key, typename Value> std::vector<Value> values(const std::unordered_map<Key, Value> &map) {
    std::vector<Value> values;
    values.reserve(map.size()); // optional: reserve space for efficiency
    for (const auto &pair : map) {
        values.push_back(pair.second);
    }
    return values;
}

/**
 * @brief Extracts all keys from an unordered_map into a vector.
 *
 * @tparam Key Type of the keys in the map.
 * @tparam Value Type of the values in the map.
 * @param map The unordered_map to extract keys from.
 * @return std::vector<Key> A vector containing all keys from the map.
 *
 * @note The order of keys in the resulting vector is unspecified because
 *       unordered_map does not guarantee ordering.
 */
template <typename Key, typename Value> std::vector<Key> keys(const std::unordered_map<Key, Value> &map) {
    std::vector<Key> keys;
    keys.reserve(map.size()); // optional: reserve space for efficiency
    for (const auto &pair : map) {
        keys.push_back(pair.first);
    }
    return keys;
}
// endfold

// startfold sets

/**
 * @brief Converts a vector into a set, removing duplicates.
 *
 * @tparam T Type of the elements in the vector.
 * @param vec The vector to convert.
 * @return std::set<T> A set containing all unique elements from the vector.
 *
 * @note The elements in the set are sorted because std::set maintains order.
 */
template <typename T> std::set<T> to_set(const std::vector<T> &vec) { return std::set<T>(vec.begin(), vec.end()); }

/**
 * @brief Converts a vector into an unordered_set, removing duplicates.
 *
 * @tparam T Type of the elements in the vector.
 * @param vec The vector to convert.
 * @return std::unordered_set<T> An unordered_set containing all unique elements from the vector.
 *
 * @note The elements in the unordered_set are not sorted.
 */
template <typename T> std::unordered_set<T> to_unordered_set(const std::vector<T> &vec) {
    return std::unordered_set<T>(vec.begin(), vec.end());
}

/**
 * @brief Computes the intersection of two sets.
 *
 * @tparam SetType Type of the input sets (std::set or std::unordered_set).
 * @param a First set.
 * @param b Second set.
 * @return SetType A new set containing elements present in both input sets.
 *
 * @note For std::set, the result will be ordered. For std::unordered_set, order is unspecified.
 */
template <typename SetType> SetType set_intersection(const SetType &a, const SetType &b) {
    SetType result;

    if constexpr (std::is_same_v<SetType, std::set<typename SetType::value_type>>) {
        // use std::set_intersection for ordered sets
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(result, result.begin()));
    } else {
        // Fallback for unordered_set or other sets
        for (const auto &elem : a) {
            if (b.count(elem)) {
                result.insert(elem);
            }
        }
    }

    return result;
}
// endfold

}; // namespace collection_utils

#endif // COLLECTION_UTILS_HPP
