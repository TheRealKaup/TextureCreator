/*

	  || /   ----  ----  ----
	 |||/\   |     |  _  |--
	||||  \  \___  \__/  \___  kaup's console game engine
	
	Docs will be available at https://www.kaup.dev/kcge. Capybaright � 2021-2022 Kaup. All rights reserved.

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
	struct Texture;
	struct Collider;
	struct Object;
	struct Layer;
	struct Camera;
	struct Map;
	struct TimePoint;

	extern std::vector<std::vector<unsigned char>> colliderTypes; /* Example:
	0 - Can't push | 1 - Can push | 2 - Can overlap
	{
		  A  B, C <- Being collided
	  A { 0, 1, 2 }, <- Moving
	  B { 0, 1, 2 },
	  C { 2, 2, 2 }
	}
	// A Can't be pushed, can push (Heavy)
	// B Can be pushed, can push (Normal)
	// C Can be pushed, can't push (Light) */

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

	// The final image that will be printed. This allows multiple cameras to print at the same time
	extern std::vector<std::vector<Pixel>> image;
	// Instead of allocating a string (that will be printed to the console) every print, it is kept for next prints.
	extern std::string stringImage;

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
		Vector2D(int x = 0, int y = 0);
	};

	struct UVector2D
	{
		unsigned int x, y;
		UVector2D(unsigned int x = 0, unsigned int y = 0);
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

	struct Texture
	{
		bool active = true;

		std::vector<std::vector<SuperChar>> t;
		Vector2D pos;

		// LATER: Change() which instead of going through the whole process of creating a new texture to change the texture, easy function viola

		// Block of the same character and color.
		Texture(UVector2D size = {0, 0}, char defaultChar = ' ', RGBA frgba = {255, 255, 255, 1.0f}, RGBA brgba = {0, 0, 0, 0.0f}, Vector2D pos = {0, 0});
		// Actual texture - 2D vector of SuperChar.
		Texture(std::vector<std::vector<SuperChar>> texture, Vector2D pos = {0, 0});
		// Load from a file.
		Texture(std::string fileName, Vector2D pos = { 0, 0 });
		// Easy to create texture using a string vector.
		Texture(std::vector<std::string> stringVector, RGBA frgba, RGBA brgba, Vector2D pos = { 0, 0 });
	};

	struct Collider
	{
		Object* parentObject = NULL;
		// A simple collider is a rectangle.
		bool simple;
		// Used if the collider is complex.
		std::vector<std::vector<bool>> c;
		// Used by both simple and complex colliders.
		Vector2D pos;
		// Used if the collider is simple.
		UVector2D size;

		// Turn the collider on/off.
		bool active = true;
		// The collider type which is defined by Engine::colliderTypes
		unsigned char type = 0;

		// For a simple collider
		Collider(UVector2D size = {1, 1}, Vector2D pos = {0, 0});
		// For a complex collider
		Collider(std::vector<std::vector<bool>> collider, Vector2D pos = { 0, 0 });
	};

	class Object
	{
	public:
		Layer* parentLayer = NULL; 
		
		Object(Vector2D position = { 0, 0 }, std::string objectName = "Unnamed Object");

		std::function<void()> OnTick;
		
		std::string name;
		Vector2D pos;

		std::vector<Texture> textures;
		std::vector<Collider> colliders;

		// bool colliderActive = true;
		// std::vector<std::vector<bool>> collider;
		// int colliderType = 0;

		Object* theOtherObject = NULL;
		Vector2D lastPush;
		int theColliderIndex = -1;
		int theOtherColliderIndex = -1;

		// This event can be called after calling Move. OnPushed means that another object pushed this one.
		std::function<void()> OnPushed;
		// This event can be called after calling Move. OnPush means that this objects pushed another object.
		std::function<void()> OnPush;
		// This event can be called after calling Move. OnBlocked means that another objects blocked this one.
		std::function<void()> OnBlocked;
		// This event can be called after calling Move. OnBlock means that this object blocked another object.
		std::function<void()> OnBlock;
		
		std::vector<long int> data; // user defined data, for sharing between classes and functions

		bool Move(Vector2D dir);

		bool CanMove(Vector2D dir, std::vector<Object*>* pushingObjects = nullptr, std::vector<Object*>* objectsToPush = nullptr, std::vector<Object*>* blockingObjectd = nullptr); // mostly engine used function (specifically block-moving)
	};

	class Layer
	{
	public:
		bool active = true;

		std::function<void()> OnTick;

		std::vector<Object*> objects;
		
		RGBA frgba = { 0, 0, 0, 0.0f };
		RGBA brgba = { 0, 0, 0, 0.0f };
		float transparency = 1.0f;

		int AddObject(Object* object);
		bool RemoveObject(int index);
		bool RemoveObject(std::string name);
	};

	class Camera
	{
	public:
		std::function<void()> OnTick;
		std::string name;
		Vector2D pos;
		Vector2D res;
		std::vector<std::vector<Pixel>> image;

		Camera(Vector2D position = Vector2D{ 0, 0 }, Vector2D resolution = { 10, 10 }, std::string cameraName = "Unnamed Camera");
		
		void Render(std::vector<Layer*> layers);
		// This function will draw what was rendered (Camera::image's contents) into the "final" image (Engine::image), which will be printed at once with Engine::Print.
		// pos is the position desired to draw the rendered image to the final image.
		// left, top, right and bottom are a rectangle representing the area of the rendered image desired to draw to the final image.
		// Setting right/bottom to 0 will default to the rendered image's size.
		void Draw(Vector2D pos = {0, 0}, unsigned short left = 0U, unsigned short top = 0U, unsigned short right = 0U, unsigned short bottom = 0U);
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

	// An easy to use time point which uses std::chrono.
	// If you want to measure the time passed between 2 points in your code, you use 2 time points at those points and subtract the last one by the first one, which will get you the time passed between those 2 time points.
	struct TimePoint
	{
		// The std::chrono time point
		std::chrono::high_resolution_clock::time_point chornoTimePoint;
		
		// The default constructor will set the time point to the time it was created
		TimePoint();

		// Will reset the time point to the current time
		void SetToNow();

		// Get an int which represents the time point in seconds
		int Seconds();
		// Get an int which represents the time point in milliseconds
		int Milliseconds();
		// Get an int which represents the time point in microseconds
		int Microseconds();
		// Get an int which represents the time point in nanoseconds
		int Nanoseconds();
	};

	bool AreObjectsOverlapping(Engine::Object* objA, Engine::Object* objB, bool ignoreIrrelevancy = false);

	// Use ASCII. For Virtual-Key Codes set vk to true, which if set, will not try to translte from ASCII to VK.
	void RegisterInputHandler(int key, bool onKeyDown, std::function<void()> function, bool vk = false);

	// Storage, could be used for many things.
	extern std::vector<Object> storedObjects;
	extern std::vector<Layer> storedLayers;
	// If you don't want to store an object yourself, add it to this function which will store it for you. Add the returned pointer to the desired layer.
	// This is used by the engine when you load a map.
	Object* StoreObject(Object object);
	// If you don't want to store a layer yourself, add it to this function which will store it for you. Add the returned pointer to the desired map.
	// This is used by the engine when you load a map.
	Layer* StoreLayer(Layer layer);

	/* Functions for custom game loop. */
	// Prepare the console for first usage.
	void InitializeConsole(unsigned short fontX = 16, unsigned short fontY = 16, unsigned short columns = 20, unsigned short rows = 20, const wchar_t* title = L"Dope KCGE Game");
	// NOTE - THIS FUNCTION IS UNRELIABLE, GLITCHY, AND UNRECOMMENDED.
	// Change the console's amount of columns and rows.
	void ResizeConsole(unsigned short columns = 50, unsigned short rows = 30);
	// Change the console's font size by pixels
	void ResizeConsoleFont(unsigned short x = 16, unsigned short y = 16);
	// Call OnKey functions according to key states.
	void ManageInputs();
	// Print the final image to the console.
	void Print();
	// Wait the precise amount of time to fill the frame according to the ticks per second value.
	void WaitUntilNextTick();
	// Give this function a map and it will call all the OnKey functions of the layers, objects and cameras (map itself included) in the map.
	// This way, you don't have to write the very long and ugly for loops to do so.
	void CallOnTicks(Engine::Map* map);

	// Default game loop
	void GameLoop(unsigned int ticksPerSecond);
};