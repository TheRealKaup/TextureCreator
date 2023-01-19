#include "Engine.h"

// ---=== Engine Variables ===---

// Objects
Engine::Map* Engine::activeMap;
std::vector<Engine::Object> Engine::storedObjects = {};
std::vector<Engine::Layer> Engine::storedLayers = {};
// Data
int Engine::lastDownKey = -1;
int Engine::lastUpKey = -1;
int Engine::lastDownVirtualKey = -1;
int Engine::lastUpVirtualKey = -1;
bool Engine::lastDownKeyRequiresShift = false;
bool Engine::lastUpKeyRequiresShift = false;
// Time
float Engine::potentialfps = 0.0f;
float Engine::fps = 0.0f;
Engine::TimePoint Engine::thisTickStartTP;
int Engine::tps = 12;
long int Engine::deltaTime = 0;
Engine::TimePoint Engine::engineStartTP;
long int Engine::totalTicks = 0;
// Config
std::function<void()> Engine::GlobalOnTick;
bool Engine::running = true;
// Physics
std::vector<std::vector<int>> Engine::colliderTypes = {
	{ 0, 1, 2 }, // Heavy - 0
	{ 0, 1, 2 }, // Normal - 1
	{ 0, 0, 2 } // Overlappable - 2
};
// Other
extern wchar_t* consoleBuffer;

// ---=== Input Manager ===---

struct InputHandler
{
	std::vector<std::function<void()>> functions;
	bool down = false;
	bool onDown;
	int key;
	int lastKeyRepresentation; // is the user registers the key as ',' instead of VK_OEM_COMMA, then they would probably want lastKey to be set as ',' and not a virutal key code.
	bool requiresShift;

	InputHandler(int win32Key, bool onKeyDown, std::function<void()> function, int lastKeyRepresentation, bool requiresShift)
		: key(win32Key), onDown(onKeyDown), functions({ function }), lastKeyRepresentation(lastKeyRepresentation), requiresShift(requiresShift) {}
};
std::vector<InputHandler> inputHandlers;
void Engine::RegisterInputHandler(int key, bool onKeyDown, std::function<void()> function, bool vk)
{
	if (!function)
		return;

	int convertedKey = key;
	bool requiresShift = false;
	
	if (!vk) // Microsoft are so stupid they set virtual keys like VK_UP to = '&'.
	{
		if (key >= 'A' && key <= 'Z')
			requiresShift = true;
		else if (key >= 'a' && key <= 'z')
			convertedKey = key - 32;
		else if (key == '!')
		{
			requiresShift = true;
			convertedKey = '1';
		}
		else if (key == '@')
		{
			requiresShift = true;
			convertedKey = '2';
		}
		else if (key == '#')
		{
			requiresShift = true;
			convertedKey = '3';
		}
		else if (key == '$')
		{
			requiresShift = true;
			convertedKey = '4';
		}
		else if (key == '%')
		{
			requiresShift = true;
			convertedKey = '5';
		}
		else if (key == '^')
		{
			requiresShift = true;
			convertedKey = '6';
		}
		else if (key == '&')
		{
			requiresShift = true;
			convertedKey = '7';
		}
		else if (key == '*')
		{
			requiresShift = true;
			convertedKey = '8';
		}
		else if (key == '(')
		{
			requiresShift = true;
			convertedKey = '9';
		}
		else if (key == ')')
		{
			requiresShift = true;
			convertedKey = '0';
		}
		else if (key == ' ')
			convertedKey = VK_SPACE;
		else if (key == '`')
			convertedKey = VK_OEM_3;
		else if (key == '~')
		{
			requiresShift = true;
			convertedKey = VK_OEM_3;
		}
		else if (key == '-')
			convertedKey = VK_OEM_MINUS;
		else if (key == '_')
		{
			requiresShift = true;
			convertedKey = VK_OEM_MINUS;
		}
		else if (key == '=')
			convertedKey = VK_OEM_PLUS;
		else if (key == '+')
		{
			requiresShift = true;
			convertedKey = VK_OEM_PLUS;
		}
		else if (key == '\\')
			convertedKey = VK_OEM_5;
		else if (key == '|')
		{
			requiresShift = true;
			convertedKey = VK_OEM_5;
		}
		else if (key == '[')
			convertedKey = VK_OEM_4;
		else if (key == '{')
		{
			requiresShift = true;
			convertedKey = VK_OEM_4;
		}
		else if (key == ']')
			convertedKey = VK_OEM_6;
		else if (key == '}')
		{
			requiresShift = true;
			convertedKey = VK_OEM_6;
		}
		else if (key == ';')
			convertedKey = VK_OEM_1;
		else if (key == ':')
		{
			requiresShift = true;
			convertedKey = VK_OEM_1;
		}
		else if (key == '\'')
			convertedKey = VK_OEM_7;
		else if (key == '"')
		{
			requiresShift = true;
			convertedKey = VK_OEM_7;
		}
		else if (key == ',')
			convertedKey = VK_OEM_COMMA;
		else if (key == '<')
		{
			requiresShift = true;
			convertedKey = VK_OEM_COMMA;
		}
		else if (key == '.')
			convertedKey = VK_OEM_PERIOD;
		else if (key == '>')
		{
			requiresShift = true;
			convertedKey = VK_OEM_PERIOD;
		}
		else if (key == '/')
			convertedKey = VK_OEM_2;
		else if (key == '?')
		{
			requiresShift = true;
			convertedKey = VK_OEM_2;
		}
	}
	
	// If the InputHandler already exists, add the function to the functions vector and return. No need to create another InputHandler.
	for (size_t i = 0; i < inputHandlers.size(); i++)
	{
		if (inputHandlers[i].key == convertedKey && inputHandlers[i].lastKeyRepresentation == key && inputHandlers[i].onDown == onKeyDown)
		{
			inputHandlers[i].functions.push_back(function);
			return;
		}
	}

	inputHandlers.push_back(InputHandler(convertedKey, onKeyDown, function, key, requiresShift));
}
void Engine::ManageInputs()
{
	int startingHandlersAmount = inputHandlers.size();
	bool currentKeyState = false;
	
	for (int i = 0; i < inputHandlers.size(); i++)
	{
		currentKeyState = GetAsyncKeyState(inputHandlers[i].key);

		if (inputHandlers[i].onDown && !inputHandlers[i].down && currentKeyState)
		{
			Engine::lastDownKey = inputHandlers[i].lastKeyRepresentation;
			Engine::lastDownVirtualKey = inputHandlers[i].key;
			Engine::lastDownKeyRequiresShift = inputHandlers[i].requiresShift;
			for (size_t j = 0; j < inputHandlers[i].functions.size(); j++)
				inputHandlers[i].functions[j]();
		}
		if (!inputHandlers[i].onDown && inputHandlers[i].down && !currentKeyState)
		{
			Engine::lastUpKey = inputHandlers[i].lastKeyRepresentation;
			Engine::lastUpVirtualKey = inputHandlers[i].key;
			Engine::lastUpKeyRequiresShift = inputHandlers[i].requiresShift;
			for (size_t j = 0; j < inputHandlers[i].functions.size(); j++)
				inputHandlers[i].functions[j]();
		}
		
		if (inputHandlers.size() != startingHandlersAmount)
			return;

		inputHandlers[i].down = currentKeyState;
	}
}

