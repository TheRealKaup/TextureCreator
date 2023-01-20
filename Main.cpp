#include "Engine/Engine.h"
#include "Engine/Widgets.h"
#include "Textures.h"

/*
TODO:

Frame for color fields - generally make the TC prettier
Background options (transparent grid) - to fix invisible brush
The render of brush should be rendered through and not on top of canvas - to also fix invisible brush
*/

using namespace Engine;
using namespace Widgets;

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
IntInputField* pif_background; // 13

// Canvas
Object* pcanvas;
Object* pbrush;


RGBA canvasSelected(255, 255, 255, 1.0f);
RGBA canvasUnselected(150, 150, 150, 1.0f);
Vector2D canvasPos(14, 6);

int brushSpeed = 0;

void SelectCanvas()
{
	for (size_t i = 1; i < 5; i++)
	{
		for (size_t y = 0; y < pcanvas->textures[i].t.size(); y++)
		{
			for (size_t x = 0; x < pcanvas->textures[i].t[y].size(); x++)
				pcanvas->textures[i].t[y][x].frgba = canvasSelected;
		}
	}
}

void DeselectCanvas()
{
	for (size_t i = 1; i < 5; i++)
	{
		for (size_t y = 0; y < pcanvas->textures[i].t.size(); y++)
		{
			for (size_t x = 0; x < pcanvas->textures[i].t[y].size(); x++)
				pcanvas->textures[i].t[y][x].frgba = canvasUnselected;
		}
	}
}

void Exit() { running = false; }

void ChangeCanvasBackground()
{
	if (pif_background->number == 1)
	{
		for (size_t y = 0; y < pcanvas->textures[0].t.size(); y++)
		{
			for (size_t x = 0; x < pcanvas->textures[0].t[y].size(); x++)
				pcanvas->textures[0].t[y][x].brgba = { 0, 0, 0, 0.0f };
		}
	}
	else if (pif_background->number == 2)
	{
		for (size_t y = 0; y < pcanvas->textures[0].t.size(); y++)
		{
			for (size_t x = 0; x < pcanvas->textures[0].t[y].size(); x++)
				pcanvas->textures[0].t[y][x].brgba = { 255, 0, 0, 1.0f };
		}
	}
	else if (pif_background->number == 3)
	{
		bool offone = false;
		for (size_t y = 0; y < pcanvas->textures[0].t.size(); y++, offone = !offone)
		{
			for (size_t x = 0; x < pcanvas->textures[0].t[y].size(); x++)
			{
				if (x % 2 == offone)
					pcanvas->textures[0].t[y][x].brgba = { 205, 205, 205, 1.0f };
				else
					pcanvas->textures[0].t[y][x].brgba = { 255, 255, 255, 1.0f };
			}
		}
	}
}

