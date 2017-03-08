#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include "scene_renderer.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include "vertex_source.hpp"
#include "shader_program.hpp"
#include "texture2d.hpp"

struct LevelDescription
{
    std::vector<glm::vec3> path;
    unsigned int num_segments;
    float radius;
    unsigned int texture_tiling;
    float texture_scaling;
};

class LevelRenderer final :
    public SceneRenderer
{
public:
    LevelRenderer(Scene* scene);
    
    virtual void Render() override;
    
    void SetLevelDescription(const LevelDescription& level_description);

    void LoadLevel(const std::string& filename);
    void SaveLevel(const std::string& filename) const;
    
private:
    struct Vertex
    {
        float height;
        glm::vec3 normal;
        Uint8 x;
        Uint8 y;
    };
    
    struct CaveVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coords;
    };

    LevelDescription m_level_description;
    
    std::size_t m_width;
    std::size_t m_height;

    std::unique_ptr<IndexBuffer> m_index_buffer;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<VertexSource> m_vertex_source;
    std::unique_ptr<ShaderProgram> m_shader_program;
    std::unique_ptr<Texture2D> m_sand_texture;

    std::unique_ptr<IndexBuffer> m_cave_ib;
    std::unique_ptr<VertexBuffer> m_cave_vb;
    std::unique_ptr<VertexSource> m_cave_vs;
    std::unique_ptr<ShaderProgram> m_cave_shader;
    std::unique_ptr<Texture2D> m_rock_texture;
    std::unique_ptr<Texture2D> m_rock_normal_map;
    std::size_t m_index_count;
};
