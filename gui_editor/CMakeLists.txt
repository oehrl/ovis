file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_executable(OvisGuiEditor ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(OvisGuiEditor Engine SDL2-static freetype TurboBadgerLib ${OPENGL_LIBRARIES})
