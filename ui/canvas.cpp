#include "ui.hpp"

TextureCreatorUI::Canvas::Canvas(TextureCreatorUI* const p_textureCreatorUI,
	KTech::Engine& p_engine,
	KTech::ID<KTech::UI> p_parentUI,
	KTech::Point p_pos,
	KTech::UPoint p_size,
	Background p_background,
	KTech::UPoint p_brushSize,
	KTech::CellA p_brushValue,
	bool p_foreTool, bool p_backTool, bool p_charTool)
	: Widget(p_engine, p_parentUI, p_pos), textureCreatorUI(p_textureCreatorUI), m_foregroundTool(p_foreTool), m_backgroundTool(p_backTool),
	m_characterTool(p_charTool), m_size(p_size), m_frame(p_engine, p_parentUI, p_pos, KTech::UPoint(p_size.x + 2, p_size.y + 2))
{
	// Textures
	m_textures.resize(TEXTURES_SIZE);
	SetBackground(p_background); // Background (m_textures[ti_background])
	m_textures[ti_canvas].Rectangle(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBAColors::transparent), KTech::Point(1, 1)); // Canvas itself
	m_textures[ti_brush].Simple(p_brushSize, p_brushValue, KTech::Point(m_size.x / 2 - p_brushSize.x / 2 + 1, m_size.y / 2 - p_brushSize.y / 2 + 1)); // Brush
	// Frame
	m_frame.Select();
	AddWidget(m_frame.m_id);
	// Input handlers
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::return_, std::bind(&Canvas::Draw, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("w", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("a", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("s", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("d", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::backspace, std::bind(&Canvas::EraseFully, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::delete_, std::bind(&Canvas::EraseAccordingToToggledTools, this)));
}

void TextureCreatorUI::Canvas::SetBackground(Background p_background)
{
	m_background = p_background;
	if (m_background == Background::black)
		m_textures[ti_background].Simple(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBAColors::transparent), KTech::Point(1, 1));
	else if (m_background == Background::red)
		m_textures[ti_background].Simple(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBA(255, 0, 0, 255)), KTech::Point(1, 1));
	else if (m_background == Background::transparent)
	{
		m_textures[ti_background].Rectangle(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBA(255, 255, 255, 255)), KTech::Point(1, 1));
		bool offone = true;
		for (size_t y = 0; y < m_textures[ti_background].m_t.size(); y++, offone = !offone)
			for (size_t x = 0; x < m_textures[ti_background].m_t[y].size(); x++)
				if (x % 2 == offone)
					m_textures[ti_background].m_t[y][x].b = { 205, 205, 205, 255 };
	}
}

void TextureCreatorUI::Canvas::Resize(KTech::UPoint p_size)
{
	// If the size isn'm_t changed then don'm_t proceed.
	if (m_size.x == p_size.x && m_size.y == p_size.y)
		return;

	m_size = p_size;

	// Background
	SetBackground(m_background);
	// Resize the canvas itself.
	m_textures[ti_canvas].m_t.resize(m_size.y);
	for (size_t y = 0; y < m_textures[ti_canvas].m_t.size(); y++)
		m_textures[ti_canvas].m_t[y].resize(m_size.x, KTech::CellA(' ', KTech::RGBAColors::transparent));
	// Frame
	m_frame.Resize(KTech::UPoint(m_size.x + 2, m_size.y + 2));
	// Reset brush pos
	m_textures[ti_brush].m_rPos = KTech::Point( m_size.x / 2 + m_textures[ti_brush].m_size.x / 2 + 1, m_size.y / 2 + m_textures[ti_brush].m_size.y / 2 + 1);
	// Update ui
	textureCreatorUI->m_topSection.m_canvasSizeX.ChangeValue(std::to_string(m_size.x));
	textureCreatorUI->m_topSection.m_canvasSizeY.ChangeValue(std::to_string(m_size.y));
}

