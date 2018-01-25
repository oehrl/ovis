file(GLOB ROCKREAPER3D_HEADER_FILES "*.hpp")
file(GLOB ROCKREAPER3D_SOURCE_FILES "*.cpp")

set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(RockReaper3D ${ROCKREAPER3D_HEADER_FILES}
                            ${ROCKREAPER3D_SOURCE_FILES})

target_link_libraries(RockReaper3D OvisEngine)