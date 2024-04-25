#include "widget.hpp"

struct Canvas : public Widget
{
	Engine::UPoint size;
	uint8_t backgroundType;

	Engine::Object brushDeprecated;

	Engine::RGBA selectedRGBA = Engine::RGBA(255, 255, 255, 255);
	Engine::RGBA unselectedRGBA = Engine::RGBA(150, 150, 150, 255);

	void Select()
	{
		for (size_t i = 1; i < 10; i++)
			obj.textures[i].value.f = selectedRGBA;
		selected = true;
	}

	void Deselect()
	{
		for (size_t i = 1; i < 10; i++)
			obj.textures[i].value.f = unselectedRGBA;
		selected = false;
	}

	void SetBackground(uint8_t type)
	{
		backgroundType = type;

		if (type == 1)
			obj.textures[0].Simple(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1, 1));
		else if (type == 2)
			obj.textures[0].Simple(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(255, 0, 0, 255)), Engine::Point(1, 1));
		else if (type == 3)
		{
			obj.textures[0].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(255, 255, 255, 255)), Engine::Point(1, 1));
			bool offone = true;
			for (size_t y = 0; y < obj.textures[0].t.size(); y++, offone = !offone)
				for (size_t x = 0; x < obj.textures[0].t[y].size(); x++)
					if (x % 2 == offone)
						obj.textures[0].t[y][x].b = { 205, 205, 205, 255 };
		}
	}

	void Resize(Engine::UPoint _size)
	{
		// If the size isn't changed then don't proceed.
		if (size.x == _size.x && size.y == _size.y)
			return;

		size = _size;

		SetBackground(backgroundType);
		
		// Resize the canvas itself.
		obj.textures[1].t.resize(size.y);
		for (size_t y = 0; y < obj.textures[1].t.size(); y++)
			obj.textures[1].t[y].resize(size.x, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0)));

		// Top-right corner
		obj.textures[3].pos.x = size.x + 1;
		// Bottom-left corner
		obj.textures[4].pos.y = size.y + 1;
		// Bottom-right corner
		obj.textures[5].pos = Engine::Point(size.x + 1, size.y + 1);
		// Top frame
		obj.textures[6].size.x = size.x;
		// Bottom frame
		obj.textures[7].size.x = size.x;
		obj.textures[7].pos.y = size.y + 1;
		// Left frame
		obj.textures[8].size.y = size.y;
		// Right frame
		obj.textures[9].size.y = size.y;
		obj.textures[9].pos.x = size.x + 1;
	}

	void Draw(bool fore, bool back, bool character)
	{
		// Only draw if canvas is selected
		if (!selected)
			return;

		bool drew = false;

		// Draw
		for (size_t y = 0; y < obj.textures[10].size.y; y++)
		{
			for (size_t x = 0; x < obj.textures[10].size.x; x++)
			{
				uint32_t fy = obj.textures[10].pos.y - 1 + y;
				uint32_t fx = obj.textures[10].pos.x - 1 + x;

				if (fy < 0 || fy >= obj.textures[1].t.size())
					break;
				if (fx < 0 || fx >= obj.textures[1].t[fy].size())
					continue;

				drew = true;

				if (fore)
					obj.textures[1].t[fy][fx].f = obj.textures[10].value.f;
				if (back)
					obj.textures[1].t[fy][fx].b = obj.textures[10].value.b;
				if (character)
					obj.textures[1].t[fy][fx].c = obj.textures[10].value.c;
			}
		}
	}

	void UpdateBrush(Engine::UPoint size, Engine::CellA value)
	{
		obj.textures[10].size = size;
		obj.textures[10].value = value;
	}

	void MoveBrush()
	{
		// Only move if canvas is selected
		if (!selected)
			return;

		if (Engine::Input::Is('w')) // up
		{
			if (obj.textures[10].pos.y + obj.textures[10].size.y > 2)
				obj.textures[10].pos.y--;
		}
		else if (Engine::Input::Is('a')) // left
		{
			if (obj.textures[10].pos.x + obj.textures[10].size.x > 2)
				obj.textures[10].pos.x--;
		}
		else if (Engine::Input::Is('s')) // down
		{
			// Not casting to `int32_t` will make the brush stuck when the position is negative.
			if (obj.textures[10].pos.y < (int32_t)size.y)
				obj.textures[10].pos.y++;
		}
		else // right
		{
			if (obj.textures[10].pos.x < (int32_t)size.x)
				obj.textures[10].pos.x++;
		}
	}

	void ResetBrushPos()
	{
		obj.textures[10].pos = Engine::Point( size.x / 2 + obj.textures[10].size.x / 2 + 1, size.y / 2 + obj.textures[10].size.y / 2 + 1);
	}

	void Import(const std::string& fileName)
	{
		Resize(obj.textures[1].File(fileName, Engine::Point(1, 1)));
	}

	void Export(const std::string& fileName)
	{
		obj.textures[1].ExportToFile(fileName);
	}

	Canvas(Engine::UPoint _size, Engine::Point pos, uint8_t backgroundType, Engine::UPoint brushSize, Engine::CellA brushValue, Engine::Layer* layer)
	{
		obj.pos = pos;
		size = _size;

		// Textures
		obj.textures.resize(11);
		// Background
		if (backgroundType == 1)
			obj.textures[0].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(0, 0, 0, 255)), Engine::Point(1, 1));
		else if (backgroundType == 2)
			obj.textures[0].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(255, 0, 0, 255)), Engine::Point(1, 1));
		else
		{
			obj.textures[0].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(255, 255, 255, 255)), Engine::Point(1, 1));
			bool offset = false;
			for (size_t y = 0; y < obj.textures[0].t.size(); y++, offset = !offset)
			{
				bool isGrayPixel = offset;
				for (size_t x = 0; x < obj.textures[0].t[y].size(); x++, isGrayPixel = !isGrayPixel)
					if (isGrayPixel)
						obj.textures[0].t[y][x].b = Engine::RGBA(150, 150, 150, 255); 
			}
		}
		// Canvas itself
		obj.textures[1].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0)), Engine::Point(1, 1));
		// Top-left corner
		obj.textures[2].Simple(Engine::UPoint(1, 1), Engine::CellA('/', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(0, 0));
		// Top-right corner
		obj.textures[3].Simple(Engine::UPoint(1, 1), Engine::CellA('\\', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1 + size.x, 0));
		// Bottom-left corner
		obj.textures[4].Simple(Engine::UPoint(1, 1), Engine::CellA('\\', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(0, 1 + size.y));
		// Bottom-right corner
		obj.textures[5].Simple(Engine::UPoint(1, 1), Engine::CellA('/', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1 + size.x, 1 + size.y));
		// Top frame
		obj.textures[6].Simple(Engine::UPoint(size.x, 1), Engine::CellA('-', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1, 0));
		// Bottom frame
		obj.textures[7].Simple(Engine::UPoint(size.x, 1), Engine::CellA('-', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1, 1 + size.y));
		// Left frame
		obj.textures[8].Simple(Engine::UPoint(1, size.y), Engine::CellA('|', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(0, 1));
		// Right frame
		obj.textures[9].Simple(Engine::UPoint(1, size.y), Engine::CellA('|', Engine::RGBA(150, 150, 150, 255), Engine::RGBA(0, 0, 0, 0)), Engine::Point(1 + size.x, 1));
		// Brush texture
		obj.textures[10].Simple(brushSize, brushValue, Engine::Point(size.x / 2 - brushSize.x / 2 + 1, size.y / 2 - brushSize.y / 2 + 1));

		layer->AddObject(&obj);

		// Input handlers
		Engine::Input::RegisterHandler("w", std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler("a", std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler("s", std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler("d", std::bind(&Canvas::MoveBrush, this), true);
	}
};