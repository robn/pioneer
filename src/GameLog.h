#ifndef _GAMELOG_H
#define _GAMELOG_H

#include "libs.h"

class Game;

class GameLog {
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

	typedef std::vector<Message>::iterator MessageIterator;
	MessageIterator MessagesBegin() { return m_messages.begin(); }
	MessageIterator MessagesEnd() { return m_messages.end(); }

	const Message &GetMessage(unsigned int n) const { assert(n >= 0 && n < m_messages.size()); return m_messages[n]; }
	unsigned int GetMessageCount() const { return m_messages.size(); }

	sigc::signal<void> onNewMessage;

private:

	Game *m_game;
	unsigned int m_size;

	std::vector<Message> m_messages;

	unsigned int m_index;
};

#endif
