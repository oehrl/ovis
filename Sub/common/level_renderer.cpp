#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <log.hpp>

#include "application.hpp"
#include "level_renderer.hpp"
#include "range.hpp"
#include "graphics_device.hpp"
#include "scene.hpp"
#include "camera_controller.hpp"
#include "perlin_noise.hpp"

namespace
{
    const std::size_t CAVE_LENGTH = 512;
    const std::size_t VERTICES_PER_RING = 8;
    const std::size_t CAVE_VERTEX_COUNT = CAVE_LENGTH * VERTICES_PER_RING;
    const std::size_t INDICES_PER_RING_SEGMENT = VERTICES_PER_RING * 2 * 3;
    const std::size_t INDEX_COUNT = INDICES_PER_RING_SEGMENT * (CAVE_LENGTH - 1);
    const float CAVE_RADIUS = 5.0f;
    const float CAVE_CIRCUMFERENCE = 2.0 * M_PI * CAVE_RADIUS;
    const std::size_t TEXTURE_REPEAT_COUNT = 2;
    
    const Uint32 MAX_VERTICES = 65536;
    const Uint32 MAX_INDICES = 200000;
    
    static_assert(CAVE_VERTEX_COUNT <= std::numeric_limits<Uint16>::max() + 1, "too many vertices");
    
    std::vector<glm::vec3> CalculatePlaneNormals(const std::vector<glm::vec3>& positions)
    {
        SDL_assert(positions.size() > 1);
        const auto count = positions.size();
        std::vector<glm::vec3> normals(count);
        
        normals[0] = glm::normalize(positions[1] - positions[0]);
        for (auto i : IRange<std::size_t>(1, count - 1))
        {
            normals[i] = glm::normalize(positions[i + 1] - positions[i - 1]);
        }
        normals[count - 1] = glm::normalize(positions[count - 1] - positions[count - 2]);
        return normals;
    }
    
    void NormalizeNormals(std::vector<glm::vec3>* normals)
    {
        SDL_assert(normals != nullptr);
        for (auto& normal : *normals)
        {
            normal = glm::normalize(normal);
        }
    }
}

