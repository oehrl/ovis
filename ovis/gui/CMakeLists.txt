file(GLOB OVIS_GUI_HEADER_FILES "*.hpp")
file(GLOB OVIS_GUI_SOURCE_FILES "*.cpp")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/extern/imgui")
set(IMGUI_HEADER_FILES ${IMGUI_DIR}/imconfig.h
                       ${IMGUI_DIR}/imgui_internal.h
                       ${IMGUI_DIR}/imgui.h
                       ${IMGUI_DIR}/stb_rect_pack.h
                       ${IMGUI_DIR}/stb_textedit.h
                       ${IMGUI_DIR}/stb_truetype.h)
set(IMGUI_SOURCE_FILES ${IMGUI_DIR}/imgui_draw.cpp
                       ${IMGUI_DIR}/imgui.cpp
                       ${IMGUI_DIR}/imgui_demo.cpp)

add_library(OvisGui ${OVIS_GUI_HEADER_FILES}
                    ${OVIS_GUI_SOURCE_FILES}
                    ${IMGUI_HEADER_FILES}
                    ${IMGUI_SOURCE_FILES})

target_link_libraries(OvisGui OvisScene OvisGraphics)

# FreeType
# target_include_directories(OvisGui PUBLIC freetype)
# target_link_libraries(OvisGui freetype)