void ResizeCanvas()
{
	// Textures

	unsigned int sizeX = (unsigned int)pif_canvasX->number;
	unsigned int sizeY = (unsigned int)pif_canvasY->number;

	// Background
	pcanvas->textures[0].pos = { 1, 1 };
	pcanvas->textures[0].t.resize(sizeY);
	if (pif_background->number == 1)
	{
		for (size_t y = 0; y < sizeY; y++)
			pcanvas->textures[0].t[y].resize(sizeX, SuperChar(' ', { 0, 0, 0, 0.0f }, { 0, 0, 0, 0.0f }));
	}
	else if (pif_background->number == 2)
	{
		for (size_t y = 0; y < sizeY; y++)
			pcanvas->textures[0].t[y].resize(sizeX, SuperChar(' ', { 0, 0, 0, 0.0f }, { 255, 0, 0, 1.0f }));
	}
	else if (pif_background->number == 3)
	{
		bool offone = false;
		for (size_t y = 0; y < sizeY; y++, offone = !offone)
		{
			pcanvas->textures[0].t[y].resize(sizeX, SuperChar(' ', { 0, 0, 0, 0.0f }, { 255, 255, 255, 1.0f }));
			for (size_t x = offone; x < sizeX; x+=2)
				pcanvas->textures[0].t[y][x].brgba = { 205, 205, 205, 1.0f };
		}
	}

	// Top frame
	pcanvas->textures[1] = Texture({ sizeX, 1 }, '-', canvasUnselected, { 0, 0, 0, 0.0f }, { 1, 0 });

	// Left frame
	pcanvas->textures[2] = Texture({ 1, sizeY + 2 }, '|', canvasUnselected, { 0, 0, 0, 0.0f }, { 0, 0 });
	pcanvas->textures[2].t[0][0].character = '/';
	pcanvas->textures[2].t[sizeY + 1][0].character = '\\';

	// Bottom frame
	pcanvas->textures[3] = Texture({ sizeX, 1 }, '-', canvasUnselected, { 0, 0, 0, 0.0f }, { 1, (int)(sizeY + 1) });

	// Right frame
	pcanvas->textures[4] = Texture({ 1, sizeY + 2 }, '|', canvasUnselected, { 0, 0, 0, 0.0f }, { (int)(sizeX + 1), 0 });
	pcanvas->textures[4].t[0][0].character = '\\';
	pcanvas->textures[4].t[sizeY + 1][0].character = '/';

	// Canvas itself
	pcanvas->textures[5].pos = { 1, 1 };
	pcanvas->textures[5].t.resize(sizeY);
	for (size_t y = 0; y < sizeY; y++)
		pcanvas->textures[5].t[y].resize(sizeX, SuperChar(' ', { 0, 0, 0, 0.0f }, { 0, 0, 0, 0.0f }));


	// Colliders

	// Top
	pcanvas->colliders[0].pos = { 0, 0 };
	pcanvas->colliders[0].simple = true;
	pcanvas->colliders[0].size = { sizeX + 2, 1 };
	pcanvas->colliders[0].type = 0;

	// Left
	pcanvas->colliders[1].pos = { 0, 0 };
	pcanvas->colliders[1].simple = true;
	pcanvas->colliders[1].size = { 1, sizeY + 2 };
	pcanvas->colliders[1].type = 1;

	// Bottom
	pcanvas->colliders[2].pos = { 0, (int)(sizeY + 1) };
	pcanvas->colliders[2].simple = true;
	pcanvas->colliders[2].size = { sizeX + 2, 1 };
	pcanvas->colliders[2].type = 2;

	// Right
	pcanvas->colliders[3].pos = { (int)(sizeX + 1), 0 };
	pcanvas->colliders[3].simple = true;
	pcanvas->colliders[3].size = { 1, sizeY + 2 };
	pcanvas->colliders[3].type = 3;
}

void MoveBrushSlow(unsigned char dir)
{
	// Only move if canvas is selected
	if (sectionState != 0 || IsKeyDown(VK_LSHIFT))
		return;

	if (dir == 0) // up
		pbrush->Move({ 0, -1 });
	if (dir == 1) // left
		pbrush->Move({ -1, 0 });
	if (dir == 2) // down
		pbrush->Move({ 0, 1 });
	if (dir == 3) // right
		pbrush->Move({ 1, 0 });
}

void MoveBrushFast()
{
	// Only move if canvas is selected
	if (sectionState != 0 || !(IsKeyDown(VK_LSHIFT) && IsKeyDown(VK_LSHIFT)))
		return;

	if (brushSpeed < pif_brushSpeed->number)
	{
		brushSpeed++;
		return;
	}
	else
		brushSpeed = 0;

	if (IsKeyDown('W')) // up
		pbrush->Move({ 0, -1 });
	if (IsKeyDown('A')) // left
		pbrush->Move({ -1, 0 });
	if (IsKeyDown('S')) // down
		pbrush->Move({ 0, 1 });
	if (IsKeyDown('D')) // right
		pbrush->Move({ 1, 0 });
}

void DrawToCanvas()
{
	// Only draw if canvas is selected
	if (sectionState != 0)
		return;

	for (size_t y = 0; y < pbrush->textures[0].t.size(); y++)
	{
		for (size_t x = 0; x < pbrush->textures[0].t[y].size(); x++)
		{
			int fy = y - pcanvas->pos.y - pcanvas->textures[5].pos.y + pbrush->pos.y;
			int fx = x - pcanvas->pos.x - pcanvas->textures[5].pos.x + pbrush->pos.x;

			if (fy < 0 || fy >= pcanvas->textures[5].t.size())
				break;
			if (fx < 0 || fx >= pcanvas->textures[5].t[0].size())
				continue;

			pcanvas->textures[5].t[fy][fx] = pbrush->textures[0].t[y][x];
		}
	}
}

