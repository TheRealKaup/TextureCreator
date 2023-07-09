#include "engine/engine.hpp"
#include "widgets/button.hpp"
#include "widgets/canvas.hpp"
#include "widgets/intinputfield.hpp"
#include "widgets/stringinputfield.hpp"
#include "widgets/switch.hpp"

#define displaySizeX 101
#define displaySizeY 39

void Log(const std::string& text)
{
	static uint32_t logI = 0;
	std::cout << "\033[38;2;150;50;255m" << logI << "] " << text << "\033[38;2m" << std::endl << std::flush;
	logI++;
}

using namespace Engine;

void Exit() { running = false; }

void SwitchButton(Button* button) {
	if (button->unselectedRGBA.r != 150)
		button->unselectedRGBA = { 150, 150, 150, 255 };
	else
		button->unselectedRGBA = { 75, 75, 75, 255 };
}

namespace UI
{
	// States
	enum class SectionState{
		canvas,
		top,
		side
	};
	SectionState sectionState = SectionState::canvas;
	uint8_t topState = 0;
	uint8_t sideState = 0;

	// Audio
	AudioSource widgetSelectionSFX;

	// Widgets
	Canvas* canvas;
	Widget* topSection[6];
	// 0 - (Button) Exit
	// 1 - (Button) Import
	// 2 - (Button) Export
	// 3 - (Int field) Canvas size X
	// 4 - (Int field) Canvas size Y
	// 5 - (Button) Confirm canvas size
	Widget* sideSection[16];
	// 0 - (Int field) Foreground R
	// 1 - (Int field) Foreground G
	// 2 - (Int field) Foreground B
	// 3 - (Int field) Foreground A
	// 4 - (Int field) Background R
	// 5 - (Int field) Background G
	// 6 - (Int field) Background B
	// 7 - (Int field) Background A
	// 8 - (String field) Character
	// 9 - (Int field) Brush size X
	// 10 - (Int field) Brush size Y
	// 11 - (Button) Reset brush position
	// 12 - (Switch) Foreground switch
	// 13 - (Switch) Background switch
	// 14 - (Switch) Character switch
	// 15 - (Int field) Canvas background

	// Frames
	Object mainFrame;
	Object foregroundFrame;
	Object backgroundFrame;
	Object settingsFrame;
	Object backgroundSelectorFrame;

	void ResizeCanvas()
	{
		canvas->Resize(Engine::UPoint(((IntInputField*)topSection[3])->number, ((IntInputField*)topSection[4])->number));
	}

	void StateMachine()
	{
		bool moved = false;
		switch (sectionState)
		{
			case SectionState::canvas:
			{
				// From canvas to top
				if (Input::Is(kUp))
				{
					canvas->Deselect();
					topSection[topState]->Select();
					sectionState = SectionState::top;
					moved = true;
				}
				// From canvas to side
				else if (Input::Is(kLeft))
				{
					canvas->Deselect();
					sideSection[sideState]->Select();
					sectionState = SectionState::side;
					moved = true;
				}
				break;
			}
			case SectionState::top:
			{
				// A widget left
				if (Input::Is(kLeft) && topState > 0)
				{
					topSection[topState]->Deselect();
					topSection[--topState]->Select();
					moved = true;
				}
				// A widget right
				else if (Input::Is(kRight) && topState < sizeof(topSection) / sizeof(nullptr) - 1)
				{
					topSection[topState]->Deselect();
					topSection[++topState]->Select();
					moved = true;
				}
				// From top to canvas
				else if (Input::Is(kDown))
				{
					topSection[topState]->Deselect();
					canvas->Select();
					sectionState = SectionState::canvas;
					moved = true;
				}
				break;
			}
			case SectionState::side:
			{
				// A widget up
				if (Input::Is(kUp) && sideState > 0)
				{
					sideSection[sideState]->Deselect();
					sideSection[--sideState]->Select();
					moved = true;
				}
				// A widget down
				else if (Input::Is(kDown) && sideState < sizeof(sideSection) / sizeof(nullptr) - 1)
				{
					sideSection[sideState]->Deselect();
					sideSection[++sideState]->Select();
					moved = true;
				}
				// From side to canvas
				else if (Input::Is(kRight))
				{
					sideSection[sideState]->Deselect();
					canvas->Select();
					sectionState = SectionState::canvas;
					moved = true;
				}
				break;
			}
		}

		// Play widget selection sound effect
		if (moved)
			widgetSelectionSFX.Play();
	}