void TextureCreatorUI::Canvas::Draw()
{
	// Only draw if canvas is m_selected
	if (!m_selected)
		return;

	// Draw
	for (size_t y = 0; y < m_textures[ti_brush].m_size.y; y++)
	{
		for (size_t x = 0; x < m_textures[ti_brush].m_size.x; x++)
		{
			uint32_t fy = m_textures[ti_brush].m_rPos.y - 1 + y;
			uint32_t fx = m_textures[ti_brush].m_rPos.x - 1 + x;

			if (fy < 0 || fy >= m_textures[ti_canvas].m_t.size())
				break;
			if (fx < 0 || fx >= m_textures[ti_canvas].m_t[fy].size())
				continue;

			if (m_foregroundTool)
				m_textures[ti_canvas].m_t[fy][fx].f = m_textures[ti_brush].m_value.f;
			if (m_backgroundTool)
				m_textures[ti_canvas].m_t[fy][fx].b = m_textures[ti_brush].m_value.b;
			if (m_characterTool)
				m_textures[ti_canvas].m_t[fy][fx].c = m_textures[ti_brush].m_value.c;
		}
	}
}

void TextureCreatorUI::TextureCreatorUI::Canvas::EraseFully()
{
	for (size_t y = 0; y < m_textures[ti_brush].m_size.y; y++)
	{
		for (size_t x = 0; x < m_textures[ti_brush].m_size.x; x++)
		{
			uint32_t fy = m_textures[ti_brush].m_rPos.y - 1 + y;
			uint32_t fx = m_textures[ti_brush].m_rPos.x - 1 + x;

			if (fy < 0 || fy >= m_textures[ti_canvas].m_t.size())
				break;
			if (fx < 0 || fx >= m_textures[ti_canvas].m_t[fy].size())
				continue;

			m_textures[ti_canvas].m_t[fy][fx].f = KTech::RGBAColors::transparent;
			m_textures[ti_canvas].m_t[fy][fx].b = KTech::RGBAColors::transparent;
			m_textures[ti_canvas].m_t[fy][fx].c = ' ';
		}
	}
}

void TextureCreatorUI::Canvas::EraseAccordingToToggledTools()
{
	for (size_t y = 0; y < m_textures[ti_brush].m_size.y; y++)
	{
		for (size_t x = 0; x < m_textures[ti_brush].m_size.x; x++)
		{
			uint32_t fy = m_textures[ti_brush].m_rPos.y - 1 + y;
			uint32_t fx = m_textures[ti_brush].m_rPos.x - 1 + x;

			if (fy < 0 || fy >= m_textures[ti_canvas].m_t.size())
				break;
			if (fx < 0 || fx >= m_textures[ti_canvas].m_t[fy].size())
				continue;

			if (m_foregroundTool)
				m_textures[ti_canvas].m_t[fy][fx].f = KTech::RGBAColors::transparent;
			if (m_backgroundTool)
				m_textures[ti_canvas].m_t[fy][fx].b = KTech::RGBAColors::transparent;
			if (m_characterTool)
				m_textures[ti_canvas].m_t[fy][fx].c = ' ';
		}
	}
}

void TextureCreatorUI::Canvas::SetBrushSize(KTech::UPoint p_size)
{
	m_textures[ti_brush].m_size = p_size;
}

void TextureCreatorUI::Canvas::SetBrushValue(KTech::CellA p_value)
{
	m_textures[ti_brush].m_value = p_value;
}

void TextureCreatorUI::Canvas::MoveBrush()
{
	// Only move if canvas is m_selected
	if (!m_selected)
		return;

	if (engine.input.Is('w')) // up
	{
		if (m_textures[ti_brush].m_rPos.y + m_textures[ti_brush].m_size.y > 2)
			m_textures[ti_brush].m_rPos.y--;
	}
	else if (engine.input.Is('a')) // left
	{
		if (m_textures[ti_brush].m_rPos.x + m_textures[ti_brush].m_size.x > 2)
			m_textures[ti_brush].m_rPos.x--;
	}
	else if (engine.input.Is('s')) // down
	{
		// Not casting to `int32_t` will make the brush stuck when the position is negative.
		if (m_textures[ti_brush].m_rPos.y < (int32_t)m_size.y)
			m_textures[ti_brush].m_rPos.y++;
	}
	else // right
	{
		if (m_textures[ti_brush].m_rPos.x < (int32_t)m_size.x)
			m_textures[ti_brush].m_rPos.x++;
	}
}

void TextureCreatorUI::Canvas::Import(const std::string& fileName)
{
	Canvas::Resize(m_textures[ti_canvas].File(fileName, KTech::Point(1, 1)));
}

void TextureCreatorUI::Canvas::Export(const std::string& fileName)
{
	m_textures[ti_canvas].ExportToFile(fileName);
}