void UpdateBrush()
{
	// Texture
	pbrush->textures[0] = Texture(
		{ (unsigned int)pif_brushX->number, (unsigned int)pif_brushY->number },
		psf_char->string.size() == 0 ? ' ' : psf_char->string[0],
		{ (unsigned char)(pif_fr->number), (unsigned char)(pif_fg->number), (unsigned char)(pif_fb->number), pif_fa->number / 255.0f },
		{ (unsigned char)(pif_br->number), (unsigned char)(pif_bg->number), (unsigned char)(pif_bb->number), pif_ba->number / 255.0f },
		{ 0, 0 }
	);

	// Colliders
	pbrush->colliders[0].pos = { 0, 0 };
	pbrush->colliders[0].simple = true;
	pbrush->colliders[0].size = { 1, 1 };
	pbrush->colliders[0].type = 0;

	pbrush->colliders[1].pos = { 0, pif_brushY->number - 1 };
	pbrush->colliders[1].simple = true;
	pbrush->colliders[1].size = { 1, 1 };
	pbrush->colliders[1].type = 1;

	pbrush->colliders[2].pos = { pif_brushX->number - 1, pif_brushY->number - 1 };
	pbrush->colliders[2].simple = true;
	pbrush->colliders[2].size = { 1, 1 };
	pbrush->colliders[2].type = 2;

	pbrush->colliders[3].pos = { pif_brushX->number - 1, 0 };
	pbrush->colliders[3].simple = true;
	pbrush->colliders[3].size = { 1, 1 };
	pbrush->colliders[3].type = 3;
}

void Import()
{
	std::ifstream file("import.kcget");
	if (!file.is_open())
		return;
	std::string line;

	SuperChar schar;

	int maxX = 0;

	pcanvas->textures[5].t = {};

	size_t y = 0;
	for (; std::getline(file, line); y++)
	{
		pcanvas->textures[5].t.push_back({});

		// potentially broken if one of the values = 10 ('\n')

		for (size_t x = 0, j = 0; x < line.length(); x++, j++)
		{
			if (j == 9)
				j = 0;
			if (j == 0)
				schar.frgba.r = (unsigned char)line[x];
			else if (j == 1)
				schar.frgba.g = (unsigned char)line[x];
			else if (j == 2)
				schar.frgba.b = (unsigned char)line[x];
			else if (j == 3)
				schar.frgba.a = (unsigned char)line[x] / 255.0f;
			else if (j == 4)
				schar.brgba.r = (unsigned char)line[x];
			else if (j == 5)
				schar.brgba.g = (unsigned char)line[x];
			else if (j == 6)
				schar.brgba.b = (unsigned char)line[x];
			else if (j == 7)
				schar.brgba.a = (unsigned char)line[x] / 255.0f;
			else if (j == 8)
			{
				schar.character = line[x];
				pcanvas->textures[5].t[y].push_back(schar);
			}
		}

		if (maxX < pcanvas->textures[5].t[y].size())
			maxX = pcanvas->textures[5].t[y].size();
	}

	for (size_t y = 0; y < pcanvas->textures[5].t.size(); y++)
		pcanvas->textures[5].t[y].resize(maxX, SuperChar(' ', { 0, 0, 0, 0.0f }, { 0, 0, 0, 0.0f }));

	pif_canvasX->ChangeValue(std::to_string(maxX));
	pif_canvasY->ChangeValue(std::to_string(y));

	ResizeCanvas();
}

void Export()
{
	std::ofstream outFile("export.kcget");

	// create (if doesn't exist) file named "textureExport.kcget" and its content is
	// the current texture of canvas.textures[0].t

	std::string out = "";

	for (size_t y = 0; y < pcanvas->textures[5].t.size(); y++)
	{
		for (size_t x = 0; x < pcanvas->textures[5].t[y].size(); x++)
		{
			out += pcanvas->textures[5].t[y][x].frgba.r;
			out += pcanvas->textures[5].t[y][x].frgba.g;
			out += pcanvas->textures[5].t[y][x].frgba.b;
			out += (char)(pcanvas->textures[5].t[y][x].frgba.a * 255);
			out += pcanvas->textures[5].t[y][x].brgba.r;
			out += pcanvas->textures[5].t[y][x].brgba.g;
			out += pcanvas->textures[5].t[y][x].brgba.b;
			out += (char)(pcanvas->textures[5].t[y][x].brgba.a * 255);
			out += pcanvas->textures[5].t[y][x].character;
		}
		if (y < pcanvas->textures[5].t.size() - 1)
			out += '\n';
	}

	outFile.write(out.c_str(), out.length());
}

