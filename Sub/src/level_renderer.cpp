#include <glm/gtc/matrix_transform.hpp>

#include "application.hpp"
#include "level_renderer.hpp"
#include "range.hpp"
#include "graphics_device.hpp"
#include "scene.hpp"
#include "camera_controller.hpp"
#include "perlin_noise.hpp"

#include <iostream>

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
    
    static_assert(CAVE_VERTEX_COUNT <= std::numeric_limits<Uint16>::max() + 1, "too many vertices");
}

LevelRenderer::LevelRenderer(Scene* scene) :
    SceneRenderer(scene, "LevelRenderer"),
    m_width(100),
    m_height(100)
{
    RenderBefore("GuiRenderer");

    m_shader_program = LoadShaderProgram(app()->GetFullResourcePath("heightmap"));
    m_cave_shader = LoadShaderProgram(app()->GetFullResourcePath("cave"));
    m_sand_texture = LoadTexture(app()->GetFullResourcePath("sand2.bmp"));
    m_rock_texture = LoadTexture(app()->GetFullResourcePath("rock2.bmp"));
    m_rock_normal_map = LoadTexture(app()->GetFullResourcePath("rock_norm.bmp"));
    
    {
        std::vector<glm::vec3> positions(CAVE_LENGTH);
        std::vector<glm::vec3> directions(CAVE_LENGTH);
        
        const double MAX_Z = CAVE_LENGTH * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING);
        for (auto i : IndexRange(positions))
        {
            double z = i.index() * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING);
            i.value().x = 1000.0 * OctavePerlin(0.123, 0.123, 0.8 * z / MAX_Z, 3, 0.5);
            i.value().y = CAVE_RADIUS + CAVE_RADIUS * OctavePerlin(0.123, 0.123, z / (CAVE_LENGTH * (CAVE_CIRCUMFERENCE / VERTICES_PER_RING)), 1, 0.5);
            i.value().z = z;
        }
        
        directions.front() = positions[1] - positions[0];
        directions.back() = positions[CAVE_LENGTH - 1] - positions[CAVE_LENGTH - 2];
        for (auto i : IRange<std::size_t>(1, CAVE_LENGTH - 1))
        {
            directions[i] = positions[i + 1] - positions[i - 1];
        }
        
        std::vector<CaveVertex> vertices(CAVE_VERTEX_COUNT);
        static const glm::vec3 offset(0.3234, 0.234234, 0.234234);
        static const float frequency = 0.142;
        static const float amplitude = CAVE_RADIUS * 0.5f;
        
        const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
        for (auto i : IRange(CAVE_LENGTH))
        {
            const auto base_index = i * VERTICES_PER_RING;
            const glm::vec3 right = glm::normalize(glm::cross(directions[i], UP));
            const glm::vec3 up = glm::normalize(glm::cross(directions[i], right));
            
            for (auto j : IRange(VERTICES_PER_RING))
            {
                const float angle = (2.0f * glm::pi<float>() * j) / VERTICES_PER_RING;
                const auto index = base_index + j;
                
                const glm::vec3 base_normal = std::sin(angle) * right + std::cos(angle) * up;
                const glm::vec3 base_position = positions[i] + CAVE_RADIUS * base_normal;
                const glm::vec3 noise_coord = offset + frequency * base_position;
                const float noise = OctavePerlin(noise_coord.x, noise_coord.y, noise_coord.z, 20, 0.5f);
                
                vertices[index].normal = -base_normal;
                vertices[index].position = base_position + base_normal * amplitude * noise;
                vertices[index].texture_coords.x = TEXTURE_REPEAT_COUNT * static_cast<float>(j) / VERTICES_PER_RING;
                vertices[index].texture_coords.y = TEXTURE_REPEAT_COUNT * positions[i].z / CAVE_CIRCUMFERENCE;
            }
        }
        
        auto get_vertex = [&vertices](std::size_t ring_index, std::size_t vertex_index) -> CaveVertex&
        {
            return vertices[ring_index * VERTICES_PER_RING + (vertex_index % VERTICES_PER_RING)];
        };
        
        auto get_position = [vertices,get_vertex](std::size_t ring_index, std::size_t vertex_index)
        {
            return get_vertex(ring_index, vertex_index).position;
        };
        
        for (auto i : IRange<size_t>(1, CAVE_LENGTH - 1))
        {
            for (auto j : IRange(VERTICES_PER_RING))
            {
                const glm::vec3 va =
                    glm::normalize(
                        get_position(i - 1, j) -
                        get_position(i + 1, j)
                    );
                
                const glm::vec3 vb =
                    glm::normalize(
                        get_position(i, j - 1) -
                        get_position(i, j + 1)
                    );
                get_vertex(i,j).normal = -glm::normalize(glm::cross(va, vb));
                //get_vertex(i,j).tangent = -glm::cross(get_vertex(i,j).normal, directions[i]);
            }
        }
        
        VertexBufferDescription vb_desc;
        vb_desc.vertex_size_in_bytes = sizeof(CaveVertex);
        vb_desc.size_in_bytes = sizeof(CaveVertex) * CAVE_VERTEX_COUNT;
        m_cave_vb = std::make_unique<VertexBuffer>(graphics_device(), vb_desc, vertices.data());
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
        ib_desc.size_in_bytes = sizeof(Uint16) * INDEX_COUNT;
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
    int window_width;
    int window_height;
    SDL_GetWindowSize(app()->window(), &window_width, &window_height);
    
    glm::mat4 projection_matrix = glm::perspectiveFovLH(
        0.5f * glm::pi<float>(),
        static_cast<float>(window_width),
        static_cast<float>(window_height),
        0.1f,
        500.0f
    );
    
    CameraController* camera_controller = dynamic_cast<CameraController*>(scene()->GetController("CameraController"));
    
    glm::mat4 rotation_matrix_pitch = glm::rotate(glm::mat4(), camera_controller->pitch(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotation_matrix_yaw = glm::rotate(glm::mat4(), camera_controller->yaw(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translation_matrix = glm::translate(glm::mat4(), -camera_controller->position());
    
    {
        m_shader_program->SetUniform("WorldViewMatrix", rotation_matrix_pitch * rotation_matrix_yaw * translation_matrix);
        m_shader_program->SetUniform("ProjectionMatrix", projection_matrix);
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
        m_cave_shader->SetUniform("WorldViewMatrix", rotation_matrix_pitch * rotation_matrix_yaw * translation_matrix);
        m_cave_shader->SetUniform("ProjectionMatrix", projection_matrix);
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
        draw_item.count = INDEX_COUNT;
        draw_item.depth_buffer_state = DepthBufferState::ENABLED;
        draw_item.alpha_blending_enabled = false;
        graphics_device()->Draw(draw_item);
    }
}
