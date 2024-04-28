#include "ui.hpp"
#include "../texturecreator.hpp"

TextureCreatorUI::SideSection::SideSection(KTech::Engine& p_engine, KTech::ID<KTech::UI> p_parentUI, KTech::Point p_pos)
	: Widget(p_engine, p_parentUI, p_pos),
	m_foregroundR(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.r), KTech::Point( p_pos.x, p_pos.y ), "R="),
	m_foregroundG(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.g), KTech::Point( p_pos.x, p_pos.y + 1 ), "G="),
	m_foregroundB(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.b), KTech::Point( p_pos.x, p_pos.y + 2 ), "B="),
	m_foregroundA(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.a), KTech::Point( p_pos.x, p_pos.y + 3 ), "A="),
	m_backgroundR(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.r), KTech::Point( p_pos.x, p_pos.y + 7 ), "R="),
	m_backgroundG(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.g), KTech::Point( p_pos.x, p_pos.y + 8 ), "G="),
	m_backgroundB(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.b), KTech::Point( p_pos.x, p_pos.y + 9 ), "B="),
	m_backgroundA(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.a), KTech::Point( p_pos.x, p_pos.y + 10 ), "A="),
	m_character(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), { keyrange_all }, KTech::Point( p_pos.x, p_pos.y + 14 ), "Char=", 1, {DefaultValues::brushValue.c}, true),
	m_brushSizeX(p_engine, p_parentUI, std::bind(&SideSection::SetBrushSize, this), 1, 9, std::to_string(DefaultValues::brushSize.x), KTech::Point( p_pos.x, p_pos.y + 18 ), "BrushX="),
	m_brushSizeY(p_engine, p_parentUI, std::bind(&SideSection::SetBrushSize, this), 1, 9, std::to_string(DefaultValues::brushSize.y), KTech::Point( p_pos.x, p_pos.y + 19 ), "BrushY="),
	m_toggleForeground(p_engine, p_parentUI, std::bind(&SideSection::ToggleForeground, this), KTech::Keys::return_, KTech::Point(p_pos.x, p_pos.y + 20), "Fore Tool", DefaultValues::foreTool),
	m_toggleBackground(p_engine, p_parentUI, std::bind(&SideSection::ToggleBackground, this), KTech::Keys::return_, KTech::Point(p_pos.x, p_pos.y + 21), "Back Tool", DefaultValues::backTool),
	m_toggleCharacter(p_engine, p_parentUI, std::bind(&SideSection::ToggleCharacter, this), KTech::Keys::return_, KTech::Point(p_pos.x, p_pos.y + 22), "Char Tool", DefaultValues::charTool),
	m_canvasBackground(p_engine, p_parentUI, std::bind(&SideSection::SetBackground, this), 0, 2, std::to_string((uint8_t)DefaultValues::canvasBackground), KTech::Point(p_pos.x, p_pos.y + 26), "Back=", true),
	m_frame1(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y), KTech::UPoint(7, 6)),
	m_frame2(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y + 7), KTech::UPoint(7, 6)),
	m_frame3(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y + 18), KTech::UPoint(11, 7))
{
	// Widgets
	m_foregroundR.Select();
	AddWidget(m_foregroundR.m_id);
	AddWidget(m_foregroundG.m_id);
	AddWidget(m_foregroundB.m_id);
	AddWidget(m_foregroundA.m_id);
	AddWidget(m_backgroundR.m_id);
	AddWidget(m_backgroundG.m_id);
	AddWidget(m_backgroundB.m_id);
	AddWidget(m_backgroundA.m_id);
	AddWidget(m_character.m_id);
	AddWidget(m_brushSizeX.m_id);
	AddWidget(m_brushSizeY.m_id);
	AddWidget(m_toggleForeground.m_id);
	AddWidget(m_toggleBackground.m_id);
	AddWidget(m_toggleCharacter.m_id);
	AddWidget(m_canvasBackground.m_id);
	// Inputs
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::down, std::bind(&SideSection::MoveDown, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::up, std::bind(&SideSection::MoveUp, this)));
}

void TextureCreatorUI::SideSection::MoveDown()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == WIDGETS_SIZE - 1)
		m_curWidget = (WidgetIndex)0;
	else
		m_curWidget = (WidgetIndex)(m_curWidget + 1);
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
}

void TextureCreatorUI::SideSection::MoveUp()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == 0)
		m_curWidget = (WidgetIndex)(WIDGETS_SIZE - 1);
	else
		m_curWidget = (WidgetIndex)(m_curWidget - 1);
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
}

void TextureCreatorUI::SideSection::SetBrushSize()
{

	TextureCreator::ui.m_canvas.SetBrushSize(KTech::UPoint(m_brushSizeX.m_number, m_brushSizeY.m_number));
}

void TextureCreatorUI::SideSection::SetBrushValue()
{
	TextureCreator::ui.m_canvas.SetBrushValue(
		KTech::CellA(
			(m_character.m_string.size() == 0 ? ' ' : m_character.m_string[0]),
			KTech::RGBA(
				m_foregroundR.m_number,
				m_foregroundG.m_number,
				m_foregroundB.m_number,
				m_foregroundA.m_number
			),
			KTech::RGBA(
				m_backgroundR.m_number,
				m_backgroundG.m_number,
				m_backgroundB.m_number,
				m_backgroundA.m_number
			)
		)
	);
}

void TextureCreatorUI::SideSection::SetBackground()
{
	TextureCreator::ui.m_canvas.SetBackground((Canvas::Background)m_canvasBackground.m_number);
}

void TextureCreatorUI::SideSection::ToggleForeground()
{
	TextureCreator::ui.m_canvas.m_foregroundTool = m_toggleForeground.m_on;
}

void TextureCreatorUI::SideSection::ToggleBackground()
{
	TextureCreator::ui.m_canvas.m_backgroundTool = m_toggleBackground.m_on;
}

void TextureCreatorUI::SideSection::ToggleCharacter()
{
	TextureCreator::ui.m_canvas.m_characterTool = m_toggleCharacter.m_on;
}