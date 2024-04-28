#include "ui.hpp"
#include "../texturecreator.hpp"

TextureCreatorUI::TextureCreatorUI(KTech::Engine& p_engine)
	: KTech::UI(p_engine, TextureCreator::viewportSize),
	m_topSection(p_engine, m_id, KTech::Point(44, 1)),
	m_sideSection(p_engine, m_id, KTech::Point(2, 4)),
	m_canvas(
		p_engine,
		m_id,
		KTech::Point(14, 4),
		DefaultValues::canvasSize,
		DefaultValues::canvasBackground,
		DefaultValues::brushSize,
		DefaultValues::brushValue,
		DefaultValues::foreTool,
		DefaultValues::backTool,
		DefaultValues::charTool
	),
	m_frame(p_engine, m_id, KTech::Point(0, 0))
{
	// Sections
	m_topSection.Deselect();
	m_sideSection.Deselect();
	m_canvas.Select();
	// Frame
	m_frame.m_textures.resize(3);
	m_frame.m_textures[0].Write( // Logo
		{
		"  ||  /   ----- ----- ----- -   - ___ ___ /",
		" ||| /\\     |   |---  |     |---|  |  |   |",
		"||||/  \\    |   \\____ \\____ |   |  |  \\__ |",
		"/-----------------------------------------/",
		}, KTech::RGBA( 90, 170, 80, 255 ), KTech::RGBA( 0, 0, 0, 0 ), KTech::Point( 0, 0 ));
	m_frame.m_textures[1].Simple({ 100, 1 }, KTech::CellA('-', { 90, 170, 80 }, { 0, 0, 0 }), { 43, 0 }); // Top line
	m_frame.m_textures[2].Simple({ 1, 100 }, KTech::CellA('|', { 90, 170, 80 }, { 0, 0, 0 }), { 0, 4 }); // Side lineti_texture
	// Input
	p_engine.input.RegisterCallback(KTech::Keys::up, std::bind(&TextureCreatorUI::Up, this));
	p_engine.input.RegisterCallback(KTech::Keys::left, std::bind(&TextureCreatorUI::Left, this));
	p_engine.input.RegisterCallback(KTech::Keys::right, std::bind(&TextureCreatorUI::Right, this));
	p_engine.input.RegisterCallback(KTech::Keys::down, std::bind(&TextureCreatorUI::Down, this));
}

void TextureCreatorUI::Up()
{
	if (m_curSection == Section::canvas)
	{
		m_canvas.Deselect();
		m_topSection.Select();
		m_curSection = Section::top;
	}
}

void TextureCreatorUI::Left()
{
	if (m_curSection == Section::canvas)
	{
		m_canvas.Deselect();
		m_sideSection.Select();
		m_curSection = Section::side;
	}
}

void TextureCreatorUI::Right()
{
	if (m_curSection == Section::side)
	{
		m_sideSection.Deselect();
		m_canvas.Select();
		m_curSection = Section::canvas;
	}
}

void TextureCreatorUI::Down()
{
	if (m_curSection == Section::top)
	{
		m_topSection.Deselect();
		m_canvas.Select();
		m_curSection = Section::canvas;
	}
}