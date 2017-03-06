#pragma once

#include <vector>

#include <SDL.h>
#include <glm/vec3.hpp>

#include <scene_renderer.hpp>
#include <index_buffer.hpp>
#include <shader_program.hpp>
#include <vertex_buffer.hpp>
#include <vertex_source.hpp>

class PathEditingRenderer :
    public SceneRenderer
{
public:
    PathEditingRenderer(Scene* scene);

    virtual void Render() override;

private:
    std::unique_ptr<VertexBuffer> m_sphere_vb;
    std::unique_ptr<VertexSource> m_sphere_vs;
    std::unique_ptr<IndexBuffer> m_sphere_ib;
    std::unique_ptr<ShaderProgram> m_sphere_sp;
    
    void DrawSphere(
        const glm::mat4& view_projection_matrix,
        float radius,
        const glm::vec3& position,
        const glm::vec4& color
    );
};
