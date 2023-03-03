﻿#include "Engine.h"

// ---=== Engine Variables ===---

// Objects
Engine::Map* Engine::activeMap = NULL;
// Data
std::vector<Engine::Object> Engine::storedObjects = {};
std::vector<Engine::Layer> Engine::storedLayers = {};
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
std::function<void()> Engine::GlobalOnTick = NULL;
bool Engine::running = true;
// Physics
std::vector<std::vector<unsigned char>> Engine::colliderTypes = {
	{ 0, 1, 2 }, // Heavy - 0
	{ 0, 1, 2 }, // Normal - 1
	{ 2, 2, 2 } // Overlappable - 2
};
// Console
std::vector<std::vector<Engine::Pixel>> Engine::image = {};
std::string Engine::stringImage = "";

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

void Engine::InitializeConsole(unsigned fontX, unsigned fontY, unsigned columns, unsigned rows, std::wstring fontName, std::wstring title)
{
	// if size == {0, 0}, don't change window size at all.
	//		if columns == 0, use c. 
	//		if rows == 0, use r. 
	// if font == {0, 0} && fontName.length() == 0, don't change font size at all.
	//		if fontX == 0, use fX.
	//		if fontY == 0, use fY.
	//		if fontName.length() == 0, use lastFontName.
	// if title.length() == 0, don't change title at all

	static unsigned lastFontY = 16;
	static unsigned lastFontX = 16;
	static unsigned lastColumns = 30;
	static unsigned lastRows = 30;
	static std::wstring lastFontName = L"Consolas";

	// Enable UTF-8
	SetConsoleOutputCP(65001);
	
	// Enable virtual sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hOut, &mode);
	SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	
	// Set cursor at 0,0
	std::cout << "\033[0;0H";
	// Hide cursor
	std::cout << "\033[?25l";

	// Resize the engine's image
	if (rows > 0 || columns > 0) // if non are set to change, don't change at all.
	{
		// Got to this point if at least one of them are set to change.

		unsigned decidedColumns= columns > 0 ? columns : lastColumns;
		unsigned decidedRows = rows > 0 ? rows : lastRows;

		image.resize(decidedRows);

		for (size_t y = 0; y < image.size(); y++)
			image[y].resize(decidedColumns);

		// Change window size
		SMALL_RECT srect;
		srect.Left = 0;
		srect.Top = 0;
		srect.Right = decidedColumns;
		srect.Bottom = decidedRows;
		for (int i = 0; i < 10; i++) // Yes, seriously.
		{
			SetConsoleWindowInfo(hOut, true, &srect);
			SetConsoleScreenBufferSize(hOut, { (short)(decidedColumns + 1), (short)(decidedRows + 1) });
		}
	}
	
	// Change font
	if (fontX > 0 || fontY > 0 || fontName.length() > 0)
	{
		CONSOLE_FONT_INFOEX fontInfo;
		fontInfo.cbSize = sizeof(fontInfo);
		fontInfo.dwFontSize.X = fontX > 0 ? fontX : lastFontX;
		fontInfo.dwFontSize.Y = fontY > 0 ? fontY : lastFontY;
		fontInfo.FontFamily = FF_DONTCARE;
		fontInfo.FontWeight = FW_NORMAL;
		fontInfo.nFont = 0;
		std::wstring decidedFontName = fontName.length() > 0 ? fontName : lastFontName;
		for (size_t i = 0; i < LF_FACESIZE; i++)
		{
			if (i < decidedFontName.length())
				fontInfo.FaceName[i] = decidedFontName[i];
			else {
				fontInfo.FaceName[i] = '\0';
				break;
			}
		}
		SetCurrentConsoleFontEx(hOut, false, &fontInfo);
	}

	// Give it a title
	if (title.length() > 0)
		SetConsoleTitle(title.c_str());

	// Disable Shift+Arrow/mouse selection and Ctrl+C
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hIn, &mode);
	SetConsoleMode(hIn, mode & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_PROCESSED_INPUT);
	RegisterHotKey(GetConsoleWindow(), 0, MOD_ALT, VK_RETURN);
	// Hide the scrollbars
	ShowScrollBar(GetConsoleWindow(), SB_BOTH, false);
	// Disable the resizing of the console and the option for fullscreen
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX); // & ~WS_TILEDWINDOW

	
	// Update last values
	if (fontY > 0)
		lastFontY = fontY;
	if (fontX > 0)
		lastFontX = fontX;
	if (columns > 0)
		lastColumns = columns;
	if (rows > 0)
		lastRows = rows;
	if (fontName.length() > 0)
		lastFontName = fontName;
}
void Engine::Print()
{
	if (image.size() == 0)
		return;
	int maxStringSize = image.size();
	for (int y = 0; y < image.size(); y++)
		maxStringSize += image[y].size() * 39;
	if (maxStringSize == 0)
		return;

	if (stringImage.size() != maxStringSize)
		stringImage.resize(maxStringSize);

	unsigned int l = 0;
	unsigned char lfr = 0, lfg = 0, lfb = 0, lbr = 0, lbg = 0, lbb = 0;

	lfr = image[0][0].frgb.r;
	lfg = image[0][0].frgb.g;
	lfb = image[0][0].frgb.b;
	lbr = image[0][0].brgb.r;
	lbg = image[0][0].brgb.g;
	lbb = image[0][0].brgb.b;
	stringImage[l] = '\033';
	stringImage[l + 1] = '[';
	stringImage[l + 2] = '3';
	stringImage[l + 3] = '8';
	stringImage[l + 4] = ';';
	stringImage[l + 5] = '2';
	stringImage[l + 6] = ';';
	stringImage[l + 7] = lfr / 100 + '0';
	stringImage[l + 8] = (lfr % 100) / 10 + '0';
	stringImage[l + 9] = lfr % 10 + '0';
	stringImage[l + 10] = ';';
	stringImage[l + 11] = lfg / 100 + '0';
	stringImage[l + 12] = (lfg % 100) / 10 + '0';
	stringImage[l + 13] = lfg % 10 + '0';
	stringImage[l + 14] = ';';
	stringImage[l + 15] = lfb / 100 + '0';
	stringImage[l + 16] = (lfb % 100) / 10 + '0';
	stringImage[l + 17] = lfb % 10 + '0';
	stringImage[l + 18] = 'm';
	l += 19;
	stringImage[l] = '\033';
	stringImage[l + 1] = '[';
	stringImage[l + 2] = '4';
	stringImage[l + 3] = '8';
	stringImage[l + 4] = ';';
	stringImage[l + 5] = '2';
	stringImage[l + 6] = ';';
	stringImage[l + 7] = lbr / 100 + '0';
	stringImage[l + 8] = (lbr % 100) / 10 + '0';
	stringImage[l + 9] = lbr % 10 + '0';
	stringImage[l + 10] = ';';
	stringImage[l + 11] = lbg / 100 + '0';
	stringImage[l + 12] = (lbg % 100) / 10 + '0';
	stringImage[l + 13] = lbg % 10 + '0';
	stringImage[l + 14] = ';';
	stringImage[l + 15] = lbb / 100 + '0';
	stringImage[l + 16] = (lbb % 100) / 10 + '0';
	stringImage[l + 17] = lbb % 10 + '0';
	stringImage[l + 18] = 'm';
	l += 19;

	for (int y = 0; y < image.size(); y++)
	{
		for (int x = 0; x < image[y].size(); x++)
		{
			if (image[y][x].character != ' ')
			{
				if (image[y][x].frgb.r != lfr || image[y][x].frgb.g != lfg || image[y][x].frgb.b != lfb)
				{
					lfr = image[y][x].frgb.r;
					lfg = image[y][x].frgb.g;
					lfb = image[y][x].frgb.b;

					stringImage[l] = '\033';
					stringImage[l + 1] = '[';
					stringImage[l + 2] = '3';
					stringImage[l + 3] = '8';
					stringImage[l + 4] = ';';
					stringImage[l + 5] = '2';
					stringImage[l + 6] = ';';
					stringImage[l + 7] = lfr / 100 + '0';
					stringImage[l + 8] = (lfr % 100) / 10 + '0';
					stringImage[l + 9] = lfr % 10 + '0';
					stringImage[l + 10] = ';';
					stringImage[l + 11] = lfg / 100 + '0';
					stringImage[l + 12] = (lfg % 100) / 10 + '0';
					stringImage[l + 13] = lfg % 10 + '0';
					stringImage[l + 14] = ';';
					stringImage[l + 15] = lfb / 100 + '0';
					stringImage[l + 16] = (lfb % 100) / 10 + '0';
					stringImage[l + 17] = lfb % 10 + '0';
					stringImage[l + 18] = 'm';
					l += 19;
				}
			}
			if (image[y][x].brgb.r != lbr || image[y][x].brgb.g != lbg || image[y][x].brgb.b != lbb)
			{
				lbr = image[y][x].brgb.r;
				lbg = image[y][x].brgb.g;
				lbb = image[y][x].brgb.b;

				if (lbr == 0 && lbg == 0 && lbb == 0)
				{
					stringImage[l] = '\033';
					stringImage[l + 1] = '[';
					stringImage[l + 2] = '4';
					stringImage[l + 3] = '8';
					stringImage[l + 4] = ';';
					stringImage[l + 5] = '2';
					stringImage[l + 6] = ';';
					stringImage[l + 7] = '0';
					stringImage[l + 8] = ';';
					stringImage[l + 9] = '0';
					stringImage[l + 10] = ';';
					stringImage[l + 11] = '0';
					stringImage[l + 12] = 'm';
					l += 13;
				}
				else
				{
					stringImage[l] = '\033';
					stringImage[l + 1] = '[';
					stringImage[l + 2] = '4';
					stringImage[l + 3] = '8';
					stringImage[l + 4] = ';';
					stringImage[l + 5] = '2';
					stringImage[l + 6] = ';';
					stringImage[l + 7] = lbr / 100 + '0';
					stringImage[l + 8] = (lbr % 100) / 10 + '0';
					stringImage[l + 9] = lbr % 10 + '0';
					stringImage[l + 10] = ';';
					stringImage[l + 11] = lbg / 100 + '0';
					stringImage[l + 12] = (lbg % 100) / 10 + '0';
					stringImage[l + 13] = lbg % 10 + '0';
					stringImage[l + 14] = ';';
					stringImage[l + 15] = lbb / 100 + '0';
					stringImage[l + 16] = (lbb % 100) / 10 + '0';
					stringImage[l + 17] = lbb % 10 + '0';
					stringImage[l + 18] = 'm';
					l += 19;
				}
			}

			stringImage[l] = image[y][x].character;
			l++;
		}
		stringImage[l] = '\n';
		l++;
	}
	std::cout << "\033[0;0H\033[?25l" << stringImage.substr(0, l) << "\033[0;0H";
}
void Engine::WaitUntilNextTick()
{
	Engine::deltaTime = NowInMicroseconds() - Engine::thisTickStartTP.Microseconds();
	Engine::potentialfps = 1000000.0f / Engine::deltaTime;
	while (Engine::deltaTime < 1000000 / Engine::tps)
		Engine::deltaTime = NowInMicroseconds() - Engine::thisTickStartTP.Microseconds();
	Engine::fps = 1000000.0f / Engine::deltaTime;
}
void Engine::CallOnTicks(Engine::Map* map)
{
	if (Engine::GlobalOnTick)
		Engine::GlobalOnTick();

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