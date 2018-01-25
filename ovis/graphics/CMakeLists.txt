file(GLOB OVIS_GRAPHICS_HEADER_FILES "*.hpp")
file(GLOB OVIS_GRAPHICS_SOURCE_FILES "*.cpp")

add_library(OvisGraphics ${OVIS_GRAPHICS_HEADER_FILES}
                         ${OVIS_GRAPHICS_SOURCE_FILES})

