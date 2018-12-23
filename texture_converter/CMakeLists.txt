file(GLOB TEXTURE_CONVERTER_HEADER_FILES "*.hpp")
file(GLOB TEXTURE_CONVERTER_SOURCE_FILES "*.cpp")

add_executable(OvisTextureConverter
  ${TEXTURE_CONVERTER_HEADER_FILES}
  ${TEXTURE_CONVERTER_SOURCE_FILES}
)
target_link_libraries(OvisTextureConverter
  PRIVATE stb_image
  PRIVATE rapidjson
)
