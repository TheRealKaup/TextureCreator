#include "widget.hpp"

struct Canvas : public Widget
{
	Engine::Object brush;

	Engine::AudioSource as_canvasSize;
	Engine::AudioSource as_draw;

	Engine::RGBA selectedRGBA = Engine::RGBA(255, 255, 255, 255);
	Engine::RGBA unselectedRGBA = Engine::RGBA(150, 150, 150, 255);

	Engine::UPoint size;

	int brushSpeed = 0;

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

	void ChangeBackground(uint8_t type)
	{
		if (type == 1)
		{
			for (size_t y = 0; y < obj.textures[0].t.size(); y++)
			{
				for (size_t x = 0; x < obj.textures[0].t[y].size(); x++)
					obj.textures[0].t[y][x].b = { 0, 0, 0, 0 };
			}
		}
		else if (type == 2)
		{
			for (size_t y = 0; y < obj.textures[0].t.size(); y++)
			{
				for (size_t x = 0; x < obj.textures[0].t[y].size(); x++)
					obj.textures[0].t[y][x].b = { 255, 0, 0, 255 };
			}
		}
		else if (type == 3)
		{
			bool offone = false;
			for (size_t y = 0; y < obj.textures[0].t.size(); y++, offone = !offone)
			{
				for (size_t x = 0; x < obj.textures[0].t[y].size(); x++)
				{
					if (x % 2 == offone)
						obj.textures[0].t[y][x].b = { 205, 205, 205, 255 };
					else
						obj.textures[0].t[y][x].b = { 255, 255, 255, 255 };
				}
			}
		}
	}

