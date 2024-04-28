#pragma once

#include "ktech/ktech.hpp"

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