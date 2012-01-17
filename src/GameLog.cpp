#include "GameLog.h"
#include "Game.h"

GameLog::GameLog(Game *game, unsigned int size) :
	Gui::Fixed(),
	m_game(game),
	m_size(size)
{
	assert(game);
	assert(m_size > 0);

	SetBgColor(0.0f,0.0f,0.0f,0.5f);
	SetTransparency(false);

	Gui::HBox *box = new Gui::HBox();
	box->SetSpacing(2);
	Add(box, 2, 2);

	Gui::LabelButton *buttonLeft = new Gui::LabelButton(new Gui::Label("<"));
	Gui::LabelButton *buttonRight = new Gui::LabelButton(new Gui::Label(">"));
	m_label = new Gui::Label("");

	box->PackEnd(buttonLeft);
	box->PackEnd(buttonRight);
	box->PackEnd(m_label);
}

void GameLog::AddMessage(const std::string &text, Priority priority)
{
	if (m_messages.size() == m_size)
		m_messages.erase(m_messages.begin());
	m_messages.push_back(Message(m_game->GetTime(), text, priority));

	m_label->SetText(text);
	ResizeRequest();
}

void GameLog::GetSizeRequested(float size[2])
{
	size[0] = FLT_MAX;
	size[1] = 24.0f;	// 16px button + 4px labelbutton padding + 4px our padding
}
