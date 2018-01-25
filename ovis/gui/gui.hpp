#pragma once

#include <memory>

#include "application.hpp"

class Gui
{
    friend class GuiContoller;
    friend class GuiRenderer;
    friend class Application;
    
public:
    Gui();
    ~Gui();

    inline Uint32 event_type() const
    {
        return m_event_type;
    }
    
    void LoadSkin(const std::string& skin_filename);
    void LoadFont(const std::string& font_filename, const std::string& font_name);
    void SetDefaultFont(const std::string& font_name, float font_size_in_dp);

private:
    class InternalRenderer;
    
    std::unique_ptr<InternalRenderer> m_tb_renderer;
    Uint32 m_event_type;
};

inline Gui* gui()
{
    return app()->gui();
}
