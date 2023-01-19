/*

	  || /   ----  ----  ----
	 |||/\   |     |  _  |--
	||||  \  \___  \__/  \___  kaup's console game engine
	
	Docs will be available at https://www.kaup.dev/kcge. Capybaright © 2021-2022 Kaup. All rights reserved.

*/

#pragma once

#include <Windows.h> // Console related
#include <vector> // Heavily based
#include <iostream> // Printing, strings, et cetera
#include <functional> // For sharing functions with the engine for it to call on events
#include <thread> // For sleeping, time managing.
#include <string> // to_string
#include <fstream> // Loading textures

#include "Config.h"

namespace Engine
{
	struct RGB;
	struct RGBA;
	struct Vector2D;
	struct Pixel;
	struct SuperChar;
	struct Object;
	struct Layer;
	struct Camera;
	struct Map;
	struct TimePoint;

	extern std::vector<std::vector<int>> colliderTypes; /* Example:
	0 - Can't push | 1 - Can push | 2 - Can overlap
	{
		  0  1, 2 <- Being collided
	  0 { 0, 1, 2 }, <- Moving
	  1 { 0, 1, 2 },
	  2 { 0, 0, 2 }
	}
	// 0 Can't be pushed, can push (Heavy)
	// 1 Can be pushed, can push (Normal)
	// 2 Can be pushed, can't push (Light) */

	extern int lastDownKey;
	extern int lastUpKey;
	extern int lastDownVirtualKey;
	extern int lastUpVirtualKey;
	// When you register a key that requires shift (for example '!' and 'W'), it will set lastDownKeyRequiresShift and lastUpKeyRequiresShift when triggered.
	// Important: the engine will not check if shift is down if a key requires it. The user needs to check it themselves. The engien doesn't do it because it's way too complex, 
	// resource costly and mostly unnecessary.
	extern bool lastDownKeyRequiresShift;
	extern bool lastUpKeyRequiresShift;

	extern int tps;
	extern long int deltaTime;
	extern TimePoint thisTickStartTP;
	extern float potentialfps;
	extern float fps;
	extern TimePoint engineStartTP;
	extern long int totalTicks;

	extern std::function<void()> GlobalOnTick;
	extern Map* activeMap;
	extern bool running;

	extern wchar_t* consoleBuffer;

	struct RGB
	{
		unsigned char r, g, b;
		RGB(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0);
	};

	struct RGBA
	{
		unsigned char r, g, b;
		float a;
		RGBA(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, float alpha = 1.0f);
	};

	struct Vector2D
	{
		int x, y;
		Vector2D(int xAxis = 0, int yAxis = 0);
	};

	struct Pixel
	{
		char32_t character;
		RGB frgb;
		RGB brgb;
		Pixel(char32_t character = ' ', RGB foreRGB = {0, 0, 0}, RGB backRGB = { 0, 0, 0 });
	};

	struct SuperChar
	{
		char32_t character;
		RGBA frgba;
		RGBA brgba;
		SuperChar(char32_t character = ' ', RGBA foreRGBA = {255, 255, 255, 1.0f}, RGBA backRGBA = {0, 0, 0, 0.0f});
	};

	class Object
	{
	public:
		Layer* parentLayer = NULL; 
		
		Object(std::vector<std::string> texture = { "" }, Vector2D position = { 0, 0 }, RGBA frgba = {255, 255, 255, 1.0f}, RGBA brgba = {0, 0, 0, 0.0f}, std::string objectName = "Unnamed Object");

		std::function<void()> OnTick;

		std::vector<std::vector<SuperChar>> texture;
		std::string name;
		Vector2D pos;

		bool colliderActive = true;
		std::vector<std::vector<bool>> collider;
		int colliderType = 0;

		std::function<void()> OnCollision;
		std::function<void()> OnPushed;
		Object* lastCollidedObject = NULL;
		Vector2D lastPush;
		
		std::vector<long int> data; // user defined data, for sharing between classes and functions