void ResetBrushPos()
{
	pbrush->pos = { canvasPos.x + 1, canvasPos.y + 1 };
}


void StateMachine(unsigned char dir) // 0 = up, 1 = left, 2 = down, 3 = right
{
	// FIRST TRY BABY IM TIRED AS SHIT YEAHHHH
	// nvm i'm completely changing this

	/* upper:
	pb_exit; // 0
	pb_import; // 1
	pb_export; // 2
	pif_canvasX; // 3
	pif_canvasY; // 4
	pb_confirmSize; // 5
	*/

	/* side:
	pif_fr; // 0
	pif_fg; // 1
	pif_fb; // 2
	pif_fa; // 3
	pif_br; // 4
	pif_bg; // 5
	pif_bb; // 6
	pif_ba; // 7
	psf_char; // 8
	pif_brushSpeed; // 9
	pif_brushX; // 10
	pif_brushY; // 11
	pb_resetBrushPos // 12
	pif_background // 13
	*/

	// Can't use 'else' because visual studio is broken

	// Canvas
	if (sectionState == 0)
	{
		// To upper
		if (dir == 0)
		{
			DeselectCanvas();
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
			DeselectCanvas();
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
				pif_background->Select();
			sectionState = 2;
		}
	}
	// Upper
	if (sectionState == 1)
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
			SelectCanvas();
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
	if (sectionState == 2)
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
				pif_background->Deselect();
				pb_resetBrushPos->Select();
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
				pif_background->Deselect();
			SelectCanvas();
			sectionState = 0;
		}
	}
}


