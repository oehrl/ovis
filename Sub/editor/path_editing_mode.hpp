#pragma once

#include "editor_mode.hpp"
#include "path_editing_controller.hpp"
#include "path_editing_renderer.hpp"

class PathEditingMode :
    public EditorMode
{
public:
    PathEditingMode(EditorScene* scene);

private:
    PathEditingController m_controller;
    PathEditingRenderer m_renderer;
};
