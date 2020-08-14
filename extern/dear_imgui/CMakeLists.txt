add_library(
  DearImGui

  imgui.cpp
  imgui_demo.cpp
  imgui_draw.cpp
  imgui_widgets.cpp
)

target_include_directories(
  DearImGui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
