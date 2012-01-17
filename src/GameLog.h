#ifndef _GAMELOG_H
#define _GAMELOG_H

#include "gui/Gui.h"
#include <list>
#include <string>

class Game;

class GameLog : public Gui::Fixed {
public:

	enum Priority {
		PRIORITY_NORMAL,
		PRIORITY_IMPORTANT,
	};

	class Message {
	public:
		Message() {}
		Message(double _time, const std::string &_text, Priority _priority) : time(_time), text(_text), priority(_priority) {}

		double      time;
		std::string text;
		Priority    priority;
	};

	GameLog(Game *game, unsigned int size);

	void AddMessage(const std::string &text, Priority priority = PRIORITY_NORMAL);

	virtual void GetSizeRequested(float size[2]);

private:

	Game *m_game;
	unsigned int m_size;

	Gui::Label *m_label;

	std::list<Message> m_messages;
};

#endif
