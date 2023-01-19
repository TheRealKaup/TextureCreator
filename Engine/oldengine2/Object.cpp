#include "Engine.h"

Engine::Object::Object(	std::vector<std::string> stringvectorTexture, Vector2D pos, RGBA frgba, RGBA brgba, std::string name)
	: texture(CreateTexture(stringvectorTexture, frgba, brgba)), pos(pos), name(name) {}