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