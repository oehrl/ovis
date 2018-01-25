cmake_minimum_required(VERSION 3.7)

include(CMakeToolsHelpers OPTIONAL)

# include_directories(extern/freetype/include)
# add_subdirectory(extern)

# include_directories(extern/SDL/include)
# include_directories(extern/turbobadger/src/tb)
include_directories(extern/glm/include)

add_definitions("-DSDL_ASSERT_LEVEL=3")

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fno-rtti")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -Wall -Wextra -Wpedantic -Werror -s USE_SDL=2 -g4 -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=1 -s TOTAL_MEMORY=536870912")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fno-rtti")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /W4 /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4221")
endif()

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

include_directories(.)
add_subdirectory(ovis)
add_subdirectory(rockreaper3d)