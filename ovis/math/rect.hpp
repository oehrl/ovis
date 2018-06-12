#pragma once

namespace ovis {

template <typename T>
struct Rect {
  T top;
  T left;
  T width;
  T height;
};

}  // namespace ovis