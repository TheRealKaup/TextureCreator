#pragma once

#include "../ktech/ktech.hpp"

struct Canvas : public KTech::Widget
{
	enum class Background {
		black,
		red,
		transparent
	};

	KTech::UPoint m_size;

	bool m_foregroundTool, m_backgroundTool, m_characterTool;

	Background m_background;
	
	KTech::RGBA m_selectedRGBA = KTech::RGBA(255, 255, 255, 255);
	KTech::RGBA m_unselectedRGBA = KTech::RGBA(150, 150, 150, 255);

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

	virtual void RenderSelected() override;
	virtual void RenderUnselected() override;

	void SetBackground(Background background);
	void Resize(KTech::UPoint _size);
	void Draw();

	void SetBrushSize(KTech::UPoint size);
	void SetBrushValue(KTech::CellA m_value);
	void MoveBrush();

	void Import(const std::string& fileName);
	void Export(const std::string& fileName);
};