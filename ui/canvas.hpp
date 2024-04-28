#pragma once

#include "../ktech/ktech.hpp"
#include "frame.hpp"

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

	KTech::UPoint m_size;
	Background m_background;
	KTech::RGBA m_selectedRGBA = KTech::RGBA(255, 255, 255, 255);
	KTech::RGBA m_unselectedRGBA = KTech::RGBA(150, 150, 150, 255);

	Frame m_frame;

	bool m_foregroundTool, m_backgroundTool, m_characterTool;

	Canvas(KTech::Engine& engine,
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