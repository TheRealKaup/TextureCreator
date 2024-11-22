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

#pragma once

#include "ktech/ktech/ktech.hpp"

#include "ui/ui.hpp"

struct TextureCreator
{
	static constexpr KTech::UPoint viewportSize = KTech::UPoint(101, 39);

	KTech::Engine engine = KTech::Engine(viewportSize, 24);

	TextureCreatorUI ui = TextureCreatorUI(engine);

	inline void ExitGame()
	{
		engine.running = false;
	}

	void static PrintHelp();
};