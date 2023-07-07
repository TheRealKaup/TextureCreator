#include "engine/engine.hpp"
#include "widgets/button.hpp"
#include "widgets/canvas.hpp"
#include "widgets/intinputfield.hpp"
#include "widgets/stringinputfield.hpp"

void Log(const std::string& text)
{
	static uint32_t logI = 0;
	std::cout << "\033[38;2;150;50;255m" << logI << "] " << text << "\033[38;2m" << std::endl << std::flush;
	logI++;
}

using namespace Engine;

// 0 = canvas, 1 = upper, 2 = side
unsigned int sectionState = 0;
unsigned int upState = 0;
unsigned int sideState = 0;

// Upper section
Button* pb_exit; // 0
Button* pb_import; // 1
Button* pb_export; // 2
IntInputField* pif_canvasX; // 3
IntInputField* pif_canvasY; // 4
Button* pb_confirmSize; // 5

// Side section
IntInputField* pif_fr; // 0
IntInputField* pif_fg; // 1
IntInputField* pif_fb; // 2
IntInputField* pif_fa; // 3
IntInputField* pif_br; // 4
IntInputField* pif_bg; // 5
IntInputField* pif_bb; // 6
IntInputField* pif_ba; // 7
StringInputField* psf_char; // 8
IntInputField* pif_brushSpeed; // 9
IntInputField* pif_brushX; // 10
IntInputField* pif_brushY; // 11
Button* pb_resetBrushPos; // 12
Button* pb_foreTool; // 13
Button* pb_backTool; // 14
Button* pb_charTool; // 15
IntInputField* pif_fontSize; // 16
IntInputField* pif_background; // 17

// Canvas

// Audio
AudioSource as_widgetsMove;

void Exit() { running = false; }


void SwitchButton(Button* button) {
	if (button->unselectedRGBA.r != 150)
		button->unselectedRGBA = { 150, 150, 150, 255 };
	else
		button->unselectedRGBA = { 75, 75, 75, 255 };
}

