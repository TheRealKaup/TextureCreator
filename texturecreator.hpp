#pragma once

#include "ktech/ktech.hpp"

#include "ui/ui.hpp"

namespace TextureCreator
{
	inline constexpr KTech::UPoint viewportSize = KTech::UPoint(101, 39);

	inline KTech::Engine engine(viewportSize, 24);

	inline TextureCreatorUI ui(engine);

	inline static void ExitGame()
	{
		engine.running = false;
	}
}