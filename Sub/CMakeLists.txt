file(GLOB SOURCE_FILES "src/*.cpp")
file(GLOB HEADER_FILES "src/*.hpp")

add_executable(Sub ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(Sub Engine SDL2-static freetype TurboBadgerLib ${OPENGL_LIBRARIES})