// ---=== Engine ===---

std::vector<std::vector<Engine::SuperChar>> Engine::CreateTexture(std::vector<std::string> stringVector, RGBA frgba, RGBA brgba)
{
	int y = 0, x = 0;
	std::vector<std::vector<Engine::SuperChar>> texture(stringVector.size());
	for (; y < stringVector.size(); y++)
	{
		texture[y].resize(stringVector[y].size());
		for (x = 0; x < stringVector[y].length(); x++)
		{
			texture[y][x].character = stringVector[y][x];
			texture[y][x].frgba = frgba;
			texture[y][x].brgba = brgba;
		}
	}

	return texture;
}
std::vector<std::vector<Engine::SuperChar>> Engine::LoadTextureFromFile(std::string fileName)
{
	std::vector<std::vector<Engine::SuperChar>> texture;

	std::ifstream file(fileName);
	std::string line;

	RGBA frgba, brgba;
	char32_t character;

	for (size_t y = 0 ; std::getline(file, line); y++)
	{
		texture.resize(texture.size() + 1);
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
			{
				brgba.a = 255.0f / line[x];
			}
			else if (j == 8)
			{
				character = line[x];
				texture[y].push_back(SuperChar(character, frgba, brgba));
			}
		}
	}

	return texture;
}

Engine::Object* Engine::StoreObject(Object object)
{
	storedObjects.push_back(object);
	return &storedObjects[storedObjects.size() - 1];
}
Engine::Layer* Engine::StoreLayer(Layer layer)
{
	storedLayers.push_back(layer);
	return &storedLayers[storedLayers.size() - 1];
}