LevelRenderer::LevelRenderer(Scene* scene) :
    SceneRenderer(scene, "LevelRenderer"),
    m_width(100),
    m_height(100),
    m_index_count(0)
{
    RenderBefore("GuiRenderer");

    m_shader_program = LoadShaderProgram(app()->GetFullResourcePath("heightmap"));
    m_cave_shader = LoadShaderProgram(app()->GetFullResourcePath("cave"));
    m_sand_texture = LoadTexture(app()->GetFullResourcePath("sand2.bmp"));
    m_rock_texture = LoadTexture(app()->GetFullResourcePath("rock2.bmp"));
    m_rock_normal_map = LoadTexture(app()->GetFullResourcePath("rock_norm.bmp"));
    
    {
        // std::vector<glm::vec3> positions(CAVE_LENGTH);
        // std::vector<glm::vec3> directions(CAVE_LENGTH);
        
        // const double MAX_Z = CAVE_LENGTH * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING);
        // for (auto i : IndexRange(positions))
        // {
        //     double z = i.index() * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING);
        //     i.value().x = 1000.0 * OctavePerlin(0.123, 0.123, 0.8 * z / MAX_Z, 3, 0.5);
        //     i.value().y = CAVE_RADIUS + CAVE_RADIUS * OctavePerlin(0.123, 0.123, z / (CAVE_LENGTH * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING)), 1, 0.5);
        //     i.value().z = z;
        // }
        
        // directions.front() = positions[1] - positions[0];
        // directions.back() = positions[CAVE_LENGTH - 1] - positions[CAVE_LENGTH - 2];
        // for (auto i : IRange<std::size_t>(1, CAVE_LENGTH - 1))
        // {
        //     directions[i] = positions[i + 1] - positions[i - 1];
        // }
        
        // std::vector<CaveVertex> vertices(CAVE_VERTEX_COUNT);
        // static const glm::vec3 offset(0.3234, 0.234234, 0.234234);
        // static const float frequency = 0.142;
        // static const float amplitude = CAVE_RADIUS * 0.5f;
        
        // const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
        // for (auto i : IRange(CAVE_LENGTH))
        // {
        //     const auto base_index = i * VERTICES_PER_RING;
        //     const glm::vec3 right = glm::normalize(glm::cross(directions[i], UP));
        //     const glm::vec3 up = glm::normalize(glm::cross(directions[i], right));
            
        //     for (auto j : IRange(VERTICES_PER_RING))
        //     {
        //         const float angle = (2.0f * glm::pi<float>() * j) / VERTICES_PER_RING;
        //         const auto index = base_index + j;
                
        //         const glm::vec3 base_normal = std::sin(angle) * right + std::cos(angle) * up;
        //         const glm::vec3 base_position = positions[i] + CAVE_RADIUS * base_normal;
        //         const glm::vec3 noise_coord = offset + frequency * base_position;
        //         const float noise = OctavePerlin(noise_coord.x, noise_coord.y, noise_coord.z, 20, 0.5f);
                
        //         vertices[index].normal = -base_normal;
        //         vertices[index].position = base_position + base_normal * amplitude * noise;
        //         vertices[index].texture_coords.x = TEXTURE_REPEAT_COUNT * static_cast<float>(j) / VERTICES_PER_RING;
        //         vertices[index].texture_coords.y = TEXTURE_REPEAT_COUNT * positions[i].z / CAVE_CIRCUMFERENCE;
        //     }
        // }
        
        // auto get_vertex = [&vertices](std::size_t ring_index, std::size_t vertex_index) -> CaveVertex&
        // {
        //     return vertices[ring_index * VERTICES_PER_RING + (vertex_index % VERTICES_PER_RING)];
        // };
        
        // auto get_position = [vertices,get_vertex](std::size_t ring_index, std::size_t vertex_index)
        // {
        //     return get_vertex(ring_index, vertex_index).position;
        // };
        
        // for (auto i : IRange<size_t>(1, CAVE_LENGTH - 1))
        // {
        //     for (auto j : IRange(VERTICES_PER_RING))
        //     {
        //         const glm::vec3 va =
        //             glm::normalize(
        //                 get_position(i - 1, j) -
        //                 get_position(i + 1, j)
        //             );
                
        //         const glm::vec3 vb =
        //             glm::normalize(
        //                 get_position(i, j - 1) -
        //                 get_position(i, j + 1)
        //             );
        //         get_vertex(i,j).normal = -glm::normalize(glm::cross(va, vb));
        //         //get_vertex(i,j).tangent = -glm::cross(get_vertex(i,j).normal, directions[i]);
        //     }
        // }
        
        VertexBufferDescription vb_desc;
        vb_desc.vertex_size_in_bytes = sizeof(CaveVertex);
        vb_desc.size_in_bytes = sizeof(CaveVertex) * MAX_VERTICES;
        m_cave_vb = std::make_unique<VertexBuffer>(graphics_device(), vb_desc, nullptr);
    }
    
    {
        VertexSourceDescription vs_desc;
        vs_desc.shader_program = m_cave_shader.get();
        vs_desc.vertex_buffers = { m_cave_vb.get() };
        vs_desc.vertex_attributes =
        {
            { "Position",  VertexAttributeType::FLOAT32_VECTOR3, 0,  0 },
            { "Normal",    VertexAttributeType::FLOAT32_VECTOR3, 0, 12 },
            //{ "Tangent",   VertexAttributeType::FLOAT32_VECTOR3, 0, 24 },
            { "TexCoords", VertexAttributeType::FLOAT32_VECTOR2, 0, 24 },
        };
        m_cave_vs = std::make_unique<VertexSource>(graphics_device(), vs_desc);
    }
    
    {
        std::vector<Uint16> indices(INDEX_COUNT);
        
        auto get_index = [this](std::size_t ring_index, std::size_t vertex_index)
        {
            return ring_index * VERTICES_PER_RING + (vertex_index % VERTICES_PER_RING);
        };
        
        for (std::size_t ring : IRange(CAVE_LENGTH - 1))
        {
            for (std::size_t segment : IRange(VERTICES_PER_RING))
            {
                std::size_t base =
                    ring * INDICES_PER_RING_SEGMENT +
                    segment * 6;
                
                indices[base + 0] = get_index(ring,     segment    );
                indices[base + 1] = get_index(ring + 1, segment    );
                indices[base + 2] = get_index(ring + 1, segment + 1);
                
                indices[base + 3] = get_index(ring,     segment    );
                indices[base + 4] = get_index(ring + 1, segment + 1);
                indices[base + 5] = get_index(ring,     segment + 1);
            }
        }
        
        IndexBufferDescription ib_desc;
        ib_desc.index_format = IndexFormat::UINT16;
        ib_desc.size_in_bytes = sizeof(Uint16) * MAX_INDICES;
        m_cave_ib = std::make_unique<IndexBuffer>(graphics_device(), ib_desc, indices.data());
    }
    
    {
        SDL_Surface* surface = SDL_LoadBMP(app()->GetFullResourcePath("heightmap.bmp").c_str());
        SDL_LockSurface(surface);
        
        m_width = surface->w;
        m_height = surface->h;
        const unsigned char* pixels = reinterpret_cast<const unsigned char*>(surface->pixels);
        
        
        std::size_t vertex_count = m_width * m_height;
        std::vector<Vertex> vertices(vertex_count);
        
        SDL_assert(vertex_count <= 65536);
        
        auto GetVertex = [&vertices,this](std::size_t x, std::size_t y) -> Vertex&
        {
            return vertices[y * m_width + x];
        };
        
        for (std::size_t y = 0; y < m_height; ++y)
        {
            for (std::size_t x = 0; x < m_width; ++x)
            {
                float nx = x / 256.0f;
                float ny = y / 256.0f;
                float nz = 23.234724234f;
            
                Vertex& vertex = GetVertex(x, y);
                vertex.x = x;
                vertex.y = y;
                vertex.height = 0.0 * OctavePerlin(nx, ny, nz, 6, 0.5f);
                
                
//                vertex.position.x = x - 0.5f * (m_width  - 1);
//                vertex.position.y = 0.7f * pixels[y * surface->pitch + x * 3];
//                vertex.position.z = y - 0.5f * (m_height - 1);
            }
        }
        std::cout << std::endl;
        
        SDL_UnlockSurface(surface);
        SDL_FreeSurface(surface);
        
        
        
        auto GetVertexPosition = [&vertices,GetVertex,this](std::size_t x, std::size_t y) -> glm::vec3
        {
            return
            {
                x * (1.0f / 255.0f) * 100.0f - 50.0f,
                GetVertex(x, y).height,
                y * (1.0f / 255.0f) * 100.0f - 50.0f
            };
        };
        
        for (std::size_t y = 0; y < m_height; ++y)
        {
            for (std::size_t x = 0; x < m_width; ++x)
            {
                glm::vec3 va;
                glm::vec3 vb;
                
                if (x == 0)
                {
                    va = glm::normalize(GetVertexPosition(x, y) - GetVertexPosition(x + 1, y));
                } else if (x == m_width - 1)
                {
                    va = glm::normalize(GetVertexPosition(x - 1, y) - GetVertexPosition(x, y));
                } else
                {
                    va = glm::normalize(GetVertexPosition(x - 1, y) - GetVertexPosition(x + 1, y));
                }
                
                if (y == 0)
                {
                    vb = glm::normalize(GetVertexPosition(x, y) - GetVertexPosition(x, y + 1));
                } else if (y == m_height - 1)
                {
                    vb = glm::normalize(GetVertexPosition(x, y - 1) - GetVertexPosition(x, y));
                } else
                {
                    vb = glm::normalize(GetVertexPosition(x, y - 1) - GetVertexPosition(x, y + 1));
                }
                
                GetVertex(x, y).normal = glm::normalize(glm::cross(vb, va));
            }
        }
        
        VertexBufferDescription vb_desc;
        vb_desc.vertex_size_in_bytes = sizeof(Vertex);
        vb_desc.size_in_bytes = sizeof(Vertex) * vertex_count;
        m_vertex_buffer = std::make_unique<VertexBuffer>(graphics_device(), vb_desc, vertices.data());
    }
    
    {
        VertexSourceDescription vs_desc;
        vs_desc.shader_program = m_shader_program.get();
        vs_desc.vertex_buffers = { m_vertex_buffer.get() };
        vs_desc.vertex_attributes =
        {
            { "Height",      VertexAttributeType::FLOAT32,            0,  0 },
            { "Normal",      VertexAttributeType::FLOAT32_VECTOR3,    0,  4 },
            { "Coordinates", VertexAttributeType::UINT8_NORM_VECTOR2, 0, 16 },
        };
        m_vertex_source = std::make_unique<VertexSource>(graphics_device(), vs_desc);
    }
    
    {
        std::size_t triangle_count = (m_width - 1) * (m_height -1) * 2;
        std::size_t index_count = triangle_count * 3;
        std::vector<Uint16> indices(index_count);
        
        auto get_index = [this](std::size_t x, std::size_t y)
        {
            return y * m_width + x;
        };
        
        for (std::size_t y = 0; y < m_height - 1; ++y)
        {
            for (std::size_t x = 0; x < m_width - 1; ++x)
            {
                std::size_t base = (y * (m_width - 1) + x) * 6;
                
                indices[base + 0] = get_index(x    , y    );
                indices[base + 1] = get_index(x + 1, y + 1);
                indices[base + 2] = get_index(x    , y + 1);
                
                indices[base + 3] = get_index(x    , y    );
                indices[base + 4] = get_index(x + 1, y    );
                indices[base + 5] = get_index(x + 1, y + 1);
            }
        }
        
        IndexBufferDescription ib_desc;
        ib_desc.index_format = IndexFormat::UINT16;
        ib_desc.size_in_bytes = sizeof(Uint16) * index_count;
        m_index_buffer = std::make_unique<IndexBuffer>(graphics_device(), ib_desc, indices.data());
    }
}

