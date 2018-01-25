file(GLOB OVIS_ENGINE_HEADER_FILES "*.hpp")
file(GLOB OVIS_ENGINE_SOURCE_FILES "*.cpp")

add_library(OvisEngine ${OVIS_ENGINE_HEADER_FILES} ${OVIS_ENGINE_SOURCE_FILES})

# OvisCore
target_include_directories(OvisEngine PUBLIC OvisCore)
target_link_libraries(OvisEngine OvisCore)

# OvisGraphics
target_include_directories(OvisEngine PUBLIC OvisGraphics)
target_link_libraries(OvisEngine OvisGraphics)

# OvisScene
target_include_directories(OvisEngine PUBLIC OvisScene)
target_link_libraries(OvisEngine OvisScene)