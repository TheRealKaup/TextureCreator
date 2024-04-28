#pragma once

#include "../ktech/ktech.hpp"
#include "../ktech/widgets/button.hpp"
#include "../ktech/widgets/intfield.hpp"
#include "../ktech/widgets/stringfield.hpp"
#include "../ktech/widgets/switch.hpp"

#include "canvas.hpp"
#include "frame.hpp"

struct TextureCreatorUI : KTech::UI
{
	// State machine sections
	enum class Section {
		canvas,
		top,
		side
	};

	// Widgets
	struct TopSection : KTech::Widget
	{
		enum WidgetIndex : uint8_t {
			wi_exit,
			wi_import,
			wi_export,
			wi_canvasSizeX,
			wi_canvasSizeY,
			wi_confirm,
			WIDGETS_SIZE
		};

		Button m_exit;
		Button m_import;
		Button m_export;
		IntField m_canvasSizeX;
		IntField m_canvasSizeY;
		Button m_confirmCanvasSize;

		WidgetIndex m_curWidget = wi_exit;

		TopSection(KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

		void MoveRight();
		void MoveLeft();

		// Member widgets' callbacks.
		// Note: these only redirect values to other areas of TextureCreator and don't manipulate the program in any other way, as UI should behave.
		void ResizeCanvas();
	};

	struct SideSection : KTech::Widget
	{
		enum WidgetIndex : uint8_t {
			wi_foregroundR,
			wi_foregroundG,
			wi_foregroundB,
			wi_foregroundA,
			wi_backgroundR,
			wi_backgroundG,
			wi_backgroundB,
			wi_backgroundA,
			wi_character,
			wi_brushSizeX,
			wi_brushSizeY,
			wi_toggleForeground,
			wi_toggleBackground,
			wi_toggleCharacter,
			wi_canvasBackground,
			WIDGETS_SIZE
		};

		enum TextureIndex : uint8_t {
			ti_titleForeground,
			ti_titleBackground,
			TEXTURES_SIZE
		};

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

		WidgetIndex m_curWidget;

		SideSection(KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

		void MoveDown();
		void MoveUp();

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
	};

	Canvas m_canvas;
	TopSection m_topSection;
	SideSection m_sideSection;
	KTech::Widget m_frame;

	Section m_curSection = Section::canvas;

	TextureCreatorUI(KTech::Engine& engine);

	void Up();
	void Left();
	void Right();
	void Down();
};