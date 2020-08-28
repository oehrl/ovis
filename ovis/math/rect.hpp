#pragma once

namespace ovis {

template <typename T>
struct Rect {
  T top;
  T left;
  T width;
  T height;

  bool operator==(const Rect& other) const {
    return top == other.top && left == other.left && width == other.width && height == other.height;
  }

  bool operator!=(const Rect& other) const { return !(*this == other); }
};

}  // namespace ovis