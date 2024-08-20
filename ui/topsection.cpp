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

TextureCreatorUI::TopSection::TopSection(TextureCreatorUI* const p_textureCreatorUI, KTech::Engine& p_engine, KTech::ID<KTech::UI> p_parentUI, KTech::Point p_pos)
	: KTech::Widget(p_engine, p_parentUI, p_pos),
	textureCreatorUI(p_textureCreatorUI),
	m_exit(p_engine, p_parentUI, std::bind(&KTech::Engine::Quit, &p_engine), KTech::Keys::return_, KTech::Point(p_pos.x, p_pos.y), "Exit", true),
	m_import(p_engine, p_parentUI, std::bind(&Canvas::Import, &p_textureCreatorUI->m_canvas, "import.ktecht"), KTech::Keys::return_, KTech::Point(p_pos.x + 7, p_pos.y), "Import", true),
	m_export(p_engine, p_parentUI, std::bind(&Canvas::Export, &p_textureCreatorUI->m_canvas, "export.ktecht"), KTech::Keys::return_, KTech::Point(p_pos.x + 16, p_pos.y), "Export", true),
	m_canvasSizeX(p_engine, p_parentUI, nullptr, 1, DefaultValues::maxCanvasSize.x, std::to_string(DefaultValues::canvasSize.x), KTech::Point(p_pos.x + 25, p_pos.y), "CanvasX=", true),
	m_canvasSizeY(p_engine, p_parentUI, nullptr, 1, DefaultValues::maxCanvasSize.y, std::to_string(DefaultValues::canvasSize.y), KTech::Point(p_pos.x + 36, p_pos.y), "CanvasY=", true),
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
	// Select default widget
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	// Inputs
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::right, std::bind(&TopSection::MoveRight, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::left, std::bind(&TopSection::MoveLeft, this)));
}

bool TextureCreatorUI::TopSection::MoveRight()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == m_childWidgets.size() - 1)
		m_curWidget = 0;
	else
		m_curWidget++;
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	return true;
}

bool TextureCreatorUI::TopSection::MoveLeft()
{
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Deselect();
	if (m_curWidget == 0)
		m_curWidget = m_childWidgets.size() - 1;
	else
		m_curWidget--;
	engine.memory.widgets[m_childWidgets[m_curWidget].widget]->Select();
	return true;
}

void TextureCreatorUI::TopSection::ResizeCanvas()
{
	textureCreatorUI->m_canvas.Resize(KTech::UPoint(m_canvasSizeX.m_number, m_canvasSizeY.m_number));
}