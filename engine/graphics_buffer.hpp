#pragma once

#include "graphics_resource.hpp"
#include "gl.hpp"

class GraphicsBuffer :
    public GraphicsResource
{
protected:
    GraphicsBuffer(GraphicsDevice* graphics_device);
    virtual ~GraphicsBuffer() override;

    inline GLuint buffer_name() const { return m_buffer_name; }
    
private:
    GLuint m_buffer_name;
};
