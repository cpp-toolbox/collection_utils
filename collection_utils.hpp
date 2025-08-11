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

template <typename T, typename U>
std::vector<U> map_vector(const std::vector<T> &vec, const std::function<U(T)> &func) {
    std::vector<U> result;
    result.reserve(vec.size());
    for (const auto &elem : vec) {
        result.push_back(func(elem));
    }
    return result;
}

}; // namespace collection_utils

#endif // COLLECTION_UTILS_HPP
