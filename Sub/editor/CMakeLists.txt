file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_executable(Sub-Editor ${COMMON_HEADER_FILES} ${COMMON_SOURCE_FILES} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(Sub-Editor Engine SDL2-static freetype TurboBadgerLib ${OPENGL_LIBRARIES})
