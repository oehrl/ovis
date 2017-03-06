#pragma once

class EditorScene;

class EditorMode
{
public:
    EditorMode(EditorScene* editor_scene);
    virtual ~EditorMode() = default;
    
    inline EditorScene* editor_scene() const
    {
        return m_editor_scene;
    }
    
private:
    EditorScene* m_editor_scene;
};
