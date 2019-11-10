file(GLOB RESOURCE_COMPILER_HEADER_FILES "*.hpp")
file(GLOB RESOURCE_COMPILER_SOURCE_FILES "*.cpp")

add_executable(OvisResourceCompiler
  ${RESOURCE_COMPILER_HEADER_FILES}
  ${RESOURCE_COMPILER_SOURCE_FILES}
)

set_target_properties(
    OvisResourceCompiler
    PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
)

target_link_libraries(OvisResourceCompiler
  PRIVATE stb_image
  PRIVATE rapidjson
)