void LevelRenderer::Render()
{    
    CameraController* camera_controller = static_cast<CameraController*>(scene()->GetController("CameraController"));
    
    if (!camera_controller)
    {
        return;
    }
    
    {
        m_shader_program->SetUniform("WorldViewMatrix", camera_controller->view_matrix());
        m_shader_program->SetUniform("ProjectionMatrix", camera_controller->projection_matrix());
        m_shader_program->SetTexture("SandTextureSampler", m_sand_texture.get());
        m_shader_program->SetUniform("Dimensions", 1000.0f);
        m_shader_program->SetUniform("TextureOffset", glm::vec2(0, 0));
        m_shader_program->SetUniform("TextureTiling", glm::ivec2(50, 50));
        
        DrawItem draw_item;
        draw_item.shader_program = m_shader_program.get();
        draw_item.vertex_source = m_vertex_source.get();
        draw_item.index_buffer = m_index_buffer.get();
        draw_item.primitive_topology = PrimitiveTopology::TRIANGLE_LIST;
        draw_item.start = 0;
        draw_item.count = (m_width - 1) * (m_height - 1) * 2 * 3;
        draw_item.depth_buffer_state = DepthBufferState::ENABLED;
        draw_item.alpha_blending_enabled = false;
        //graphics_device()->Draw(draw_item);
    }

    {
        m_cave_shader->SetUniform("WorldViewMatrix", camera_controller->view_matrix());
        m_cave_shader->SetUniform("ProjectionMatrix", camera_controller->projection_matrix());
        m_cave_shader->SetUniform("LightPosition", camera_controller->position());
        m_cave_shader->SetUniform("LightDirection", camera_controller->forward());
        m_cave_shader->SetTexture("AlbedoSampler", m_rock_texture.get());
        //m_cave_shader->SetTexture("NormalSampler", m_rock_normal_map.get());
        
        DrawItem draw_item;
        draw_item.shader_program = m_cave_shader.get();
        draw_item.vertex_source = m_cave_vs.get();
        draw_item.index_buffer = m_cave_ib.get();
        draw_item.primitive_topology = PrimitiveTopology::TRIANGLE_LIST;
        draw_item.start = 0;
        draw_item.count = m_index_count;
        draw_item.depth_buffer_state = DepthBufferState::ENABLED;
        draw_item.alpha_blending_enabled = false;
        graphics_device()->Draw(draw_item);
    }
}

