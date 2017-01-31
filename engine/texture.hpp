#pragma once

#include "gl.hpp"
#include "graphics_resource.hpp"

class Texture :
    public GraphicsResource
{
    friend class GraphicsDevice;
    friend class UniformBuffer;
    
public:
    Texture(GraphicsDevice* graphics_device);
    virtual ~Texture() override;
    
protected:
    inline GLuint texture_name() const
    {
        return m_texture_name;
    }
    
private:
    GLuint m_texture_name;
    
    virtual void Bind(Uint32 texture_unit) = 0;
};
