// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/flappy_screen.h>

void flappy::screen::update(float delta, 
	float aspectRatio, 
	std::pair<int, int> windowSize)
{
	if (++m_PrompCounter % BLINK_RATE == 0)
	{
		m_BlinkStatus = !m_BlinkStatus;

		if (m_pCurrentText)
		{
			if (m_BlinkStatus) m_pCurrentText->show();
			else m_pCurrentText->hide();
		}
	}
}

void flappy::screen::set_current_text(std::shared_ptr<gdk::text_renderer> aText)
{
	m_pCurrentText = aText;

	m_PrompCounter = BLINK_RATE / 2;

	if (m_pCurrentText) m_pCurrentText->hide();
}

void flappy::screen::show_current_text()
{
	if (m_pCurrentText) m_pCurrentText->show();
}