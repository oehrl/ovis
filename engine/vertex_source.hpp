#pragma once

#include <vector>

#include "graphics_resource.hpp"

class VertexBuffer;
class ShaderProgram;

enum VertexAttributeType : Uint32
{
    FLOAT32,
    FLOAT32_VECTOR2,
    FLOAT32_VECTOR3,
    FLOAT32_VECTOR4,
    
    INT8,
    INT8_VECTOR2,
    INT8_VECTOR3,
    INT8_VECTOR4,
    
    UINT8,
    UINT8_VECTOR2,
    UINT8_VECTOR3,
    UINT8_VECTOR4,
    
    INT8_NORM,
    INT8_NORM_VECTOR2,
    INT8_NORM_VECTOR3,
    INT8_NORM_VECTOR4,
    
    UINT8_NORM,
    UINT8_NORM_VECTOR2,
    UINT8_NORM_VECTOR3,
    UINT8_NORM_VECTOR4,
    
    INT16,
    INT16_VECTOR2,
    INT16_VECTOR3,
    INT16_VECTOR4,
    
    UINT16,
    UINT16_VECTOR2,
    UINT16_VECTOR3,
    UINT16_VECTOR4,
    
    INT16_NORM,
    INT16_NORM_VECTOR2,
    INT16_NORM_VECTOR3,
    INT16_NORM_VECTOR4,
    
    UINT16_NORM,
    UINT16_NORM_VECTOR2,
    UINT16_NORM_VECTOR3,
    UINT16_NORM_VECTOR4,
};

struct VertexAttributeDescrition
{
    std::string name;
    VertexAttributeType type;
    Uint32 buffer_index;
    Uint32 offset_in_bytes;
};

struct VertexSourceDescription
{
    ShaderProgram* shader_program;
    std::vector<VertexBuffer*> vertex_buffers;
    std::vector<VertexAttributeDescrition> vertex_attributes;
};

class VertexSource final :
    public GraphicsResource
{
    friend class GraphicsDevice;

    struct AttributeGlDesc
    {
        const GLvoid* offset;
        GLsizei stride;
        GLuint array_buffer;
        GLenum type;
        GLint size;
        GLboolean normalized;
        GLboolean enabled;
    };

public:
    VertexSource
    (
        GraphicsDevice* graphics_device,
        const VertexSourceDescription& description
    );
    
    virtual ~VertexSource() override;
    
private:
    VertexSourceDescription m_description;
    std::vector<AttributeGlDesc> m_attribute_gl_descriptions;
    
    void Bind();
};
