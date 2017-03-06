file(GLOB COMMON_SOURCE_FILES "common/*.cpp")
file(GLOB COMMON_HEADER_FILES "common/*.hpp")

include_directories(common)

add_subdirectory(game)
add_subdirectory(editor)