		bool Move(Vector2D dir, Object* pushingObject = NULL);

		bool CanMove(Vector2D dir, std::vector<Object*>* pushingObjects = { NULL }, std::vector<Object*>* objectsToPush = {NULL}); // mostly engine used function (specifically block-moving)
		int debug_relevantCollidersInArea;
	};

	class Visual
	{
	public:
		Layer* parentLayer = NULL; 
		
		Visual(std::vector<std::string> texture = { "" }, Vector2D position = { 0, 0 }, RGBA frgba = {255, 255, 255, 1.0f}, RGBA brgba = {0, 0, 0, 0.0f});

		std::vector<std::vector<SuperChar>> texture;
		Vector2D pos;
	};

	class Layer
	{
	public:
		bool active = true;

		std::function<void()> OnTick;

		std::vector<Object*> objects;
		std::vector<Visual*> visuals;
		
		RGBA frgba = { 0, 0, 0, 0.0f };
		RGBA brgba = { 0, 0, 0, 0.0f };
		float transparency = 1.0f;

		int AddObject(Object* object);
		int AddVisual(Visual* visual);
		bool RemoveObject(size_t index);
		bool RemoveObject(std::string name);
		bool RemoveVisual(size_t index);
	};

	class Camera
	{
	public:
		std::function<void()> OnTick;
		std::string name;
		Vector2D pos;
		Vector2D res;
		std::string stringImage;
		std::vector<std::vector<Pixel>> image;

		Camera(Vector2D position = Vector2D{ 0, 0 }, Vector2D resolution = { 10, 10 }, std::string cameraName = "Unnamed Camera");
		
		void Render(std::vector<Layer*> layers);
		void Print();
	};
	
	class Map
	{
	public:
		std::function<void()> OnTick;
		std::vector<Camera*> cameras;
		std::vector<Layer*> layers;
		int activeCameraI = -1;

		int AddCamera(Camera* camera, bool asActiveCamera = false);
		int AddLayer(Layer* layer);
	};

	struct TimePoint
	{
		std::chrono::high_resolution_clock::time_point chornoTimePoint;
		
		TimePoint();

		void SetToNow();

		int Seconds();
		int Milliseconds();
		int Microseconds();
		int Nanoseconds();
	};

	// Misc
	bool AreObjectsOverlapping(Engine::Object* objA, Engine::Object* objB, bool ignoreIrrelevancy = false);
	void SetConsoleSizePixels(unsigned int sizeX, unsigned int sizeY);
	void SetConsoleFontSize(short int sizeX, short int sizeY);

	// Use ASCII. For Virtual-Key Codes set vk to true, which if set, will not try to translte from ASCII to VK.
	void RegisterInputHandler(int key, bool onKeyDown, std::function<void()> function, bool vk = false);
	
	std::vector<std::vector<SuperChar>> CreateTexture(std::vector<std::string> stringVector, RGBA frgba = {255, 255, 255, 1.0f}, RGBA brgba = {0, 0, 0, 0.0f});
	std::vector<std::vector<SuperChar>> LoadTextureFromFile(std::string fileName);

	// Storage, could be used for many things.
	extern std::vector<Object> storedObjects;
	extern std::vector<Layer> storedLayers;
	// If you don't want to store an object yourself, add it to this function which will store it for you. Add the returned pointer to the desired layer.
	// This is used by the engine when you load a map.
	Object* StoreObject(Object object);
	// If you don't want to store a layer yourself, add it to this function which will store it for you. Add the returned pointer to the desired map.
	// This is used by the engine when you load a map.
	Layer* StoreLayer(Layer layer);

	// Functions for custom game loop
	void Print();
	void ManageInputs();
	void ResetConsole();
	void InitializeConsole(const wchar_t* consoleTitle = L"KCGE Game");
	void WaitUntillNextTick();
	void CallOnTicks(Engine::Map* map);

	// Default game loop
	void GameLoop(unsigned int ticksPerSecond);
};