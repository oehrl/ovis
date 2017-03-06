//
//  path_editing_mode.cpp
//  Project
//
//  Created by Simon Oehrl on 19/02/17.
//
//

#include "path_editing_mode.hpp"
#include "editor_scene.hpp"

PathEditingMode::PathEditingMode(EditorScene* editor_scene) :
    EditorMode(editor_scene),
    m_controller(editor_scene),
    m_renderer(editor_scene)
{

}
