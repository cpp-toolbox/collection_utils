#ifndef COLLECTION_UTILS_HPP
#define COLLECTION_UTILS_HPP

#include <functional>
#include <vector>

namespace collection_utils {

template <typename T> std::vector<T> join_vectors(const std::vector<T> &v1, const std::vector<T> &v2) {
    std::vector<T> result;
    result.reserve(v1.size() + v2.size());
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
    return result;
}

template <typename T, typename Func> void for_each_in_vector(std::vector<T> &vec, Func func) {
    for (auto &elem : vec) {
        func(elem);
    }
}

template <typename T, typename Func> void for_each_in_vector(const std::vector<T> &vec, Func func) {
    for (const auto &elem : vec) {
        func(elem);
    }
}

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

template <typename T, typename Func> auto map_vector(const std::vector<T> &vec, Func func) {
    using U = decltype(func(std::declval<const T &>()));
    std::vector<U> result;
    result.reserve(vec.size());
    for (const auto &elem : vec) {
        result.push_back(func(elem));
    }
    return result;
}

}; // namespace collection_utils

#endif // COLLECTION_UTILS_HPP
