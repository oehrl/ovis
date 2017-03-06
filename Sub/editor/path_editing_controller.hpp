#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <scene_controller.hpp>

class PathEditingController :
    public SceneController
{
public:
    PathEditingController(Scene* scene);
    
    virtual void Update(Uint32 delta_time) override;
    
    inline const std::vector<glm::vec3>& path() const
    {
        return m_path;
    }
    
    inline void SetPath(const std::vector<glm::vec3>& path)
    {
        m_path = path;
    }
    
    inline int selection() const
    {
        return m_selection;
    }
    
    inline void SetSelection(int selection)
    {
        m_selection = selection;
    }
    
    void RemoveSelected();

private:
    std::vector<glm::vec3> m_path;
    int m_selection;
};
