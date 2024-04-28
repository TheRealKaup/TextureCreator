#pragma once

#include "../ktech/ktech.hpp"

class Frame : public KTech::Widget
{
public:
	KTech::RGBA m_unselectedRGBA, m_selectedRGBA, m_downRGBA;

public:
	virtual void RenderSelected() override
	{
		for (size_t i = 0; i < m_textures.size(); i++)
			m_textures[i].SetForeground(m_selectedRGBA);
	}  

	virtual void RenderUnselected() override
	{
		for (size_t i = 0; i < m_textures.size(); i++)
			m_textures[i].SetForeground(m_unselectedRGBA);
	}

	void Resize(KTech::UPoint size)
	{
		KTech::RGBA tempRGBA = m_selected ? m_selectedRGBA : m_unselectedRGBA;
		// up-left corner
		m_textures[0].Simple(KTech::UPoint(1, 1), KTech::CellA('#', tempRGBA), KTech::Point(0, 0));
		// up-right corner
		m_textures[1].Simple(KTech::UPoint(1, 1), KTech::CellA('#', tempRGBA), KTech::Point(size.x - 1, 0));
		// bottom-left corner
		m_textures[2].Simple(KTech::UPoint(1, 1), KTech::CellA('#', tempRGBA), KTech::Point(0, size.y - 1));
		// bottom-right corner
		m_textures[3].Simple(KTech::UPoint(1, 1), KTech::CellA('#', tempRGBA), KTech::Point(size.x - 1, size.y - 1));
		// up frame
		m_textures[4].Simple(KTech::UPoint(size.x - 2, 1), KTech::CellA('-', tempRGBA), KTech::Point(1, 0));
		// left frame
		m_textures[5].Simple(KTech::UPoint(1, size.y - 2), KTech::CellA('|', tempRGBA), KTech::Point(0, 1));
		// bottom frame
		m_textures[6].Simple(KTech::UPoint(size.x - 2, 1), KTech::CellA('-', tempRGBA), KTech::Point(1, size.y - 1));
		// right frame
		m_textures[7].Simple(KTech::UPoint(1, size.y - 2), KTech::CellA('|', tempRGBA), KTech::Point(size.x - 1, 1));
	}

	Frame(KTech::Engine& engine,
		KTech::ID<KTech::UI> ui,
		KTech::Point pos,
		KTech::UPoint size,
		KTech::RGBA unselectedRGBA = KTech::RGBA( 150, 150, 150, 255 ),
		KTech::RGBA selectedRGBA = KTech::RGBA( 255, 255, 255, 255 ))
		: Widget(engine, ui, pos), m_unselectedRGBA(unselectedRGBA), m_selectedRGBA(selectedRGBA)
	{
		// Texture
		m_textures.resize(8);
		// up-left corner
		m_textures[0].Simple(KTech::UPoint(1, 1), KTech::CellA('#', unselectedRGBA), KTech::Point(0, 0));
		// up-right corner
		m_textures[1].Simple(KTech::UPoint(1, 1), KTech::CellA('#', unselectedRGBA), KTech::Point(size.x - 1, 0));
		// bottom-left corner
		m_textures[2].Simple(KTech::UPoint(1, 1), KTech::CellA('#', unselectedRGBA), KTech::Point(0, size.y - 1));
		// bottom-right corner
		m_textures[3].Simple(KTech::UPoint(1, 1), KTech::CellA('#', unselectedRGBA), KTech::Point(size.x - 1, size.y - 1));
		// up frame
		m_textures[4].Simple(KTech::UPoint(size.x - 2, 1), KTech::CellA('-', unselectedRGBA), KTech::Point(1, 0));
		// left frame
		m_textures[5].Simple(KTech::UPoint(1, size.y - 2), KTech::CellA('|', unselectedRGBA), KTech::Point(0, 1));
		// bottom frame
		m_textures[6].Simple(KTech::UPoint(size.x - 2, 1), KTech::CellA('-', unselectedRGBA), KTech::Point(1, size.y - 1));
		// right frame
		m_textures[7].Simple(KTech::UPoint(1, size.y - 2), KTech::CellA('|', unselectedRGBA), KTech::Point(size.x - 1, 1));
	}
};
