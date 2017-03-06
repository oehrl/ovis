#include <application.hpp>
#include <gui_scene.hpp>
#include "file_dialogs.hpp"
#include <gui.hpp>

namespace
{
    class FileDialogScene :
        public GuiScene
    {
    public:
        FileDialogScene(
            const std::string& scene_name,
            const std::string& gui_layout_filename,
            const std::function<void(std::string)>& success_function
        ) :
            GuiScene(scene_name, gui_layout_filename, false),
            m_success_function(success_function)
        {
        }
        
    protected:
        const std::function<void(std::string)>& success_function() const
        {
            return m_success_function;
        }
        
    private:
        std::function<void(std::string)> m_success_function;
    };
    
    std::unique_ptr<FileDialogScene> dialog_scene;

    class SaveFileDialogScene :
        public FileDialogScene
    {
    public:
        SaveFileDialogScene(const std::function<void(std::string)>& success_function) :
            FileDialogScene(
                "SaveFileDialogScene",
                "$resource_path$/save_file_dialog.tb.txt",
                success_function
            )
        {
            const auto edt_filename = gui_controller()->GetWidgetById(TBIDC("edt-filename"));
            edt_filename->SetText(app()->data_path().c_str());
        }
        
        virtual bool AfterEventProcessing(const SDL_Event& event)
        {
            if (event.type == gui()->event_type())
            {
                const tb::TBWidgetEvent* tb_event = reinterpret_cast<const tb::TBWidgetEvent*>(event.user.data1);
                const auto target_id = tb_event->target->GetID();
                if (tb_event->type == tb::EVENT_TYPE_CLICK)
                {
                    if (target_id == TBIDC("btn-save"))
                    {
                        const auto function = success_function();
                        const auto edt_filename = gui_controller()->GetWidgetById(TBIDC("edt-filename"));
                        const std::string filename = edt_filename->GetText().CStr();
                        function(filename);
                        app()->PopScene();
                        dialog_scene.reset();
                        return true;
                    }
                    else if (target_id == TBIDC("btn-cancel"))
                    {
                        app()->PopScene();
                        dialog_scene.reset();
                        return true;
                    }
                }
            }
            
            return false;
        }
        
    private:
        std::function<void(std::string)> m_success_function;
    };
}

void ShowSaveFileDialog(
    const std::function<void(std::string)>& success_function
)
{
    SDL_assert(dialog_scene == nullptr);
    dialog_scene = std::make_unique<SaveFileDialogScene>(success_function);
    app()->PushScene(dialog_scene.get());
}
