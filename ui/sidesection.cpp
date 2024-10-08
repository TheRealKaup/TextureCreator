/*
	TextureCreator, a tool for creating KTech textures.
	Copyright (C) 2023-2024 Ethan Kaufman (AKA Kaup)

	This file is part of TextureCreator.

	TextureCreator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.

	TextureCreator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TextureCreator. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ui.hpp"

TextureCreatorUI::SideSection::SideSection(TextureCreatorUI* const p_textureCreatorUI, KTech::Engine& p_engine, KTech::ID<KTech::UI> p_parentUI, KTech::Point p_pos)
	: Widget(p_engine, p_parentUI, p_pos),
	textureCreatorUI(p_textureCreatorUI),
	m_foregroundR(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.r), KTech::Point( p_pos.x, p_pos.y + 1 ), "R="),
	m_foregroundG(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.g), KTech::Point( p_pos.x, p_pos.y + 2 ), "G="),
	m_foregroundB(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.b), KTech::Point( p_pos.x, p_pos.y + 3 ), "B="),
	m_foregroundA(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.f.a), KTech::Point( p_pos.x, p_pos.y + 4 ), "A="),
	m_backgroundR(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.r), KTech::Point( p_pos.x, p_pos.y + 9 ), "R="),
	m_backgroundG(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.g), KTech::Point( p_pos.x, p_pos.y + 10 ), "G="),
	m_backgroundB(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.b), KTech::Point( p_pos.x, p_pos.y + 11 ), "B="),
	m_backgroundA(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), 0, 255, std::to_string(DefaultValues::brushValue.b.a), KTech::Point( p_pos.x, p_pos.y + 12 ), "A="),
	m_character(p_engine, p_parentUI, std::bind(&SideSection::SetBrushValue, this), { keyrange_all }, KTech::Point( p_pos.x, p_pos.y + 17 ), "Char=", 1, {DefaultValues::brushValue.c}, true),
	m_brushSizeX(p_engine, p_parentUI, std::bind(&SideSection::SetBrushSize, this), 1, 99, std::to_string(DefaultValues::brushSize.x), KTech::Point( p_pos.x, p_pos.y + 21 ), "BrushX="),
	m_brushSizeY(p_engine, p_parentUI, std::bind(&SideSection::SetBrushSize, this), 1, 99, std::to_string(DefaultValues::brushSize.y), KTech::Point( p_pos.x, p_pos.y + 22 ), "BrushY="),
	m_toggleForeground(p_engine, p_parentUI, std::bind(&SideSection::ToggleForeground, this), KTech::Keys::return_, KTech::Point(p_pos.x - 1, p_pos.y - 1), "Foreground", DefaultValues::foreTool),
	m_toggleBackground(p_engine, p_parentUI, std::bind(&SideSection::ToggleBackground, this), KTech::Keys::return_, KTech::Point(p_pos.x - 1, p_pos.y + 7), "Background", DefaultValues::backTool),
	m_toggleCharacter(p_engine, p_parentUI, std::bind(&SideSection::ToggleCharacter, this), KTech::Keys::return_, KTech::Point(p_pos.x - 1, p_pos.y + 15), "Char Tool", DefaultValues::charTool),
	m_canvasBackground(p_engine, p_parentUI, std::bind(&SideSection::SetBackground, this), 0, 2, std::to_string((uint8_t)DefaultValues::canvasBackground), KTech::Point(p_pos.x, p_pos.y + 26), "Back=", true),
	m_frame1(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y + 1), KTech::UPoint(7, 6)),
	m_frame2(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y + 9), KTech::UPoint(7, 6)),
	m_frame3(p_engine, p_parentUI, KTech::Point(p_pos.x, p_pos.y + 21), KTech::UPoint(11, 4))
{
	// Widgets
	AddWidget(m_toggleForeground.m_id);
	AddWidget(m_foregroundR.m_id);
	AddWidget(m_foregroundG.m_id);
	AddWidget(m_foregroundB.m_id);
	AddWidget(m_foregroundA.m_id);
	AddWidget(m_toggleBackground.m_id);
	AddWidget(m_backgroundR.m_id);
	AddWidget(m_backgroundG.m_id);
	AddWidget(m_backgroundB.m_id);
	AddWidget(m_backgroundA.m_id);
	AddWidget(m_toggleCharacter.m_id);
	AddWidget(m_character.m_id);
	AddWidget(m_brushSizeX.m_id);
	AddWidget(m_brushSizeY.m_id);
	AddWidget(m_canvasBackground.m_id);
	// Select default widget
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	// Inputs
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::down, std::bind(&SideSection::MoveDown, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::up, std::bind(&SideSection::MoveUp, this)));
}

bool TextureCreatorUI::SideSection::MoveDown()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget < m_childWidgets.size() - 1)
		m_curWidget++;
	else
		m_curWidget = 0;
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	return true;
}

bool TextureCreatorUI::SideSection::MoveUp()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget > 0)
		m_curWidget--;
	else
		m_curWidget = m_childWidgets.size() - 1;
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	return true;
}

void TextureCreatorUI::SideSection::SetBrushSize()
{

	textureCreatorUI->m_canvas.SetBrushSize(KTech::UPoint(m_brushSizeX.m_number, m_brushSizeY.m_number));
}

void TextureCreatorUI::SideSection::SetBrushValue()
{
	textureCreatorUI->m_canvas.SetBrushValue(
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
	textureCreatorUI->m_canvas.SetBackground((Canvas::Background)m_canvasBackground.m_number);
}

void TextureCreatorUI::SideSection::ToggleForeground()
{
	textureCreatorUI->m_canvas.m_foregroundTool = m_toggleForeground.m_on;
}

void TextureCreatorUI::SideSection::ToggleBackground()
{
	textureCreatorUI->m_canvas.m_backgroundTool = m_toggleBackground.m_on;
}

void TextureCreatorUI::SideSection::ToggleCharacter()
{
	textureCreatorUI->m_canvas.m_characterTool = m_toggleCharacter.m_on;
}