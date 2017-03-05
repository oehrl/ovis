#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics_device.hpp>
#include <log.hpp>
#include <range.hpp>
#include <scene.hpp>

#include "camera_controller.hpp"
#include "path_editing_renderer.hpp"
#include "path_editing_controller.hpp"

namespace
{
    const std::size_t VERTEX_COUNT = 512;
    
    struct SeedData
    {
        glm::u8vec3 cell;
        Uint8 source : 3;
        Uint8 padding0 : 1;
        Uint8 target : 3;
        Uint8 padding1 : 1;
    };
    static_assert(sizeof(SeedData) == sizeof(Uint32), "");
    
    const glm::ivec3 CORNERS[] =
    {
        { 0, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 0, 1, 1 },
        { 1, 0, 0 },
        { 1, 0, 1 },
        { 1, 1, 0 },
        { 1, 1, 1 }
    };
    
    inline SeedData GetSeedData(Uint32 seed)
    {
        return *reinterpret_cast<SeedData*>(&seed);
    }
    
    inline glm::vec3 GetCorner(const glm::u8vec3& cell, int corner_index)
    {
        return glm::vec3(glm::ivec3(cell) + CORNERS[corner_index]);
    }
    
    std::vector<glm::vec3> CalculateSpherePositions(int segments, int rings)
    {
        SDL_assert(segments >= 3);
        SDL_assert(rings >= 3);
        
        std::vector<glm::vec3> positions;
        positions.reserve((rings - 1) * segments + 2);
        positions.emplace_back(0.0f, 1.0f, 0.0f);
        
        for (auto r : IRange(1, rings))
        {
            const float theta = (glm::pi<float>() * r) / rings;
            const float sin_theta = std::sin(theta);
            const float cos_theta = std::cos(theta);
            
            for (auto s : IRange(segments))
            {
                const float phi = (glm::two_pi<float>() * s) / segments;
                const float sin_phi = std::sin(phi);
                const float cos_phi = std::cos(phi);
            
                positions.emplace_back
                (
                    sin_theta * sin_phi,
                    cos_theta,
                    sin_theta * cos_phi
                );
            }
        }
        
        positions.emplace_back(0.0f, -1.0f, 0.0f);
        return positions;
    }
    
    std::vector<Uint16> CalculateSphereIndices(int segments, int rings)
    {
        SDL_assert(segments >= 3);
        SDL_assert(rings >= 3);
        
        std::vector<Uint16> indices;
        indices.reserve(3 * (2 * (rings - 2) * segments + 2 * segments));
        
        Uint16 current_ring_offset = 0;
        Uint16 next_ring_offset = 1;
        for (auto s : IRange(segments))
        {
            const Uint16 t = (s + 1) % segments;
            
            indices.emplace_back(current_ring_offset);
            indices.emplace_back(next_ring_offset + s);
            indices.emplace_back(next_ring_offset + t);
        }
        current_ring_offset = next_ring_offset;
        next_ring_offset = current_ring_offset + segments;
        
        for (auto r : IRange(1, rings - 1))
        {
            for (auto s : IRange(segments))
            {
                const Uint16 t = (s + 1) % segments;
                
                indices.emplace_back(current_ring_offset + s);
                indices.emplace_back(current_ring_offset + t);
                indices.emplace_back(next_ring_offset    + s);
                
                indices.emplace_back(next_ring_offset    + s);
                indices.emplace_back(next_ring_offset    + t);
                indices.emplace_back(current_ring_offset + t);
            }
            
            current_ring_offset = next_ring_offset;
            next_ring_offset = current_ring_offset + segments;
        }
        
        for (auto s : IRange(segments))
        {
            const Uint16 t = (s + 1) % segments;
            
            indices.emplace_back(current_ring_offset + s);
            indices.emplace_back(next_ring_offset);
            indices.emplace_back(current_ring_offset + t);
        }
        
        return indices;
    }
    
    const int SPHERE_SEGMENTS = 32;
    const int SPHERE_RINGS = 16;
    const int SPHERE_INDICES = 3 * (2 * (SPHERE_RINGS - 2) * SPHERE_SEGMENTS + 2 * SPHERE_SEGMENTS);
}

PathEditingRenderer::PathEditingRenderer(Scene* scene) :
    SceneRenderer(scene, "PathEditingRenderer")
{
    m_sphere_sp = LoadShaderProgram("$resource_path$/default");
    
    {
        auto vertices = CalculateSpherePositions(SPHERE_SEGMENTS, SPHERE_RINGS);
        VertexBufferDescription vb_desc;
        vb_desc.vertex_size_in_bytes = sizeof(glm::vec3);
        vb_desc.size_in_bytes = vb_desc.vertex_size_in_bytes * vertices.size();
        m_sphere_vb = std::make_unique<VertexBuffer>(graphics_device(), vb_desc, vertices.data());
    }
    
    {
        VertexSourceDescription vs_desc;
        vs_desc.shader_program = m_sphere_sp.get();
        vs_desc.vertex_buffers = { m_sphere_vb.get() };
        vs_desc.vertex_attributes =
        {
            { "Position", VertexAttributeType::FLOAT32_VECTOR3, 0, 0 }
        };
        m_sphere_vs = std::make_unique<VertexSource>(graphics_device(), vs_desc);
    }
    
    {
        auto indices = CalculateSphereIndices(SPHERE_SEGMENTS, SPHERE_RINGS);
        IndexBufferDescription ib_desc;
        ib_desc.index_format = IndexFormat::UINT16;
        ib_desc.size_in_bytes = indices.size() * 2;
        m_sphere_ib = std::make_unique<IndexBuffer>(graphics_device(), ib_desc, indices.data());
    }
    
    RenderAfter("LevelRenderer");
}

void PathEditingRenderer::Render()
{
    CameraController* camera_controller = scene()->GetController<CameraController>("CameraController");
    const glm::mat4 view_projection_matrix = camera_controller->view_projection_matrix();
    
    auto path_editing_controller = scene()->GetController<PathEditingController>("PathEditingController");
    
    for (const auto& point : IndexRange<int>(path_editing_controller->path()))
    {
        glm::vec4 sphere_color;
        if (point.index() == path_editing_controller->selection())
        {
            sphere_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            sphere_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    
        DrawSphere(
            view_projection_matrix,
            5.0f,
            point.value(),
            sphere_color
        );
    }
}

void PathEditingRenderer::DrawSphere(
    const glm::mat4& view_projection_matrix,
    float radius,
    const glm::vec3& position,
    const glm::vec4& color
)
{
    const glm::mat4 world_matrix =
        glm::scale(
            glm::translate(
                glm::mat4(1.0f),
                position
            ),
            glm::vec3(radius)
        );
    m_sphere_sp->SetUniform("WorldViewProjection", view_projection_matrix * world_matrix);
    m_sphere_sp->SetUniform("Color", color);
    
    DrawItem draw_item;
    draw_item.shader_program = m_sphere_sp.get();
    draw_item.vertex_source = m_sphere_vs.get();
    draw_item.index_buffer = m_sphere_ib.get();
    draw_item.primitive_topology = PrimitiveTopology::TRIANGLE_LIST;
    draw_item.start = 0;
    draw_item.count = SPHERE_INDICES;
    draw_item.depth_buffer_state = DepthBufferState::DISABLED;
    draw_item.alpha_blending_enabled = false;
    graphics_device()->Draw(draw_item);
}
