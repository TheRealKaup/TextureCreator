#include "Engine.h"

Engine::RGB::RGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}

Engine::RGBA::RGBA(unsigned char red, unsigned char green, unsigned char blue, float alpha) : r(red), g(green), b(blue)
{
	if (alpha > 1.0f)
		a = 1.0f;
	else if (alpha < 0.0f)
		a = 0.0f;
	else
		a = alpha;
}

Engine::Vector2D::Vector2D(int xAxis, int yAxis) : x(xAxis), y(yAxis) {}

Engine::UVector2D::UVector2D(unsigned int x, unsigned int y) : x(x), y(y) {}

Engine::Pixel::Pixel(char32_t character, RGB foreRGB, RGB backRGB) : character(character), frgb(foreRGB), brgb(backRGB) {}

Engine::SuperChar::SuperChar(char32_t character, RGBA foreRGBA, RGBA backRGBA) : character(character), frgba(foreRGBA), brgba(backRGBA) {}

Engine::TimePoint::TimePoint()
{
		chornoTimePoint = std::chrono::high_resolution_clock::now();
}
void Engine::TimePoint::SetToNow()
{
	chornoTimePoint = std::chrono::high_resolution_clock::now();
}
int Engine::TimePoint::Seconds()
{
	return std::chrono::duration_cast<std::chrono::seconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
int Engine::TimePoint::Milliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
int Engine::TimePoint::Microseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}
int Engine::TimePoint::Nanoseconds()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(chornoTimePoint - engineStartTP.chornoTimePoint).count();
}

Engine::Texture::Texture(std::vector<std::vector<SuperChar>> texture, Vector2D pos) : t(texture), pos(pos) {}
Engine::Texture::Texture(UVector2D size, char defaultChar, RGBA frgba, RGBA brgba, Vector2D pos) : pos(pos) {
	t.resize(size.y, std::vector<SuperChar>(size.x, Engine::SuperChar(defaultChar, frgba, brgba)));
}
Engine::Texture::Texture(std::string fileName, Vector2D pos) : pos(pos)
{
	std::ifstream file(fileName);
	std::string line;

	RGBA frgba, brgba;
	char32_t character;

	for (size_t y = 0; std::getline(file, line); y++)
	{
		t.resize(t.size() + 1);
		for (size_t x = 0, j = 0; x < line.length(); x++, j++)
		{
			if (j == 9)
				j = 0;
			if (j == 0)
				frgba.r = line[x];
			else if (j == 1)
				frgba.g = line[x];
			else if (j == 2)
				frgba.b = line[x];
			else if (j == 3)
				frgba.a = line[x];
			else if (j == 4)
				brgba.r = line[x];
			else if (j == 5)
				brgba.g = line[x];
			else if (j == 6)
				brgba.b = line[x];
			else if (j == 7)
				brgba.a = 255.0f / line[x];
			else if (j == 8)
			{
				character = line[x];
				t[y].push_back(SuperChar(character, frgba, brgba));
			}
		}
	}
}
Engine::Texture::Texture(std::vector<std::string> stringVector, RGBA frgba = RGBA(255U, 255U, 255U, 1.0f), RGBA brbga = RGBA(0U, 0U, 0U, 0.0f), Vector2D pos) : pos(pos)
{
	int y = 0, x = 0;
	t.resize(stringVector.size());
	for (; y < stringVector.size(); y++)
	{
		t[y].resize(stringVector[y].length());
		for (x = 0; x < stringVector[y].length(); x++)
		{
			t[y][x].character = stringVector[y][x];
			t[y][x].character = stringVector[y][x];
			t[y][x].frgba = frgba;
			t[y][x].brgba = brbga;
		}
	}
}

Engine::Collider::Collider(UVector2D size, Vector2D pos) : size(size), pos(pos), simple(true) {}
Engine::Collider::Collider(std::vector<std::vector<bool>> collider, Vector2D pos) : c(collider), pos(pos), simple(false) {}

Engine::Object::Object(Vector2D pos, std::string name) : pos(pos), name(name) {}