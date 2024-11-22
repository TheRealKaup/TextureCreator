/*
	TextureCreator, a tool for creating KTech textures.
	Copyright (C) 2023-2024 Ethan Kaufman (AKA Kaup)

	This file is part of TextureCreator.

	TextureCreator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.

	TextureCreator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TextureCreator. If not, see <https://www.gnu.org/licenses/>.
*/

#include "texturecreator.hpp"

#include <cstring>

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		for (size_t i = 1; i < argc; i++)
		{
			if (std::strcmp("--help", argv[i]) == 0)
			{
				TextureCreator::PrintHelp();
				exit(0);
			}
		}
	}

	TextureCreator textureCreator;

	if (argc == 2) // Import file
		textureCreator.ui.m_canvas.Import(argv[1]);
	else if (argc == 3) // Set canvas size
	{
		KTech::UPoint size;
		for (size_t i = 0; argv[1][i] != 0; i++)
		{
			size.x *= 10;
			size.x += argv[1][i] - '0';
		}
		if (size.x > TextureCreatorUI::DefaultValues::maxCanvasSize.x)
			size.x = TextureCreatorUI::DefaultValues::maxCanvasSize.x;
		else if (size.x == 0)
			size.x = 1;
		for (size_t i = 0; argv[2][i] != 0; i++)
		{
			size.y *= 10;
			size.y += argv[2][i] - '0';
		}
		if (size.y > TextureCreatorUI::DefaultValues::maxCanvasSize.y)
			size.y = TextureCreatorUI::DefaultValues::maxCanvasSize.y;
		else if (size.y == 0)
			size.y = 1;
		textureCreator.ui.m_canvas.Resize(size);
	}

	while (textureCreator.engine.running)
	{
		textureCreator.engine.input.CallCallbacks();
		textureCreator.engine.time.CallInvocations();
		textureCreator.engine.memory.CallOnTicks();

		textureCreator.ui.Render();
		textureCreator.engine.output.Draw(textureCreator.ui.m_image, textureCreator.ui.m_res);
		textureCreator.engine.output.Print();

		textureCreator.engine.time.WaitUntilNextTick();
	}
}