void Engine::SetConsoleSizePixels(unsigned int sizeX, unsigned int sizeY)
{
	RECT consoleRect;
	GetWindowRect(GetConsoleWindow(), &consoleRect);
	MoveWindow(GetConsoleWindow(), consoleRect.left, consoleRect.top, sizeX, sizeY, true);
}
void Engine::SetConsoleFontSize(short int sizeX, short int sizeY)
{
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(_CONSOLE_FONT_INFOEX);
	// GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), true, &fontInfo);
	fontInfo.dwFontSize.X = sizeX;
	fontInfo.dwFontSize.Y = sizeY;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), true, &fontInfo);
}

void Engine::ResetConsole()
{
	// Standard (or mostly, I think) escape sequences.
	std::cout << "\033[0;0H";
	std::cout << "\033[?25l";
}

void Engine::InitializeConsole()
{
	DWORD consoleMode;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &consoleMode);
	SetConsoleMode(hOut, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); // lovely sequences
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hIn, &consoleMode);
	SetConsoleMode(hIn, (((consoleMode & ~ENABLE_QUICK_EDIT_MODE) & ~ENABLE_PROCESSED_INPUT) & ~ENABLE_MOUSE_INPUT)); // disabling most of the annoying hotkeys
	RegisterHotKey(GetConsoleWindow(), 0, MOD_ALT, VK_RETURN);
	// SetConsoleOutputCP(65001);
}
void Engine::WaitUntillNextTick()
{
	Engine::deltaTime = NowInMicroseconds() - Engine::thisTickStartTP.Microseconds();
	Engine::potentialfps = 1000000.0f / Engine::deltaTime;
	while (Engine::deltaTime < 1000000 / Engine::tps)
		Engine::deltaTime = NowInMicroseconds() - Engine::thisTickStartTP.Microseconds();
	Engine::fps = 1000000.0f / Engine::deltaTime;
}
void Engine::CallOnTicks(Engine::Map* map)
{
	if (map->OnTick)
		map->OnTick();

	for (size_t l = 0; l < map->layers.size(); l++)
	{
		if (map->layers[l]->OnTick)
			map->layers[l]->OnTick();

		for (size_t o = 0; o < map->layers[l]->objects.size(); o++)
		{
			if (map->layers[l]->objects[o]->OnTick)
				map->layers[l]->objects[o]->OnTick();
		}
	}
	for (size_t c = 0; c < map->cameras.size(); c++)
	{
		if (map->cameras[c]->OnTick)
			map->cameras[c]->OnTick();
	}
}

void Engine::GameLoop(unsigned int ticksPerSecond)
{
	InitializeConsole(L"KCGE Game");

	srand(time(NULL));
	
	if (ticksPerSecond == 0)
		tps = 10000;
	else
		tps = ticksPerSecond;

	long tickLengthMicrosec, l, o;

	// First step: call user defined functions. First input events functions, then OnTick.
	// Second step: process collision on the active map. This will move, push and auto-unstuck objects. /!\ Collision is disabled in this version of the engine.
	// Third step: render what's going on in the active map through the active camera.
	// Fourth step: handle time. If the duration of the current tick is shorter than the expected tick length (tickLengthMicrosec), delay will be set to the remaining time so the game's speed will be consistent.

	while (running)
	{
		thisTickStartTP.SetToNow();

		ManageInputs();
		
		tickLengthMicrosec = 1000000.0f / tps;

		if (GlobalOnTick)
			GlobalOnTick();

		if (activeMap)
		{
			if (activeMap->OnTick) 
				activeMap->OnTick();

			for (l = 0; l < activeMap->layers.size(); l++)
			{
				if (activeMap->layers[l]->OnTick)
					activeMap->layers[l]->OnTick();

				for (o = 0; o < activeMap->layers[l]->objects.size(); o++)
				{
					if (activeMap->layers[l]->objects[o]->OnTick)
						activeMap->layers[l]->objects[o]->OnTick();
				}
			}

			if (activeMap->activeCameraI > -1 && activeMap->activeCameraI < activeMap->cameras.size())
			{
				if (activeMap->cameras[activeMap->activeCameraI]->OnTick)
					activeMap->cameras[activeMap->activeCameraI]->OnTick();
				
				activeMap->cameras[activeMap->activeCameraI]->Render(activeMap->layers);
				ResetConsole();
				activeMap->cameras[activeMap->activeCameraI]->Print();
			}
		}

		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - thisTickStartTP.chornoTimePoint).count();
		potentialfps = 1000000.0f / deltaTime;

		while (deltaTime <= tickLengthMicrosec - 1)
			deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - thisTickStartTP.chornoTimePoint).count();

		fps = 1000000.0f / deltaTime;

		totalTicks++;
	}
}