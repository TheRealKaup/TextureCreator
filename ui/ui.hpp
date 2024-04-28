#pragma once

#include "../ktech/ktech.hpp"
#include "../ktech/widgets/button.hpp"
#include "../ktech/widgets/intfield.hpp"
#include "../ktech/widgets/stringfield.hpp"
#include "../ktech/widgets/switch.hpp"

#include "frame.hpp"

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
		void Draw();
		void EraseFully();
		void EraseAccordingToToggledTools();

		void SetBrushSize(KTech::UPoint size);
		void SetBrushValue(KTech::CellA value);
		void MoveBrush();

		void Import(const std::string& fileName);
		void Export(const std::string& fileName);
	};

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

		TextureCreatorUI* const textureCreatorUI;

		Button m_exit;
		Button m_import;
		Button m_export;
		IntField m_canvasSizeX;
		IntField m_canvasSizeY;
		Button m_confirmCanvasSize;

		WidgetIndex m_curWidget = wi_exit;

		TopSection(TextureCreatorUI* const textureCreatorUI, KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

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

		WidgetIndex m_curWidget;

		SideSection(TextureCreatorUI* const textureCreatorUI, KTech::Engine& engine, KTech::ID<KTech::UI> parentUI, KTech::Point pos);

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
		static constexpr KTech::UPoint maxCanvasSize = KTech::UPoint(84, 33);
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