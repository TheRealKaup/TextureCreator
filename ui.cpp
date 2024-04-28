#include "ui.hpp"

#include "texturecreator.hpp"

/*
// Canvas
canvas = new Canvas(
	UPoint(((IntInputField*)topSection[3])->number, ((IntInputField*)topSection[4])->number), // Canvas size
	KTech::Point(14, 4), // Canvas position
	((IntInputField*)sideSection[15])->number, // Background type
	UPoint(((IntInputField*)sideSection[9])->number, ((IntInputField*)sideSection[10])->number), // Brush size
	KTech::CellA(
		((StringInputField*)sideSection[8])->string[0], // Brush character
		KTech::RGBA(((IntInputField*)sideSection[0])->number, // Foreground red
			((IntInputField*)sideSection[1])->number, // Foreground green
			((IntInputField*)sideSection[2])->number, // Foreground blue
			((IntInputField*)sideSection[3])->number), // Foreground alpha
		KTech::RGBA(((IntInputField*)sideSection[4])->number, // Background red
			((IntInputField*)sideSection[5])->number, // Background green
			((IntInputField*)sideSection[6])->number, // Background blue
			((IntInputField*)sideSection[7])->number)), // Background alpha),
	layer);
m_canvas->Select();
*/

TextureCreatorUI::TextureCreatorUI(KTech::Engine& p_engine)
	: KTech::UI(p_engine, TextureCreator::viewportSize),
	m_topSection(p_engine, m_id, KTech::Point(44, 1)),
	m_sideSection(p_engine, m_id, KTech::Point(2, 5)),
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
	frames(p_engine, m_id, KTech::Point(0, 0))
{
	// Sections
	m_topSection.Deselect();
	m_sideSection.Deselect();
	m_canvas.Select();
	// Frames
	frames.m_textures.resize(3);
	frames.m_textures[0].Write( // Logo
		{
		"  ||  /   ----- ----- ----- -   - ___ ___ /",
		" ||| /\\     |   |---  |     |---|  |  |   |",
		"||||/  \\    |   \\____ \\____ |   |  |  \\__ |",
		"/-----------------------------------------/",
		}, KTech::RGBA( 90, 170, 80, 255 ), KTech::RGBA( 0, 0, 0, 0 ), KTech::Point( 0, 0 ));
	frames.m_textures[1].Simple({ 100, 1 }, KTech::CellA('-', { 90, 170, 80 }, { 0, 0, 0 }), { 43, 0 }); // Top frame
	frames.m_textures[2].Simple({ 1, 100 }, KTech::CellA('|', { 90, 170, 80 }, { 0, 0, 0 }), { 0, 4 }); // Side frame
	// foregroundFrame.pos = KTech::Point(2, 4);
	// foregroundFrame.m_textures.resize(9);
	// foregroundFrame.m_textures[0].Write({"Foreground"}, KTech::RGBA(150 ,150, 150, 255), KTech::RGBA(0, 0, 0, 0), KTech::Point(0, 0)); // Text
	// foregroundFrame.m_textures[1].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 1)); // Up-left corner
	// foregroundFrame.m_textures[2].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 1)); // Up-right corner
	// foregroundFrame.m_textures[3].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 6)); // Bottom-left corner
	// foregroundFrame.m_textures[4].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 6)); // Bottom-right corner
	// foregroundFrame.m_textures[5].Simple(UPoint(5, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 1)); // Up frame
	// foregroundFrame.m_textures[6].Simple(UPoint(1, 4), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 2)); // Left frame
	// foregroundFrame.m_textures[7].Simple(UPoint(5, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 6)); // Bottom frame
	// foregroundFrame.m_textures[8].Simple(UPoint(1, 4), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 2)); // Right frame
	// layer->AddObject(&foregroundFrame);
	// backgroundFrame.pos = KTech::Point(2, 12);
	// backgroundFrame.m_textures.resize(9);
	// backgroundFrame.m_textures[0].Write({"Background"}, KTech::RGBA(150 ,150, 150, 255), KTech::RGBA(0, 0, 0, 0), KTech::Point(0, 0)); // Text
	// backgroundFrame.m_textures[1].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 1)); // Up-left corner
	// backgroundFrame.m_textures[2].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 1)); // Up-right corner
	// backgroundFrame.m_textures[3].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 6)); // Bottom-left corner
	// backgroundFrame.m_textures[4].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 6)); // Bottom-right corner
	// backgroundFrame.m_textures[5].Simple(UPoint(5, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 1)); // Up frame
	// backgroundFrame.m_textures[6].Simple(UPoint(1, 4), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 2)); // Left frame
	// backgroundFrame.m_textures[7].Simple(UPoint(5, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 6)); // Bottom frame
	// backgroundFrame.m_textures[8].Simple(UPoint(1, 4), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(6, 2)); // Right frame
	// layer->AddObject(&backgroundFrame);
	// settingsFrame.pos = KTech::Point(2, 24);
	// settingsFrame.m_textures.resize(9);
	// settingsFrame.m_textures[0].Write({"Other"}, KTech::RGBA(150 ,150, 150, 255), KTech::RGBA(0, 0, 0, 0), KTech::Point(0, 0)); // Text
	// settingsFrame.m_textures[1].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 1)); // Up-left corner
	// settingsFrame.m_textures[2].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(10, 1)); // Up-right corner
	// settingsFrame.m_textures[3].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 8)); // Bottom-left corner
	// settingsFrame.m_textures[4].Simple(UPoint(1, 1), KTech::CellA('#', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(10, 8)); // Bottom-right corner
	// settingsFrame.m_textures[5].Simple(UPoint(9, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 1)); // Up frame
	// settingsFrame.m_textures[6].Simple(UPoint(1, 6), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(0, 2)); // Left frame
	// settingsFrame.m_textures[7].Simple(UPoint(9, 1), KTech::CellA('-', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(1, 8)); // Bottom frame
	// settingsFrame.m_textures[8].Simple(UPoint(1, 6), KTech::CellA('|', KTech::RGBA(150 ,150, 150, 255)), KTech::Point(10, 2)); // Right frame
	// layer->AddObject(&settingsFrame);
	// backgroundSelectorFrame.pos = KTech::Point(2, 37);
	// backgroundSelectorFrame.m_textures.resize(1);
	// backgroundSelectorFrame.m_textures[0].File("assets/backgroundOptions.ktecht", KTech::Point(0, 0));
	// layer->AddObject(&backgroundSelectorFrame);
	// Inputs
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