	void Initialize(Layer* layer)
	{
		// Canvas
		canvas = new Canvas(UPoint(10, 10), Point(13, 4), 0, UPoint(3, 3), CellA('#', RGBA(255, 0, 0, 255), RGBA(0, 0, 255, 255)), layer);
		canvas->Select();
		// Up section
		topSection[0] = new Button(layer, Exit, kReturn, Point( 44, 1 ), "Exit", true); // (Button) Exit
		topSection[1] = new Button(layer, std::bind(&Canvas::Import, canvas), kReturn, Point( 51, 1 ), "Import", true); // (Button) Import
		topSection[2] = new Button(layer, std::bind(&Canvas::Export, canvas), kReturn, Point( 60, 1 ), "Export", true); // (Button) Export
		topSection[3] = new IntInputField(layer, nullptr, 1, 78, "16", Point( 69, 1 ), "CanvasX=", true); // (Int field) Canvas size X
		topSection[4] = new IntInputField(layer, nullptr, 1, 33, "16", Point( 80, 1 ), "CanvasY=", true); // (Int field) Canvas size Y
		topSection[5] = new Button(layer, ResizeCanvas, kReturn, Point( 91, 1 ), "Confirm", true); // (Button) Confirm canvas size
		// Side section
		sideSection[0] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 5 ), "R="); // (Int field) Foreground R
		sideSection[1] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 6 ), "G="); // (Int field) Foreground G
		sideSection[2] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 7 ), "B="); // (Int field) Foreground B
		sideSection[3] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 8 ), "A="); // (Int field) Foreground A
		sideSection[4] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 13 ), "R="); // (Int field) Background R
		sideSection[5] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 14 ), "G="); // (Int field) Background G
		sideSection[6] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 15 ), "B="); // (Int field) Background B
		sideSection[7] = new IntInputField(layer, nullptr, 0, 255, "255", Point( 2, 16 ), "A="); // (Int field) Background A
		sideSection[8] = new StringInputField(layer, nullptr, { Characters_All }, { 2, 20 }, "Char=", 1, "#", true); // (String field) Character
		sideSection[9] = new IntInputField(layer, nullptr, 1, 9, "1", { 2, 25 }, "BrushX="); // (Int field) Brush size X
		sideSection[10] = new IntInputField(layer, nullptr, 1, 9, "1", { 2, 26 }, "BrushY="); // (Int field) Brush size Y
		sideSection[11] = new Button(layer, nullptr, kReturn, { 2, 27 }, "Reset Pos"); // (Button) Reset brush position
		sideSection[12] = new Switch(layer, NULL, kReturn, { 2, 28 }, "Fore Tool", true); // (Switch) Foreground switch
		sideSection[13] = new Switch(layer, NULL, kReturn, { 2, 29 }, "Back Tool", true); // (Switch) Background switch
		sideSection[14] = new Switch(layer, NULL, kReturn, { 2, 30 }, "Char Tool", true); // (Switch) Character switch
		sideSection[15] = new IntInputField(layer, nullptr, 1, 3, "1", { 2, 34 }, "Back=", true); // (Int field) Canvas background
		// Audio
		widgetSelectionSFX.LoadWavFile("assets/widgetsMove.wav"); // Widget selection sound effect
		// Frames
		mainFrame.textures.resize(3);
		mainFrame.textures[0].Write( // Logo
			{
			"  ||  /   ----- ----- ----- -   - ___ ___ /",
			" ||| /\\     |   |---  |     |---|  |  |   |",
			"||||/  \\    |   \\____ \\____ |   |  |  \\__ |",
			"/-----------------------------------------/",
			}, RGBA( 87, 166, 75, 255 ), RGBA( 0, 0, 0, 0 ), Point( 0, 0 ));
		mainFrame.textures[1].Simple({ 100, 1 }, CellA('-', { 87, 166, 75 }, { 0, 0, 0 }), { 43, 0 }); // Top frame
		mainFrame.textures[2].Simple({ 1, 100 }, CellA('|', { 87, 166, 75 }, { 0, 0, 0 }), {0, 4}); // Side frame
		layer->AddObject(&mainFrame);
		foregroundFrame.pos = Point(2, 4);
		foregroundFrame.textures.resize(9);
		foregroundFrame.textures[0].Write({"Foreground"}, RGBA(150 ,150, 150, 255), RGBA(0, 0, 0, 0), Point(0, 0)); // Text
		foregroundFrame.textures[1].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 1)); // Up-left corner
		foregroundFrame.textures[2].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(6, 1)); // Up-right corner
		foregroundFrame.textures[3].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 6)); // Bottom-left corner
		foregroundFrame.textures[4].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(6, 6)); // Bottom-right corner
		foregroundFrame.textures[5].Simple(UPoint(5, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 1)); // Up frame
		foregroundFrame.textures[6].Simple(UPoint(1, 4), CellA('|', RGBA(150 ,150, 150, 255)), Point(0, 2)); // Left frame
		foregroundFrame.textures[7].Simple(UPoint(5, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 6)); // Bottom frame
		foregroundFrame.textures[8].Simple(UPoint(1, 4), CellA('|', RGBA(150 ,150, 150, 255)), Point(6, 2)); // Right frame
		layer->AddObject(&foregroundFrame);
		backgroundFrame.pos = Point(2, 12);
		backgroundFrame.textures.resize(9);
		backgroundFrame.textures[0].Write({"Background"}, RGBA(150 ,150, 150, 255), RGBA(0, 0, 0, 0), Point(0, 0)); // Text
		backgroundFrame.textures[1].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 1)); // Up-left corner
		backgroundFrame.textures[2].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(6, 1)); // Up-right corner
		backgroundFrame.textures[3].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 6)); // Bottom-left corner
		backgroundFrame.textures[4].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(6, 6)); // Bottom-right corner
		backgroundFrame.textures[5].Simple(UPoint(5, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 1)); // Up frame
		backgroundFrame.textures[6].Simple(UPoint(1, 4), CellA('|', RGBA(150 ,150, 150, 255)), Point(0, 2)); // Left frame
		backgroundFrame.textures[7].Simple(UPoint(5, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 6)); // Bottom frame
		backgroundFrame.textures[8].Simple(UPoint(1, 4), CellA('|', RGBA(150 ,150, 150, 255)), Point(6, 2)); // Right frame
		layer->AddObject(&backgroundFrame);
		settingsFrame.pos = Point(2, 24);
		settingsFrame.textures.resize(9);
		settingsFrame.textures[0].Write({"Background"}, RGBA(150 ,150, 150, 255), RGBA(0, 0, 0, 0), Point(0, 0)); // Text
		settingsFrame.textures[1].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 1)); // Up-left corner
		settingsFrame.textures[2].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(10, 1)); // Up-right corner
		settingsFrame.textures[3].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(0, 8)); // Bottom-left corner
		settingsFrame.textures[4].Simple(UPoint(1, 1), CellA('#', RGBA(150 ,150, 150, 255)), Point(10, 8)); // Bottom-right corner
		settingsFrame.textures[5].Simple(UPoint(9, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 1)); // Up frame
		settingsFrame.textures[6].Simple(UPoint(1, 6), CellA('|', RGBA(150 ,150, 150, 255)), Point(0, 2)); // Left frame
		settingsFrame.textures[7].Simple(UPoint(9, 1), CellA('-', RGBA(150 ,150, 150, 255)), Point(1, 8)); // Bottom frame
		settingsFrame.textures[8].Simple(UPoint(1, 6), CellA('|', RGBA(150 ,150, 150, 255)), Point(10, 2)); // Right frame
		layer->AddObject(&settingsFrame);
		backgroundSelectorFrame.pos = Point(2, 37);
		backgroundSelectorFrame.textures.resize(1);
		backgroundSelectorFrame.textures[0].File("assets/backgroundSelectorFrame.kcget", Point(0, 0));
		layer->AddObject(&backgroundSelectorFrame);
		// Input handlers
		Input::RegisterHandler(kUp, StateMachine, true);
		Input::RegisterHandler(kLeft, StateMachine, true);
		Input::RegisterHandler(kDown, StateMachine, true);
		Input::RegisterHandler(kRight, StateMachine, true);
	}
}

int main()
{
	InitializeAudio();
	PrepareTerminal(UPoint(displaySizeX, displaySizeY));
	std::thread t_inputLoop(Input::Loop);

	// Map, layer and camera
	Map map;
	Layer layer;
	Layer backlayer;
	Camera camera(Point(0, 0), UPoint(displaySizeX, displaySizeY));
	map.AddLayer(&backlayer);
	map.AddLayer(&layer);
	map.AddCamera(&camera, true);

	UI::Initialize(&layer);

	// Loop
	for(Time::tps = 60; running; Time::StartThisTick())
	{
		// Callback functions calling
		Input::Call(); // Input events
		map.CallOnTicks(); // OnTicks 
		Time::CallInvocations(); // Invocations made by Time::Invoke()

		// Render, draw and print
		map.cameras[map.activeCameraI]->Render(map.layers);
		map.cameras[map.activeCameraI]->Draw({ 0, 0 }, 0, 0, 0, 0);
		Print();

		Time::WaitUntilNextTick();
	}

	TerminateAudio();
	ResetTerminal();
	exit(0);
}