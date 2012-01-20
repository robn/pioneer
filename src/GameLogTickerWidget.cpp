#include "GameLogTickerWidget.h"

GameLogTickerWidget::GameLogTickerWidget(GameLog *gameLog) :
	Gui::Fixed(),
	m_gameLog(gameLog),
	m_index(0)
{
	assert(m_gameLog);

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

	buttonPrev->onClick.connect(sigc::mem_fun(this, &GameLogTickerWidget::SwitchMessagePrev));
	buttonNext->onClick.connect(sigc::mem_fun(this, &GameLogTickerWidget::SwitchMessageNext));

	m_onNewMessageConnection = m_gameLog->onNewMessage.connect(sigc::mem_fun(this, &GameLogTickerWidget::OnNewMessage));

	if (m_gameLog->GetMessageCount() > 0) {
		m_index = m_gameLog->GetMessageCount()-1;
		OnNewMessage();
	}
}

GameLogTickerWidget::~GameLogTickerWidget()
{
	m_onNewMessageConnection.disconnect();
}

void GameLogTickerWidget::GetSizeRequested(float size[2])
{
	size[0] = FLT_MAX;
	size[1] = 24.0f;	// 16px button + 4px labelbutton padding + 4px our padding
}

void GameLogTickerWidget::SwitchMessagePrev()
{
	if (m_index == 0)
		return;

	m_index--;
	m_label->SetText(m_gameLog->GetMessage(m_index).text);
}

void GameLogTickerWidget::SwitchMessageNext()
{
	if (m_index == m_gameLog->GetMessageCount()-1)
		return;

	m_index++;
	m_label->SetText(m_gameLog->GetMessage(m_index).text);
}

void GameLogTickerWidget::OnNewMessage()
{
	unsigned int last = m_gameLog->GetMessageCount()-1;

	if (m_index >= last-1)
		m_index = last;

	else if (m_index > 0)
		m_index--;

	m_label->SetText(m_gameLog->GetMessage(m_index).text);
}
