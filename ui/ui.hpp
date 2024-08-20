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

#include "../ktech/ktech.hpp"
#include "../ktech/widgets/button.hpp"
#include "../ktech/widgets/intfield.hpp"
#include "../ktech/widgets/stringfield.hpp"
#include "../ktech/widgets/switch.hpp"

#include "../ktech/widgets/frame.hpp"

struct TextureCreatorUI : KTech::UI
{
	// State machine sections
	enum class Section {
		canvas,
		top,
		side
	};

	struct Canvas : public KTech::Widget
	{
		enum class Background {
			black,
			red,
			transparent
		};

		enum TextureIndex {
			ti_background,
			ti_canvas,
			ti_brush,
			TEXTURES_SIZE
		};

		TextureCreatorUI* const textureCreatorUI;

		KTech::UPoint m_size;
		Background m_background;
		KTech::RGBA m_selectedRGBA = KTech::RGBA(255, 255, 255, 255);
		KTech::RGBA m_unselectedRGBA = KTech::RGBA(150, 150, 150, 255);

		Frame m_frame;

		bool m_foregroundTool, m_backgroundTool, m_characterTool;

		Canvas(TextureCreatorUI* const textureCreatorUI,
			KTech::Engine& engine,
			KTech::ID<KTech::UI> parentUI,
			KTech::Point position,
			KTech::UPoint size,
			Background background,
			KTech::UPoint brushSize,
			KTech::CellA brushValue,
			bool foreTool,
			bool backTool,
			bool charTool);

		void SetBackground(Background background);
		void Resize(KTech::UPoint size);
		bool Draw();
		bool EraseFully();
		bool EraseAccordingToToggledTools();

		void SetBrushSize(KTech::UPoint size);
		void SetBrushValue(KTech::CellA value);
		bool MoveBrush();

		void Import(const std::string& fileName);
		void Export(const std::string& fileName);
	};

	struct TopSection : KTech::Widget
	{
		TextureCreatorUI* const textureCreatorUI;

		Button m_exit;
		Button m_import;
		Button m_export;
		IntField m_canvasSizeX;
		IntField m_canvasSizeY;
		Button m_confirmCanvasSize;

		size_t m_curWidget = 0;

		TopSection(TextureCreatorUI* const textureCreatorUI, KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

		bool MoveRight();
		bool MoveLeft();

		// Member widgets' callbacks.
		// Note: these only redirect values to other areas of TextureCreator and don't manipulate the program in any other way, as UI should behave.
		void ResizeCanvas();
	};

	struct SideSection : KTech::Widget
	{		
		TextureCreatorUI* const textureCreatorUI;

		IntField m_foregroundR;
		IntField m_foregroundG;
		IntField m_foregroundB;
		IntField m_foregroundA;
		IntField m_backgroundR;
		IntField m_backgroundG;
		IntField m_backgroundB;
		IntField m_backgroundA;
		StringField m_character;
		IntField m_brushSizeX;
		IntField m_brushSizeY;
		Switch m_toggleForeground;
		Switch m_toggleBackground;
		Switch m_toggleCharacter;
		IntField m_canvasBackground;

		Frame m_frame1, m_frame2, m_frame3;

		size_t m_curWidget = 0;

		SideSection(TextureCreatorUI* const textureCreatorUI, KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

		bool MoveDown();
		bool MoveUp();

		void SetBrushSize();
		void SetBrushValue();
		void SetBackground();
		void ToggleForeground();
		void ToggleBackground();
		void ToggleCharacter();
	};

	struct DefaultValues
	{
		static constexpr KTech::CellA brushValue = KTech::CellA('#', KTech::RGBA(255, 255, 255, 255), KTech::RGBA(255, 0, 0, 100));
		static constexpr KTech::UPoint brushSize = KTech::UPoint(1, 1);
		static constexpr bool foreTool = true;
		static constexpr bool backTool = false;
		static constexpr bool charTool = true;
		static constexpr Canvas::Background canvasBackground = Canvas::Background::black;
		static constexpr KTech::UPoint canvasSize = KTech::UPoint(60, 30);
		static constexpr KTech::UPoint maxCanvasSize = KTech::UPoint(84, 33);
	};

	Canvas m_canvas;
	TopSection m_topSection;
	SideSection m_sideSection;
	KTech::Widget m_frame;

	Section m_curSection = Section::canvas;

	TextureCreatorUI(KTech::Engine& engine);

	bool Up();
	bool Left();
	bool Right();
	bool Down();
};