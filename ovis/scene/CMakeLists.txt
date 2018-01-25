file(GLOB OVIS_SCENE_HEADER_FILES "*.hpp")
file(GLOB OVIS_SCENE_SOURCE_FILES "*.cpp")

add_library(OvisScene ${OVIS_SCENE_HEADER_FILES} ${OVIS_SCENE_SOURCE_FILES})

################################################################################
# Dependencies 
################################################################################

# OvisGraphics
target_include_directories(OvisScene PUBLIC OvisGraphics)
target_link_libraries(OvisScene OvisGraphics)

# OvisMath
target_include_directories(OvisScene PUBLIC OvisMath)
target_link_libraries(OvisScene OvisMath)
