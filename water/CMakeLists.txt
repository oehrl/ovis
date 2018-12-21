file(GLOB WATER_HEADER_FILES "*.hpp")
file(GLOB WATER_SOURCE_FILES "*.cpp")

if (OVIS_EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

add_executable(Water ${WATER_HEADER_FILES}
                            ${WATER_SOURCE_FILES})

target_link_libraries(Water OvisEngine)