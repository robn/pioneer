#ifndef _GAMELOGTICKERWIDGET_H
#define _GAMELOGTICKERWIDGET_H

#include "gui/Gui.h"

#include "GameLog.h"

class GameLogTickerWidget : public Gui::Fixed {
public:
	GameLogTickerWidget(GameLog *gameLog);
	virtual ~GameLogTickerWidget();

	virtual void GetSizeRequested(float size[2]);

private:
	void SwitchMessagePrev();
	void SwitchMessageNext();

	void OnNewMessage();

	GameLog *m_gameLog;

	sigc::connection m_onNewMessageConnection;

	unsigned int m_index;

	Gui::Label *m_label;
};

#endif