void StateMachine(unsigned char dir) // 0 = up, 1 = left, 2 = down, 3 = right
{
	/* upper:
	pb_exit; // 0
	pb_import; // 1
	pb_export; // 2
	pif_canvasX; // 3
	pif_canvasY; // 4
	pb_confirmSize; // 5
	*/

	// pif_fr; // 0
	// pif_fg; // 1
	// pif_fb; // 2
	// pif_fa; // 3
	// pif_br; // 4
	// pif_bg; // 5
	// pif_bb; // 6
	// pif_ba; // 7
	// psf_char; // 8
	// pif_brushSpeed; // 9
	// pif_brushX; // 10
	// pif_brushY; // 11
	// pb_resetBrushPos // 12
	// pb_foreTool; // 13
	// pb_backTool; // 14
	// pb_charTool; // 15
	// pif_fontSize; // 16
	// pif_background // 17

	// Canvas
	if (sectionState == 0)
	{
		// To upper
		if (dir == 0)
		{
			// DeselectCanvas();
			if (upState == 0) // exit button
				pb_exit->Select();
			else if (upState == 1) // import button
				pb_import->Select();
			else if (upState == 2) // export button
				pb_export->Select();
			else if (upState == 3) // canvas size x
				pif_canvasX->Select();
			else if (upState == 4) // canvas size y
				pif_canvasY->Select();
			else if (upState == 5) // confirm size
				pb_confirmSize->Select();
			sectionState = 1;
		}
		// To side
		else if (dir == 1)
		{
			// DeselectCanvas();
			if (sideState == 0) // fr
				pif_fr->Select();
			else if (sideState == 1) // fg
				pif_fg->Select();
			else if (sideState == 2) // fb
				pif_fb->Select();
			else if (sideState == 3) // fa
				pif_fa->Select();
			else if (sideState == 4) // br
				pif_br->Select();
			else if (sideState == 5) // bg
				pif_bg->Select();
			else if (sideState == 6) // bb
				pif_bb->Select();
			else if (sideState == 7) // ba
				pif_ba->Select();
			else if (sideState == 8) // char
				psf_char->Select();
			else if (sideState == 9) // brush speed
				pif_brushSpeed->Select();
			else if (sideState == 10) // brush speed
				pif_brushX->Select();
			else if (sideState == 11) // brush speed
				pif_brushY->Select();
			else if (sideState == 12)
				pb_resetBrushPos->Select();
			else if (sideState == 13)
				pb_foreTool->Select();
			else if (sideState == 14)
				pb_backTool->Select();
			else if (sideState == 15)
				pb_charTool->Select();
			else if (sideState == 16)
				pif_fontSize->Select();
			else if (sideState == 17)
				pif_background->Select();

			sectionState = 2;
		}
	}
	// Upper
	else if (sectionState == 1)
	{
		// Move left
		if (dir == 1)
		{
			if (upState == 1)
			{
				pb_import->Deselect();
				pb_exit->Select();
			}
			else if (upState == 2)
			{
				pb_export->Deselect();
				pb_import->Select();
			}
			else if (upState == 3)
			{
				pif_canvasX->Deselect();
				pb_export->Select();
			}
			else if (upState == 4)
			{
				pif_canvasY->Deselect();
				pif_canvasX->Select();
			}
			else if (upState == 5)
			{
				pb_confirmSize->Deselect();
				pif_canvasY->Select();
			}
			else
				return;
			upState--;
		}
		// To canvas
		else if (dir == 2)
		{
			if (upState == 0) // exit button
				pb_exit->Deselect();
			else if (upState == 1) // import button
				pb_import->Deselect();
			else if (upState == 2) // export button
				pb_export->Deselect();
			else if (upState == 3) // canvas size x
				pif_canvasX->Deselect();
			else if (upState == 4) // canvas size y
				pif_canvasY->Deselect();
			else if (upState == 5) // confirm size
				pb_confirmSize->Deselect();
			// SelectCanvas();
			sectionState = 0;
		}
		// Move right
		else if (dir == 3)
		{
			if (upState == 0)
			{
				pb_exit->Deselect();
				pb_import->Select();
			}
			else if (upState == 1)
			{
				pb_import->Deselect();
				pb_export->Select();
			}
			else if (upState == 2)
			{
				pb_export->Deselect();
				pif_canvasX->Select();
			}
			else if (upState == 3)
			{
				pif_canvasX->Deselect();
				pif_canvasY->Select();
			}
			else if (upState == 4)
			{
				pif_canvasY->Deselect();
				pb_confirmSize->Select();
			}
			else
				return;
			upState++;
		}
	}
	// Side
	else if (sectionState == 2)
	{
		// Move up
		if (dir == 0)
		{
			if (sideState == 1)
			{
				pif_fg->Deselect();
				pif_fr->Select();
			}
			else if (sideState == 2)
			{
				pif_fb->Deselect();
				pif_fg->Select();
			}
			else if (sideState == 3)
			{
				pif_fa->Deselect();
				pif_fb->Select();
			}
			else if (sideState == 4)
			{
				pif_br->Deselect();
				pif_fa->Select();
			}
			else if (sideState == 5)
			{
				pif_bg->Deselect();
				pif_br->Select();
			}
			else if (sideState == 6)
			{
				pif_bb->Deselect();
				pif_bg->Select();
			}
			else if (sideState == 7)
			{
				pif_ba->Deselect();
				pif_bb->Select();
			}
			else if (sideState == 8)
			{
				psf_char->Deselect();
				pif_ba->Select();
			}
			else if (sideState == 9)
			{
				pif_brushSpeed->Deselect();
				psf_char->Select();
			}
			else if (sideState == 10)
			{
				pif_brushX->Deselect();
				pif_brushSpeed->Select();
			}
			else if (sideState == 11)
			{
				pif_brushY->Deselect();
				pif_brushX->Select();
			}
			else if (sideState == 12)
			{
				pb_resetBrushPos->Deselect();
				pif_brushY->Select();
			}
			else if (sideState == 13)
			{
				pb_foreTool->Deselect();
				pb_resetBrushPos->Select();
			}
			else if (sideState == 14)
			{
				pb_backTool->Deselect();
				pb_foreTool->Select();
			}
			else if (sideState == 15)
			{
				pb_charTool->Deselect();
				pb_backTool->Select();
			}
			else if (sideState == 16)
			{
				pif_fontSize->Deselect();
				pb_charTool->Select();
			}
			else if (sideState == 17)
			{
				pif_background->Deselect();
				pif_fontSize->Select();
			}
			else
				return;

			sideState--;
		}
		// Move down
		else if (dir == 2)
		{
			if (sideState == 0)
			{
				pif_fr->Deselect();
				pif_fg->Select();
			}
			else if (sideState == 1)
			{
				pif_fg->Deselect();
				pif_fb->Select();
			}
			else if (sideState == 2)
			{
				pif_fb->Deselect();
				pif_fa->Select();
			}
			else if (sideState == 3)
			{
				pif_fa->Deselect();
				pif_br->Select();
			}
			else if (sideState == 4)
			{
				pif_br->Deselect();
				pif_bg->Select();
			}
			else if (sideState == 5)
			{
				pif_bg->Deselect();
				pif_bb->Select();
			}
			else if (sideState == 6)
			{
				pif_bb->Deselect();
				pif_ba->Select();
			}
			else if (sideState == 7)
			{
				pif_ba->Deselect();
				psf_char->Select();
			}
			else if (sideState == 8)
			{
				psf_char->Deselect();
				pif_brushSpeed->Select();
			}
			else if (sideState == 9)
			{
				pif_brushSpeed->Deselect();
				pif_brushX->Select();
			}
			else if (sideState == 10)
			{
				pif_brushX->Deselect();
				pif_brushY->Select();
			}
			else if (sideState == 11)
			{
				pif_brushY->Deselect();
				pb_resetBrushPos->Select();
			}
			else if (sideState == 12)
			{
				pb_resetBrushPos->Deselect();
				pb_foreTool->Select();
			}
			else if (sideState == 13)
			{
				pb_foreTool->Deselect();
				pb_backTool->Select();
			}
			else if (sideState == 14)
			{
				pb_backTool->Deselect();
				pb_charTool->Select();
			}
			else if (sideState == 15)
			{
				pb_charTool->Deselect();
				pif_fontSize->Select();
			}
			else if (sideState == 16)
			{
				pif_fontSize->Deselect();
				pif_background->Select();
			}
			else
				return;

			sideState++;
		}
		// To canvas
		else if (dir == 3)
		{
			if (sideState == 0) // fr
				pif_fr->Deselect();
			else if (sideState == 1) // fg
				pif_fg->Deselect();
			else if (sideState == 2) // fb
				pif_fb->Deselect();
			else if (sideState == 3) // fa
				pif_fa->Deselect();
			else if (sideState == 4) // br
				pif_br->Deselect();
			else if (sideState == 5) // bg
				pif_bg->Deselect();
			else if (sideState == 6) // bb
				pif_bb->Deselect();
			else if (sideState == 7) // ba
				pif_ba->Deselect();
			else if (sideState == 8) // char
				psf_char->Deselect();
			else if (sideState == 9) // brush speed
				pif_brushSpeed->Deselect();
			else if (sideState == 10) // brush speed
				pif_brushX->Deselect();
			else if (sideState == 11) // brush speed
				pif_brushY->Deselect();
			else if (sideState == 12)
				pb_resetBrushPos->Deselect();
			else if (sideState == 13)
				pb_foreTool->Deselect();
			else if (sideState == 14)
				pb_backTool->Deselect();
			else if (sideState == 15)
				pb_charTool->Deselect();
			else if (sideState == 16)
				pif_fontSize->Deselect();
			else if (sideState == 17)
				pif_background->Deselect();
			// SelectCanvas();
			sectionState = 0;
		}
	}
	else
		return;
	
	// If reached this point then did move between widgets.
	// Play sound effect.
	as_widgetsMove.Play(0, 0, 0, 0.15f);
}


