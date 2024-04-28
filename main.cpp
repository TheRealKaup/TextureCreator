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

int main()
{
	using namespace TextureCreator;
	
	while (engine.running)
	{
		engine.input.CallHandlers();
		engine.time.CallInvocations();
		engine.memory.CallOnTicks();

		ui.Render();
		engine.output.Draw(ui.m_image);
		engine.output.Print();

		engine.time.WaitUntilNextTick();
	}
}