#include "ui.hpp"
#include "texturecreator.hpp"
#include "canvas.hpp"

/*
	// Up section
	topSection[0] = new Button(layer, Exit, kReturn, Point( 44, 1 ), "Exit", true); // (Button) Exit
	topSection[1] = new Button(layer, Import, kReturn, Point( 51, 1 ), "Import", true); // (Button) Import
	topSection[2] = new Button(layer, Export, kReturn, Point( 60, 1 ), "Export", true); // (Button) Export
	topSection[3] = new IntInputField(layer, UpdateBrush, 1, 78, "16", Point( 69, 1 ), "CanvasX=", true); // (Int field) Canvas size X
	topSection[4] = new IntInputField(layer, UpdateBrush, 1, 33, "16", Point( 80, 1 ), "CanvasY=", true); // (Int field) Canvas size Y
	topSection[5] = new Button(layer, ResizeCanvas, kReturn, Point( 91, 1 ), "Confirm", true); // (Button) Confirm canvas size
*/

TextureCreatorUI::TopSection::TopSection(KTech::Engine& p_engine, KTech::ID<KTech::UI> p_parentUI, KTech::Point p_pos)
	: KTech::Widget(p_engine, p_parentUI, p_pos),
	m_exit(p_engine, p_parentUI, TextureCreator::ExitGame, KTech::Keys::return_, KTech::Point(p_pos.x, p_pos.y), "Exit", true),
	m_import(p_engine, p_parentUI, std::bind(&Canvas::Import, &TextureCreator::ui.m_canvas, "import.ktecht"), KTech::Keys::return_, KTech::Point(p_pos.x + 7, p_pos.y), "Import", true),
	m_export(p_engine, p_parentUI, std::bind(&Canvas::Export, &TextureCreator::ui.m_canvas, "export.ktecht"), KTech::Keys::return_, KTech::Point(p_pos.x + 16, p_pos.y), "Export", true),
	m_canvasSizeX(p_engine, p_parentUI, nullptr, 1, 78, std::to_string(DefaultValues::canvasSize.x), KTech::Point(p_pos.x + 25, p_pos.y), "CanvasX=", true),
	m_canvasSizeY(p_engine, p_parentUI, nullptr, 1, 78, std::to_string(DefaultValues::canvasSize.y), KTech::Point(p_pos.x + 36, p_pos.y), "CanvasY=", true),
	m_confirmCanvasSize(p_engine, p_parentUI, std::bind(&TopSection::ResizeCanvas, this), KTech::Keys::return_, KTech::Point(p_pos.x + 47, p_pos.y), "Confirm", true)
{
	// Widgets
	m_exit.Select();
	AddWidget(m_exit.m_id);
	AddWidget(m_import.m_id);
	AddWidget(m_export.m_id);
	AddWidget(m_canvasSizeX.m_id);
	AddWidget(m_canvasSizeY.m_id);
	AddWidget(m_confirmCanvasSize.m_id);
	// Inputs
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::right, std::bind(&TopSection::MoveRight, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::left, std::bind(&TopSection::MoveLeft, this)));
}

void TextureCreatorUI::TopSection::MoveRight()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == WIDGETS_SIZE - 1)
		m_curWidget = (WidgetIndex)0;
	else
		m_curWidget = (WidgetIndex)(m_curWidget + 1);
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
}

void TextureCreatorUI::TopSection::MoveLeft()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == 0)
		m_curWidget = (WidgetIndex)(WIDGETS_SIZE - 1);
	else
		m_curWidget = (WidgetIndex)(m_curWidget - 1);
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
}

void TextureCreatorUI::TopSection::ResizeCanvas()
{
	TextureCreator::ui.m_canvas.Resize(KTech::UPoint(m_canvasSizeX.m_number, m_canvasSizeY.m_number));
}