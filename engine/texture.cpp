#include "engine.hpp"

void Engine::Texture::Simple(UPoint _size, CellA _value, Point _pos) {
	simple = true;
	pos = _pos;
	size = _size;
	value = _value;
}

void Engine::Texture::Rectangle(UPoint _size, CellA _value, Point _pos)
{
	simple = false;
	pos = _pos;
	
	t.resize(_size.y, std::vector<CellA>(_size.y, _value));
	for (size_t y = 0; y < t.size(); y++)
	{
		t[y].resize(_size.x);
		for (size_t x = 0; x < t[y].size(); x++)
			t[y][x] = _value;
	}
}

static void Log(const std::string& text)
{
	std::cout << text << std::endl << std::flush;
}

Engine::UPoint Engine::Texture::File(const std::string& fileName, Point _pos)
{
	simple = false;
	pos = _pos;

	// Open file
	std::ifstream file(fileName);
	if (!file.is_open())
		return UPoint(0, 0);

	// Get the texture size (first 8 bytes of the file)
	UPoint maxTextureSize;
	file.read((char*)&maxTextureSize, 8);

	// Resize the texture
	t.resize(maxTextureSize.y);
	for (size_t y = 0; y < t.size(); y++)
		t[y].resize(maxTextureSize.x, CellA(' ', RGBA(0, 0, 0, 0), RGBA(0, 0, 0, 0)));

	// Read and write to the texture	
	for (size_t y = 0; y < t.size(); y++)
	{		void ExportTexture(const std::string& fileName);

		for (size_t x = 0; x < t[y].size(); x++)
		{
			if (file.read((char*)&t[y][x], 9).eof())
			{
				y = t.size();
				break;
			}
		}
	}

	// running = false;

    return maxTextureSize;
}

void Engine::Texture::ExportToFile(const std::string& fileName)
{
	std::ofstream file(fileName);
	
	UPoint maxTextureSize(0, t.size());

	for (size_t y = 0; y < t.size(); y++)
		if (t[y].size() > maxTextureSize.x)
			maxTextureSize.x = t[y].size();

	file.write((char*)&maxTextureSize, 8);

	for (size_t y = 0; y < maxTextureSize.y; y++)
	{
		for (size_t x = 0; x < maxTextureSize.x; x++)
		{
			if (x < t[y].size())
				file.write((char*)&t[y][x], 9);
			else
				file.write("\0\0\0\0\0\0\0\0\0", 9);
		}
	}
}

void Engine::Texture::Write(const std::vector<std::string>& stringVector, RGBA frgba, RGBA brgba, Point _pos) {
	simple = false;
	pos = _pos;
	t.resize(stringVector.size());
	for (size_t y = 0; y < stringVector.size(); y++)
	{
		t[y].resize(stringVector[y].length());
		for (size_t x = 0; x < stringVector[y].length(); x++)
		{
			t[y][x].c = stringVector[y][x];
			t[y][x].f = frgba;
			t[y][x].b = brgba;
		}
	}
}

void Engine::Texture::SetCell(CellA _value)
{
	if (simple)
		value = _value;
	else
		for (size_t y = 0; y < t.size(); y++)
			for (size_t x = 0; x < t[y].size(); x++)
				t[y][x] = _value;
}

void Engine::Texture::SetForeground(RGBA _value)
{
	if (simple)
		value.f = _value;
	else
		for (size_t y = 0; y < t.size(); y++)
			for (size_t x = 0; x < t[y].size(); x++)
				t[y][x].f = _value;
}

void Engine::Texture::SetBackground(RGBA _value)
{
	if (simple)
		value.b = _value;
	else
		for (size_t y = 0; y < t.size(); y++)
			for (size_t x = 0; x < t[y].size(); x++)
				t[y][x].b = _value;
}

void Engine::Texture::SetCharacter(char _value)
{
	if (simple)
		value.c = _value;
	else
		for (size_t y = 0; y < t.size(); y++)
			for (size_t x = 0; x < t[y].size(); x++)
				t[y][x].c = _value;
}