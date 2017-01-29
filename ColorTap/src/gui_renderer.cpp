////#include <renderers/tb_renderer_batcher.h>
//
//#include "gui_renderer.hpp"
//
//class GuiRenderer::TBRenderer :
//	public tb::TBRendererBatcher
//{
//public:
//	virtual tb::TBBitmap* CreateBitmap(int width, int height, tb::uint32* data) override;
//	virtual void RenderBatch(Batch* batch) override;
//	virtual void SetClipRect(const tb::TBRect& rect) override;
//
//private:
//};
//
//tb::TBBitmap* GuiRenderer::TBRenderer::CreateBitmap(int width, int height, tb::uint32* data)
//{
//	return nullptr;
//}
//
//void GuiRenderer::TBRenderer::RenderBatch(Batch* batch)
//{
//	
//}
//
//GuiRenderer::GuiRenderer(Scene* scene) :
//    SceneRenderer(scene, "GuiRenderer"),
//    m_tb_renderer(std::make_unique<GuiRenderer::TBRenderer>())
//{
//}
//
