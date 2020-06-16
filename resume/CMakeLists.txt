if (OVIS_EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_executable(resume main.cpp)

target_link_libraries(resume OvisEngine)