int main()
{
	Time::tps = 60;
	
	Log("InitializeAudio");
	InitializeAudio();

	as_widgetsMove.LoadWavFile("assets/widgetsMove.wav");

	Log("Collider types");
	// To prevent the brush from exiting the canvas
	colliderTypes = {
		{2, 2, 0, 0},
		{0, 2, 2, 0},
		{0, 0, 2, 2},
		{2, 0, 0, 2}
	};

	Log("Map, layer and camera");
	// Map, layer and camera
	Map map;
	Layer layer;
	Layer backlayer;
	Camera camera({ 0, 0 }, { 95, 43 });
	map.AddLayer(&backlayer);
	map.AddLayer(&layer);
	map.AddCamera(&camera, true);
	
	Log("Frame");
	// Frame
	Object frame;
	frame.textures.resize(7);
	frame.textures[0].Simple({ 1, 200 }, Engine::CellA('|', { 87, 166, 75 }, { 0, 0, 0 }), {0, 5}); // up frame
	frame.textures[1].Write({ // logo
		"  ||  /   ----  ----  ----  ___ ___ /",
		" ||| /\\   |     |  _  |--    |  |   |",
		"||||/  \\  \\___  \\__/  \\___   |  \\__ |",
		" ___________________________________/",
		"/"
		} , { 87, 166, 75 }, { 0, 0, 0 }, { 0, 1 });
	frame.textures[2].Simple({ 200, 1 }, Engine::CellA('_', { 87, 166, 75 }, { 0, 0, 0 }), { 37, 0 }); //side frame
	frame.textures[3].Write({ // foreground color frame
		"Foreground",
		"#-----#",
		"|     |",
		"|     |",
		"|     |",
		"|     |",
		"#-----#"
		}, { 150 ,150, 150, 255 }, { 0, 0, 0, 0 }, { 2, 6 }); 
	frame.textures[4].Write({ // background color frame
		"Background",
		"#-----#",
		"|     |",
		"|     |",
		"|     |",
		"|     |",

		"#-----#"
		}, { 150 ,150, 150, 255 }, { 0, 0, 0, 0 }, { 2, 14 });
	frame.textures[5].Write({ // brush settings frame
		"Settings",
		"#---------#",
		"|         |",
		"|         |",
		"|         |",
		"|         |",
		"|         |",
		"|         |",
		"|         |",
		"|         |",
		"#---------#"
		}, { 150 ,150, 150, 255 }, { 0, 0, 0, 0 }, { 2, 26 });
	frame.textures[6].File("assets/backgroundSelectorFrame.kcget", { 2, 41 }); // background selector options
	layer.AddObject(&frame);

	Log("Up");
	// Up section
	Log("Up b_exit");
	Button b_exit(&layer, Exit, kReturn, Point( 38, 2 ), "Exit", true); // exit
	Log("Up b_import");
	Button b_import(&layer, nullptr, kReturn, Point( 45, 2 ), "Import", true); // import
	Log("Up b_export");
	Button b_export(&layer, nullptr, kReturn, Point( 54, 2 ), "Export", true); // export
	Log("Up if_canvasX");
	IntInputField if_canvasX(&layer, NULL, 1, 78, "16", Point( 63, 2 ), "CanvasX=", true); // canvas size x
	Log("Up if_canvasY");
	IntInputField if_canvasY(&layer, NULL, 1, 33, "16", Point( 74, 2 ), "CanvasY=", true); // canvas size y
	Log("Up b_confirmSize");
	Button b_confirmSize(&layer, nullptr, kReturn, { 85, 2 }, "Confirm", true); // conifm
	pb_exit = &b_exit;
	pb_import = &b_import;
	pb_export = &b_export;
	pif_canvasX = &if_canvasX;
	pif_canvasY = &if_canvasY;
	pb_confirmSize = &b_confirmSize;

	Log("Side");
	// Side section
	IntInputField if_fr(&layer, nullptr, 0, 255, "255", { 2, 7 }, "R="); // fr
	if_fr.obj.textures[2].active = false;
	IntInputField if_fg(&layer, nullptr, 0, 255, "255", { 2, 8 }, "G="); // fg
	if_fg.obj.textures[2].active = false;
	IntInputField if_fb(&layer, nullptr, 0, 255, "255", { 2, 9 }, "B="); // fb
	if_fb.obj.textures[2].active = false;
	IntInputField if_fa(&layer, nullptr, 0, 255, "255", { 2, 10 }, "A="); // fa
	if_fa.obj.textures[2].active = false;
	IntInputField if_br(&layer, nullptr, 0, 255, "0", { 2, 15 }, "R="); // br
	if_br.obj.textures[2].active = false;
	IntInputField if_bg(&layer, nullptr, 0, 255, "0", { 2, 16 }, "G="); // bg
	if_bg.obj.textures[2].active = false;
	IntInputField if_bb(&layer, nullptr, 0, 255, "0", { 2, 17 }, "B="); // bb
	if_bb.obj.textures[2].active = false;
	IntInputField if_ba(&layer, nullptr, 0, 255, "0", { 2, 18 }, "A="); // ba
	if_ba.obj.textures[2].active = false;
	StringInputField sf_char(&layer, nullptr, { Characters_All }, { 2, 22 }, "Char=", 1, "#", true); // char
	IntInputField if_brushSpeed(&layer, NULL, 0, 9, "2", { 2, 27 }, "Speed="); // brush speed
	if_brushSpeed.obj.textures[2].active = false;
	IntInputField if_brushX(&layer, nullptr, 1, 9, "1", { 2, 28 }, "BrushX="); // brush size x
	if_brushX.obj.textures[2].active = false;
	IntInputField if_brushY(&layer, nullptr, 1, 9, "1", { 2, 29 }, "BrushY="); // brush size y
	if_brushY.obj.textures[2].active = false;
	Button b_resetBrushPos(&layer, nullptr, kReturn, { 2, 30 }, "Reset Pos"); // reset brush pos
	b_resetBrushPos.obj.textures[1].active = false;
	Button b_foreTool(&layer, NULL, kReturn, { 2, 31 }, "Fore Tool");
	b_foreTool.obj.textures[1].active = false;
	Button b_backTool(&layer, NULL, kReturn, { 2, 32 }, "Back Tool");
	b_backTool.obj.textures[1].active = false;
	Button b_charTool(&layer, NULL, kReturn, { 2, 33 }, "Char Tool");
	b_charTool.obj.textures[1].active = false;
	IntInputField if_fontSize(&layer, NULL, 10, 99, "12", { 2, 34 }, "Font="); // font size x and y
	if_fontSize.obj.textures[2].active = false;
	IntInputField if_background(&layer, nullptr, 1, 3, "1", { 2, 38 }, "Back=", true); // canvas background
	pif_fr = &if_fr;
	pif_fg = &if_fg;
	pif_fb = &if_fb;
	pif_fa = &if_fa;
	pif_br = &if_br;
	pif_bg = &if_bg;
	pif_bb = &if_bb;
	pif_ba = &if_ba;
	psf_char = &sf_char;
	pif_brushSpeed = &if_brushSpeed;
	pif_brushX = &if_brushX;
	pif_brushY = &if_brushY;
	pb_resetBrushPos = &b_resetBrushPos;
	pb_foreTool = &b_foreTool;
	b_foreTool.OnPress = std::bind(SwitchButton, pb_foreTool);
	pb_backTool = &b_backTool;
	b_backTool.OnPress = std::bind(SwitchButton, pb_backTool);
	pb_charTool = &b_charTool;
	b_charTool.OnPress = std::bind(SwitchButton, pb_charTool);
	pif_fontSize = &if_fontSize;
	pif_background = &if_background;

	Log("Input handlers");
	// Input handlers
	Input::RegisterHandler(ksUp, std::bind(StateMachine, 0), true);
	Input::RegisterHandler(ksLeft, std::bind(StateMachine, 1), true);
	Input::RegisterHandler(ksDown, std::bind(StateMachine, 2), true);
	Input::RegisterHandler(ksRight, std::bind(StateMachine, 3), true);

	std::thread t_inputLoop(Input::Loop);
	PrepareTerminal({95, 43});
	
	// Loop
	for (; running; Time::thisTickStartTP.SetToNow(), Time::totalTicks++)
	{
		Input::Call();
		map.CallOnTicks();
		Time::CallInvocations();

		map.cameras[map.activeCameraI]->Render(map.layers);
		map.cameras[map.activeCameraI]->Draw({ 0, 0 }, 0, 0, 0, 0);
		
		Print();

		Time::WaitUntilNextTick();
	}

	TerminateAudio();
	ResetTerminal();

	exit(0);
}