	void Resize(bool calledFromMain, Engine::UPoint size, uint8_t backgroundType)
	{
		// Textures

		uint32_t sizeX = size.x;
		uint32_t sizeY = size.y;

		// Background
		obj.textures[0].pos = { 1, 1 };
		obj.textures[0].t.resize(sizeY);
		if (backgroundType == 1)
		{
			for (size_t y = 0; y < sizeY; y++)
				obj.textures[0].t[y].resize(sizeX, Engine::CellA(' ', { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));
		}
		else if (backgroundType == 2)
		{
			for (size_t y = 0; y < sizeY; y++)
				obj.textures[0].t[y].resize(sizeX, Engine::CellA(' ', { 0, 0, 0, 0 }, { 255, 0, 0, 255 }));
		}
		else if (backgroundType == 3)
		{
			bool offone = false;
			for (size_t y = 0; y < sizeY; y++, offone = !offone)
			{
				obj.textures[0].t[y].resize(sizeX, Engine::CellA(' ', { 0, 0, 0, 0 }, { 255, 255, 255, 255 }));
				for (size_t x = offone; x < sizeX; x+=2)
					obj.textures[0].t[y][x].b = { 205, 205, 205, 255 };
			}
		}
		
		// If the size isn't changed then don't proceed. (Unless it is the first call)
		if (obj.textures[5].t.size() == sizeY && obj.textures[5].t[0].size() == sizeX && !calledFromMain)
			return;

		// Top frame
		obj.textures[1].Simple({ sizeX, 1 }, Engine::CellA('-', unselectedRGBA, { 0, 0, 0, 0 }), { 1, 0 });

		// Left frame
		obj.textures[2].Simple({1, sizeY + 2}, Engine::CellA('|', unselectedRGBA, {0, 0, 0, 0}), {0, 0});
		obj.textures[2].t[0][0].c = '/';
		obj.textures[2].t[sizeY + 1][0].c = '\\';

		// Bottom frame
		obj.textures[3].Simple({ sizeX, 1 }, Engine::CellA('-', unselectedRGBA, { 0, 0, 0, 0 }), { 1, (int)(sizeY + 1) });

		// Right frame
		obj.textures[4].Simple({ 1, sizeY + 2 }, Engine::CellA('|', unselectedRGBA, { 0, 0, 0, 0 }), { (int)(sizeX + 1), 0 });
		obj.textures[4].t[0][0].c = '\\';
		obj.textures[4].t[sizeY + 1][0].c = '/';

		// Canvas itself
		obj.textures[5].pos = { 1, 1 };
		obj.textures[5].t.resize(sizeY);
		for (size_t y = 0; y < sizeY; y++)
			obj.textures[5].t[y].resize(sizeX, Engine::CellA(' ', { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));


		// Colliders

		// Top
		obj.colliders[0].pos = { 0, 0 };
		obj.colliders[0].simple = true;
		obj.colliders[0].size = { sizeX + 2, 1 };
		obj.colliders[0].type = 0;

		// Left
		obj.colliders[1].pos = { 0, 0 };
		obj.colliders[1].simple = true;
		obj.colliders[1].size = { 1, sizeY + 2 };
		obj.colliders[1].type = 1;

		// Bottom
		obj.colliders[2].pos = { 0, (int)(sizeY + 1) };
		obj.colliders[2].simple = true;
		obj.colliders[2].size = { sizeX + 2, 1 };
		obj.colliders[2].type = 2;

		// Right
		obj.colliders[3].pos = { (int)(sizeX + 1), 0 };
		obj.colliders[3].simple = true;
		obj.colliders[3].size = { 1, sizeY + 2 };
		obj.colliders[3].type = 3;

		// Play sound effect.
		if (!calledFromMain)
			as_canvasSize.Play(0, 0, 0, 0.5f);
	}

	void MoveBrush()
	{
		// Only move if canvas is selected
		if (!selected)
			return;

		if (Engine::Input::Is('w')) // up
			obj.textures[10].pos.y--;
		else if (Engine::Input::Is('a')) // left
			obj.textures[10].pos.x--;
		else if (Engine::Input::Is('s')) // down
			obj.textures[10].pos.y++;
		else // right
			obj.textures[10].pos.x++;
	}

	void Draw(bool fore, bool back, bool character)
	{
		// Only draw if canvas is selected
		if (!selected)
			return;

		for (size_t y = 0; y < brush.textures[0].t.size(); y++)
		{
			for (size_t x = 0; x < brush.textures[0].t[y].size(); x++)
			{
				int fy = y - obj.pos.y - obj.textures[5].pos.y + brush.pos.y;
				int fx = x - obj.pos.x - obj.textures[5].pos.x + brush.pos.x;

				if (fy < 0 || fy >= obj.textures[5].t.size())
					break;
				if (fx < 0 || fx >= obj.textures[5].t[0].size())
					continue;

				if (fore)
					obj.textures[5].t[fy][fx].f = brush.textures[0].t[y][x].f;
				if (back)
					obj.textures[5].t[fy][fx].b = brush.textures[0].t[y][x].b;
				if (character)
					obj.textures[5].t[fy][fx].c = brush.textures[0].t[y][x].c;
			}
		}
		// Play sound effect.
		as_draw.Play();
	}

	void UpdateBrush(Engine::UPoint size, Engine::CellA value)
	{
		// Texture
		brush.textures[0].Simple(size, value, Engine::Point(0, 0));

		// Colliders
		brush.colliders[0].pos = Engine::Point(0, 0);
		brush.colliders[0].simple = true;
		brush.colliders[0].size = { 1, 1 };
		brush.colliders[0].type = 0;

		brush.colliders[1].pos = Engine::Point(0, size.y - 1);
		brush.colliders[1].simple = true;
		brush.colliders[1].size = { 1, 1 };
		brush.colliders[1].type = 1;

		brush.colliders[2].pos = Engine::Point( size.x - 1, size.y - 1 );
		brush.colliders[2].simple = true;
		brush.colliders[2].size = { 1, 1 };
		brush.colliders[2].type = 2;

		brush.colliders[3].pos = Engine::Point( size.x - 1, 0 );
		brush.colliders[3].simple = true;
		brush.colliders[3].size = { 1, 1 };
		brush.colliders[3].type = 3;
	}

	void Import()
	{
		std::ifstream file("import.kcget");
		if (!file.is_open())
			return;
		std::string line;

		Engine::CellA schar;

		int maxX = 0;

		obj.textures[5].t = {};

		size_t y = 0;
		for (; std::getline(file, line); y++)
		{
			obj.textures[5].t.push_back({});

			// potentially broken if one of the values = 10 ('\n')

			for (size_t x = 0, j = 0; x < line.length(); x++, j++)
			{
				if (j == 9)
					j = 0;
				if (j == 0)
					schar.f.r = (unsigned char)line[x];
				else if (j == 1)
					schar.f.g = (unsigned char)line[x];
				else if (j == 2)
					schar.f.b = (unsigned char)line[x];
				else if (j == 3)
					schar.f.a = (unsigned char)line[x] / 255.0f;
				else if (j == 4)
					schar.b.r = (unsigned char)line[x];
				else if (j == 5)
					schar.b.g = (unsigned char)line[x];
				else if (j == 6)
					schar.b.b = (unsigned char)line[x];
				else if (j == 7)
					schar.b.a = (unsigned char)line[x] / 255.0f;
				else if (j == 8)
				{
					schar.c = line[x];
					obj.textures[5].t[y].push_back(schar);
				}
			}

			if (maxX < obj.textures[5].t[y].size())
				maxX = obj.textures[5].t[y].size();
		}

		for (size_t y = 0; y < obj.textures[5].t.size(); y++)
			obj.textures[5].t[y].resize(maxX, Engine::CellA(' ', { 0, 0, 0, 0 }, { 0, 0, 0, 0 }));

		size.x = maxX;
		size.y = y;

		Resize(true, size, 1);
	}

	void Export()
	{
		std::ofstream outFile("export.kcget");

		// create (if doesn't exist) file named "textureExport.kcget" and its content is
		// the current texture of canvas.textures[0].t

		std::string out = "";

		for (size_t y = 0; y < obj.textures[5].t.size(); y++)
		{
			for (size_t x = 0; x < obj.textures[5].t[y].size(); x++)
			{
				out += obj.textures[5].t[y][x].f.r;
				out += obj.textures[5].t[y][x].f.g;
				out += obj.textures[5].t[y][x].f.b;
				out += (char)(obj.textures[5].t[y][x].f.a * 255);
				out += obj.textures[5].t[y][x].b.r;
				out += obj.textures[5].t[y][x].b.g;
				out += obj.textures[5].t[y][x].b.b;
				out += (char)(obj.textures[5].t[y][x].b.a * 255);
				out += obj.textures[5].t[y][x].c;
			}
			if (y < obj.textures[5].t.size() - 1)
				out += '\n';
		}

		outFile.write(out.c_str(), out.length());
	}

	void ResetBrushPos()
	{
		brush.pos = { obj.pos.x + 1, obj.pos.y + 1 };
	}

	Canvas(Engine::UPoint size, Engine::Point pos, uint8_t backgroundType, Engine::UPoint brushSize, Engine::CellA brushValue, Engine::Layer* layer)
	{
		obj.pos = pos;

		// Canvas textures
		obj.textures.resize(11);
		// Background
		if (backgroundType == 0)
			obj.textures[0].Rectangle(size, Engine::CellA(' ', Engine::RGBA(0, 0, 0, 0), Engine::RGBA(0, 0, 0, 255)), Engine::Point(1, 1));
		else if (backgroundType == 1)
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
		obj.textures[1].Rectangle(size, Engine::CellA(' '), Engine::Point(1, 1));
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

		// Audio sources
		as_canvasSize.LoadWavFile("assets/canvasResize.wav");
		as_draw.LoadWavFile("assets/draw.wav");

		// Input handlers
		// Engine::Input::RegisterHandler(kReturn, std::bind(&Canvas::Draw, this, true, true, true), true);
		Engine::Input::RegisterHandler('w', std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler('a', std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler('s', std::bind(&Canvas::MoveBrush, this), true);
		Engine::Input::RegisterHandler('d', std::bind(&Canvas::MoveBrush, this), true);
	}
};