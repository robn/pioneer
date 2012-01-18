#include "GameLog.h"
#include "Game.h"

GameLog::GameLog(Game *game, unsigned int size) :
	Gui::Fixed(),
	m_game(game),
	m_size(size),
	m_index(0)
{
	assert(game);
	assert(m_size > 0);

	SetBgColor(0.0f,0.0f,0.0f,0.5f);
	SetTransparency(false);

	Gui::HBox *box = new Gui::HBox();
	box->SetSpacing(2);
	Add(box, 2, 2);

	Gui::LabelButton *buttonPrev = new Gui::LabelButton(new Gui::Label("<"));
	Gui::LabelButton *buttonNext = new Gui::LabelButton(new Gui::Label(">"));
	m_label = new Gui::Label("");

	box->PackEnd(buttonPrev);
	box->PackEnd(buttonNext);
	box->PackEnd(m_label);

	buttonPrev->onClick.connect(sigc::mem_fun(this, &GameLog::MessagePrev));
	buttonNext->onClick.connect(sigc::mem_fun(this, &GameLog::MessageNext));
}

void GameLog::GetSizeRequested(float size[2])
{
	size[0] = FLT_MAX;
	size[1] = 24.0f;	// 16px button + 4px labelbutton padding + 4px our padding
}

void GameLog::AddMessage(const std::string &text, Priority priority)
{
	bool jump = m_index == m_messages.size()-1;

	if (m_messages.size() == m_size)
		m_messages.erase(m_messages.begin());
	m_messages.push_back(Message(m_game->GetTime(), text, priority));

	if (jump) {
		m_index = m_messages.size()-1;
		m_label->SetText(text);
	}
}

void GameLog::MessagePrev()
{
	if (m_index == 0)
		return;

	m_index--;

	m_label->SetText(m_messages[m_index].text);
}

void GameLog::MessageNext()
{
	if (m_index == m_messages.size()-1)
		return;

	m_index++;
	m_label->SetText(m_messages[m_index].text);
}
