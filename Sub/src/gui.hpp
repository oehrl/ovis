#pragma once

#include <memory>

class Gui
{
    friend class GuiContoller;
    friend class GuiRenderer;

    friend void InitGui(const std::string&);
    friend void ReleaseGui();
    friend Gui* gui();
    
public:
    inline Uint32 event_type() const
    {
        return m_event_type;
    }
    
    void LoadFont(const std::string& font_filename, const std::string& font_name);
    void SetDefaultFont(const std::string& font_name, float font_size_in_dp);

private:
    class InternalRenderer;
    
    static std::unique_ptr<Gui> s_gui;
    
    std::unique_ptr<InternalRenderer> m_tb_renderer;
    Uint32 m_event_type;
    
    Gui(const std::string& skin_filename);
};

void InitGui(const std::string& skin_filename);
void ReleaseGui();

inline Gui* gui()
{
    return Gui::s_gui.get();
}
