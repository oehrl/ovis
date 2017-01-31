file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_library(Engine ${HEADER_FILES} ${SOURCE_FILES})
