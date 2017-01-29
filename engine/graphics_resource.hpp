#pragma once

#include "SDL.h"

class GraphicsDevice;

class GraphicsResource
{
public:
    inline GraphicsDevice* graphics_device() const { return m_graphics_device; }
    virtual ~GraphicsResource();

protected:
    GraphicsResource(GraphicsDevice* graphics_device);

private:
    GraphicsDevice* m_graphics_device;
};
