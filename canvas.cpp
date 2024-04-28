#include "canvas.hpp"

Canvas::Canvas(KTech::Engine& p_engine,
	KTech::ID<KTech::UI> p_parentUI,
	KTech::Point p_pos,
	KTech::UPoint p_size,
	Background p_background,
	KTech::UPoint p_brushSize,
	KTech::CellA p_brushValue,
	bool p_foreTool, bool p_backTool, bool p_charTool)
	: Widget(p_engine, p_parentUI, p_pos), m_foregroundTool(p_foreTool), m_backgroundTool(p_backTool), m_characterTool(p_charTool)
{
	m_size = p_size;

	// Textures
	m_textures.resize(11);
	// Background
	SetBackground(p_background);
	// Canvas itself
	m_textures[1].Rectangle(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBAColors::transparent), KTech::Point(1, 1));
	// Top-left corner
	m_textures[2].Simple(KTech::UPoint(1, 1), KTech::CellA('/', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(0, 0));
	// Top-right corner
	m_textures[3].Simple(KTech::UPoint(1, 1), KTech::CellA('\\', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(1 + m_size.x, 0));
	// Bottom-left corner
	m_textures[4].Simple(KTech::UPoint(1, 1), KTech::CellA('\\', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(0, 1 + m_size.y));
	// Bottom-right corner
	m_textures[5].Simple(KTech::UPoint(1, 1), KTech::CellA('/', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(1 + m_size.x, 1 + m_size.y));
	// Top frame
	m_textures[6].Simple(KTech::UPoint(m_size.x, 1), KTech::CellA('-', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(1, 0));
	// Bottom frame
	m_textures[7].Simple(KTech::UPoint(m_size.x, 1), KTech::CellA('-', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(1, 1 + m_size.y));
	// Left frame
	m_textures[8].Simple(KTech::UPoint(1, m_size.y), KTech::CellA('|', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(0, 1));
	// Right frame
	m_textures[9].Simple(KTech::UPoint(1, m_size.y), KTech::CellA('|', KTech::RGBA(150, 150, 150, 255), KTech::RGBAColors::transparent), KTech::Point(1 + m_size.x, 1));
	// Brush texture
	m_textures[10].Simple(p_brushSize, p_brushValue, KTech::Point(m_size.x / 2 - p_brushSize.x / 2 + 1, m_size.y / 2 - p_brushSize.y / 2 + 1));

	// Input handlers
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback(KTech::Keys::return_, std::bind(&Canvas::Draw, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("w", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("a", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("s", std::bind(&Canvas::MoveBrush, this)));
	m_callbacksGroup->AddCallback(engine.input.RegisterCallback("d", std::bind(&Canvas::MoveBrush, this)));
}

void Canvas::RenderSelected()
{
	for (size_t i = 1; i < 10; i++)
		m_textures[i].m_value.f = m_selectedRGBA;
	m_selected = true;
}

void Canvas::RenderUnselected()
{
	for (size_t i = 1; i < 10; i++)
		m_textures[i].m_value.f = m_unselectedRGBA;
	m_selected = false;
}

void Canvas::SetBackground(Background p_background)
{
	m_background = p_background;
	if (m_background == Background::black)
		m_textures[0].Simple(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBAColors::transparent), KTech::Point(1, 1));
	else if (m_background == Background::red)
		m_textures[0].Simple(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBA(255, 0, 0, 255)), KTech::Point(1, 1));
	else if (m_background == Background::transparent)
	{
		m_textures[0].Rectangle(m_size, KTech::CellA(' ', KTech::RGBAColors::transparent, KTech::RGBA(255, 255, 255, 255)), KTech::Point(1, 1));
		bool offone = true;
		for (size_t y = 0; y < m_textures[0].m_t.size(); y++, offone = !offone)
			for (size_t x = 0; x < m_textures[0].m_t[y].size(); x++)
				if (x % 2 == offone)
					m_textures[0].m_t[y][x].b = { 205, 205, 205, 255 };
	}
}

void Canvas::Resize(KTech::UPoint p_size)
{
	// If the size isn'm_t changed then don'm_t proceed.
	if (m_size.x == p_size.x && m_size.y == p_size.y)
		return;

	m_size = p_size;

	SetBackground(m_background);
	
	// Resize the canvas itself.
	m_textures[1].m_t.resize(m_size.y);
	for (size_t y = 0; y < m_textures[1].m_t.size(); y++)
		m_textures[1].m_t[y].resize(m_size.x, KTech::CellA(' ', KTech::RGBAColors::transparent));

	// Top-right corner
	m_textures[3].m_rPos.x = m_size.x + 1;
	// Bottom-left corner
	m_textures[4].m_rPos.y = m_size.y + 1;
	// Bottom-right corner
	m_textures[5].m_rPos = KTech::Point(m_size.x + 1, m_size.y + 1);
	// Top frame
	m_textures[6].m_size.x = m_size.x;
	// Bottom frame
	m_textures[7].m_size.x = m_size.x;
	m_textures[7].m_rPos.y = m_size.y + 1;
	// Left frame
	m_textures[8].m_size.y = m_size.y;
	// Right frame
	m_textures[9].m_size.y = m_size.y;
	m_textures[9].m_rPos.x = m_size.x + 1;
}

void Canvas::Draw()
{
	// Only draw if canvas is m_selected
	if (!m_selected)
		return;

	bool drew = false;

	// Draw
	for (size_t y = 0; y < m_textures[10].m_size.y; y++)
	{
		for (size_t x = 0; x < m_textures[10].m_size.x; x++)
		{
			uint32_t fy = m_textures[10].m_rPos.y - 1 + y;
			uint32_t fx = m_textures[10].m_rPos.x - 1 + x;

			if (fy < 0 || fy >= m_textures[1].m_t.size())
				break;
			if (fx < 0 || fx >= m_textures[1].m_t[fy].size())
				continue;

			drew = true;

			if (m_foregroundTool)
				m_textures[1].m_t[fy][fx].f = m_textures[10].m_value.f;
			if (m_backgroundTool)
				m_textures[1].m_t[fy][fx].b = m_textures[10].m_value.b;
			if (m_characterTool)
				m_textures[1].m_t[fy][fx].c = m_textures[10].m_value.c;
		}
	}
}

void Canvas::SetBrushSize(KTech::UPoint p_size)
{
	m_textures[10].m_size = p_size;
}

void Canvas::SetBrushValue(KTech::CellA p_value)
{
	m_textures[10].m_value = p_value;
}

void Canvas::MoveBrush()
{
	// Only move if canvas is m_selected
	if (!m_selected)
		return;

	if (engine.input.Is('w')) // up
	{
		if (m_textures[10].m_rPos.y + m_textures[10].m_size.y > 2)
			m_textures[10].m_rPos.y--;
	}
	else if (engine.input.Is('a')) // left
	{
		if (m_textures[10].m_rPos.x + m_textures[10].m_size.x > 2)
			m_textures[10].m_rPos.x--;
	}
	else if (engine.input.Is('s')) // down
	{
		// Not casting to `int32_t` will make the brush stuck when the position is negative.
		if (m_textures[10].m_rPos.y < (int32_t)m_size.y)
			m_textures[10].m_rPos.y++;
	}
	else // right
	{
		if (m_textures[10].m_rPos.x < (int32_t)m_size.x)
			m_textures[10].m_rPos.x++;
	}
}

void Canvas::ResetBrushPos()
{
	m_textures[10].m_rPos = KTech::Point( m_size.x / 2 + m_textures[10].m_size.x / 2 + 1, m_size.y / 2 + m_textures[10].m_size.y / 2 + 1);
}

void Canvas::Import(const std::string& fileName)
{
	Canvas::Resize(m_textures[1].File(fileName, KTech::Point(1, 1)));
}

void Canvas::Export(const std::string& fileName)
{
	m_textures[1].ExportToFile(fileName);
}