int main()
{
	// To prevent the brush from exiting the canvas
	colliderTypes = {
		{2, 2, 0, 0},
		{0, 2, 2, 0},
		{0, 0, 2, 2},
		{2, 0, 0, 2}
	};


	// Map, layer and camera
	Map map;
	Layer layer;
	Layer backlayer;
	Camera camera({ 0, 0 }, { 95, 39 });
	map.AddLayer(&backlayer);
	map.AddLayer(&layer);
	map.AddCamera(&camera, true);
	

	// Frame
	Object frame;
	frame.textures.resize(7);
	frame.textures[0] = t_frame1;
	frame.textures[1] = t_frame2;
	frame.textures[2] = t_frame3;
	frame.textures[3] = t_foregroundFrame;
	frame.textures[4] = t_backgroundFrame;
	frame.textures[5] = t_brushFrame;
	frame.textures[6] = Texture("backgroundSelectorFrame.kcget", { 2, 37 });
	layer.AddObject(&frame);


	// Up section
	Button b_exit(&layer, Exit, NULL, VK_RETURN, { 38, 2 }, "Exit"); // exit
	Button b_import(&layer, Import, NULL, VK_RETURN, { 45, 2 }, "Import"); // import
	Button b_export(&layer, Export, NULL, VK_RETURN, { 54, 2 }, "Export"); // export
	IntInputField if_canvasX(&layer, NULL, 1, 78, "16", { 63, 2 }, "CanvasX="); // canvas size x
	IntInputField if_canvasY(&layer, NULL, 1, 30, "16", { 74, 2 }, "CanvasY="); // canvas size y
	Button b_confirmSize(&layer, ResizeCanvas, NULL, VK_RETURN, { 85, 2 }, "Confirm"); // conifm
	pb_exit = &b_exit;
	pb_import = &b_import;
	pb_export = &b_export;
	pif_canvasX = &if_canvasX;
	pif_canvasY = &if_canvasY;
	pb_confirmSize = &b_confirmSize;


	// Side section
	IntInputField if_fr(&layer, UpdateBrush, 0, 255, "255", { 2, 7 }, "R="); // fr
	if_fr.obj.textures[2].active = false;
	IntInputField if_fg(&layer, UpdateBrush, 0, 255, "255", { 2, 8 }, "G="); // fg
	if_fg.obj.textures[2].active = false;
	IntInputField if_fb(&layer, UpdateBrush, 0, 255, "255", { 2, 9 }, "B="); // fb
	if_fb.obj.textures[2].active = false;
	IntInputField if_fa(&layer, UpdateBrush, 0, 255, "255", { 2, 10 }, "A="); // fa
	if_fa.obj.textures[2].active = false;
	IntInputField if_br(&layer, UpdateBrush, 0, 255, "0", { 2, 15 }, "R="); // br
	if_br.obj.textures[2].active = false;
	IntInputField if_bg(&layer, UpdateBrush, 0, 255, "0", { 2, 16 }, "G="); // bg
	if_bg.obj.textures[2].active = false;
	IntInputField if_bb(&layer, UpdateBrush, 0, 255, "0", { 2, 17 }, "B="); // bb
	if_bb.obj.textures[2].active = false;
	IntInputField if_ba(&layer, UpdateBrush, 0, 255, "0", { 2, 18 }, "A="); // ba
	if_ba.obj.textures[2].active = false;
	StringInputField sf_char(&layer, UpdateBrush, { Characters_Lower, Characters_Digits, Characters_Upper, Characters_Special }, { 2, 22 }, "Char=", 1, "#"); // char
	// sf_char.obj.textures[2].active = false;
	IntInputField if_brushSpeed(&layer, NULL, 0, 9, "2", { 2, 27 }, "Speed="); // brush speed
	if_brushSpeed.obj.textures[2].active = false;
	IntInputField if_brushX(&layer, UpdateBrush, 1, 9, "1", { 2, 28 }, "BrushX="); // brush size x
	if_brushX.obj.textures[2].active = false;
	IntInputField if_brushY(&layer, UpdateBrush, 1, 9, "1", { 2, 29 }, "BrushY="); // brush size y
	if_brushY.obj.textures[2].active = false;
	Button b_resetBrushPos(&layer, ResetBrushPos, NULL, VK_RETURN, { 2, 30 }, "Reset Pos");
	b_resetBrushPos.obj.textures[1].active = false;
	IntInputField if_background(&layer, ChangeCanvasBackground, 1, 3, "1", { 2, 34 }, "Back=");
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
	pif_background = &if_background;


	// Canvas
	Object canvas(canvasPos);
	canvas.textures.resize(6);
	canvas.colliders.resize(4);
	pcanvas = &canvas;
	ResizeCanvas();
	SelectCanvas();
	backlayer.AddObject(&canvas);

	Object brush({ canvasPos.x + 1, canvasPos.y + 1});
	brush.textures.resize(1);
	brush.colliders.resize(4);
	brush.OnTick = MoveBrushFast;
	pbrush = &brush;
	backlayer.AddObject(&brush);
	UpdateBrush();


	// Input handlers
	RegisterInputHandler(VK_UP, true, std::bind(StateMachine, 0), true);
	RegisterInputHandler(VK_LEFT, true, std::bind(StateMachine, 1), true);
	RegisterInputHandler(VK_DOWN, true, std::bind(StateMachine, 2), true);
	RegisterInputHandler(VK_RIGHT, true, std::bind(StateMachine, 3), true);

	RegisterInputHandler('W', true, std::bind(MoveBrushSlow, 0), false);
	RegisterInputHandler('A', true, std::bind(MoveBrushSlow, 1), false);
	RegisterInputHandler('S', true, std::bind(MoveBrushSlow, 2), false);
	RegisterInputHandler('D', true, std::bind(MoveBrushSlow, 3), false);

	RegisterInputHandler(VK_RETURN, true, DrawToCanvas, true);


	// Engine
	InitializeConsole(16, 16, 95, 39, L"KCGETC");
	for (tps = 60; running; thisTickStartTP.SetToNow(), totalTicks++)
	{
		ManageInputs();
		CallOnTicks(&map);

		map.cameras[map.activeCameraI]->Render(map.layers);
		map.cameras[map.activeCameraI]->Draw({ 0, 0 }, 0, 0, 0, 0);
		
		Print();

		WaitUntilNextTick();
	}
}