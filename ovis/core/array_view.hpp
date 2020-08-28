#pragma once

#include <array>
#include <cstddef>
#include <vector>

namespace ovis {

template <typename T>
class array_view {
 public:
  inline array_view() : data_(nullptr), size_(0) {}
  inline array_view(T* data, std::size_t size) : data_(data), size_(size) {}
  inline array_view(const std::vector<T>& vector) : data_(vector.data()), size_(vector.size()) {}
  template <std::size_t size>
  inline array_view(const std::array<T, size>& array) : data_(array.data()), size_(size) {}

  array_view(const array_view<T>&) = default;
  array_view<T>& operator=(const array_view<T>&) = default;

  array_view(array_view<T>&&) = default;
  array_view<T>& operator=(array_view<T>&&) = default;

  inline T* data() { return data_; }
  inline std::size_t size() { return size_; }

  inline T* begin() { return data_; }
  inline T* end() { return data_ + size_; }

  inline T& operator[](std::size_t index) { return data_[index]; }
  inline T operator[](std::size_t index) const { return data_[index]; }

 private:
  T* data_;
  std::size_t size_;
};

}  // namespace ovis