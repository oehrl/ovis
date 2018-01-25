#pragma once

#define MAKE_NON_COPYABLE(class_name)     \
  class_name(const class_name&) = delete; \
  class_name& operator=(const class_name&) = delete

#define MAKE_NON_MOVABLE(class_name)       \
  class_name(const class_name&&) = delete; \
  class_name& operator=(const class_name&&) = delete

#define MAKE_NON_COPY_OR_MOVABLE(class_name) \
  MAKE_NON_COPYABLE(class_name);             \
  MAKE_NON_MOVABLE(class_name)
