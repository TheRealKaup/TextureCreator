#pragma once

#include "Engine/Engine.h"
using namespace Engine;

// This is the collection of all the textures used. It is split to here because textures take a lot of space in the code
// and looks ugly et cetera et cetera help

// The length of the frame (how far it extends to) is irrelevant

/*
	 _________
	/    3
  2 |
/---/
|
| 1
|

   ||  /   ----  ----  ----  ___ ___  /
  ||| /\   |     |  _  |--    |  |    |
 ||||/  \  \___  \__/  \___   |  \__  |
 ____________________________________/
/
*/

// -= FRAME =-

Texture t_frame1({1, 500}, '|', { 87, 166, 75 }, { 0, 0, 0 }, {0, 5});

Texture t_frame2({
	"  ||  /   ----  ----  ----  ___ ___ /",
	" ||| /\\   |     |  _  |--    |  |   |",
	"||||/  \\  \\___  \\__/  \\___   |  \\__ |",
	" ___________________________________/",
	"/"
}, {87, 166, 75}, {0, 0, 0}, {0, 1});

Texture t_frame3({ 500, 1 }, '_', { 87, 166, 75 }, { 0, 0, 0 }, { 37, 0 });

Texture t_foregroundFrame({
	"Foreground",
	"#-----#",
	"|     |",
	"|     |",
	"|     |",
	"|     |",
	"#-----#"
	}, { 150 ,150, 150, 1.0f }, { 0, 0, 0, 0.0f }, {2, 6});

Texture t_backgroundFrame({
	"Background",
	"#-----#",
	"|     |",
	"|     |",
	"|     |",
	"|     |",
	"#-----#"
	}, { 150 ,150, 150, 1.0f }, { 0, 0, 0, 0.0f }, {2, 14});

Texture t_brushFrame({
	"Settings",
	"#---------#",
	"|         |",
	"|         |",
	"|         |",
	"|         |",
	"#---------#"
	}, { 150 ,150, 150, 1.0f }, { 0, 0, 0, 0.0f }, {2, 26});