void LevelRenderer::SetLevelDescription(const LevelDescription& level_description)
{
    const std::size_t num_rings = level_description.path.size();
    const std::size_t num_distinct_positions_per_ring = level_description.num_segments;
    const std::size_t num_vertices_per_ring = 1 + num_distinct_positions_per_ring;
    const std::size_t num_vertices = num_rings * num_vertices_per_ring;
    
    const std::vector<glm::vec3>& path = level_description.path;
    //const std::vector<float>& vertex_offsets = level_description.vertex_offsets;
    float radius = level_description.radius;
    const std::vector<glm::vec3> normals = CalculatePlaneNormals(path);
    
    //SDL_assert(vertex_offsets.size() % path.size() == 0);
    SDL_assert(num_vertices < std::numeric_limits<Uint16>::max() + 1);
    SDL_assert(num_vertices < MAX_VERTICES);
    
    std::vector<CaveVertex> vertices(num_vertices);
        
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    float current_distance = 0.0f;
    for (auto i : IRange(num_rings))
    {
        if (i > 0)
        {
            current_distance += glm::length(path[i] - path[i - 1]);
        }
        
        const glm::vec3 right = glm::normalize(glm::cross(normals[i], UP));
        const glm::vec3 up = glm::normalize(glm::cross(normals[i], right));
        
        for (auto j : IRange(num_vertices_per_ring))
        {
            const float percent = static_cast<float>(j) / num_distinct_positions_per_ring;
            const float angle = percent * glm::two_pi<float>();
            const auto vertex_index = i * num_vertices_per_ring + j;
            const auto vertex_offset_index =
                i * num_distinct_positions_per_ring +
                j % num_distinct_positions_per_ring;
            
            const glm::vec3 inverse_normal = std::sin(angle) * right + std::cos(angle) * up;
            const glm::vec3 position = path[i] + inverse_normal * radius;
            
            vertices[vertex_index].position = position;
            vertices[vertex_index].normal = -inverse_normal;
            vertices[vertex_index].texture_coords =
                static_cast<float>(level_description.texture_tiling) *
                glm::vec2(
                    percent,
                    current_distance * level_description.texture_scaling
                );
            
            //LogD(glm::to_string(position));
        }
    }
    
    m_cave_vb->Write(0, sizeof(CaveVertex) * vertices.size(), vertices.data());
    
    const std::size_t num_indices_per_ring = num_distinct_positions_per_ring * 2 * 3;
    const std::size_t num_indices = num_indices_per_ring * (num_rings - 1);
    SDL_assert(num_indices < MAX_INDICES);
    
    std::vector<Uint16> indices(num_indices);
    
    auto get_index = [num_vertices_per_ring](std::size_t ring_index, std::size_t vertex_index)
    {
        return ring_index * num_vertices_per_ring + vertex_index;
    };
    
    for (std::size_t ring : IRange(num_rings - 1))
    {
        for (std::size_t segment : IRange(num_distinct_positions_per_ring))
        {
            std::size_t base =
                6 * (ring * num_distinct_positions_per_ring + segment);
            
            indices[base + 0] = get_index(ring,     segment    );
            indices[base + 1] = get_index(ring + 1, segment    );
            indices[base + 2] = get_index(ring + 1, segment + 1);
            
            indices[base + 3] = get_index(ring,     segment    );
            indices[base + 4] = get_index(ring + 1, segment + 1);
            indices[base + 5] = get_index(ring,     segment + 1);
        }
    }
    m_cave_ib->Write(0, 2 * indices.size(), indices.data());
    m_index_count = num_indices;
}
