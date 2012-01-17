#include "GameLog.h"
#include "Game.h"

GameLog::GameLog(Game *game, unsigned int size) :
	Gui::HBox(),
	m_game(game),
	m_size(size)
{
	assert(game);
	assert(m_size > 0);
}

void GameLog::AddMessage(const std::string &text, Priority priority)
{
	if (m_messages.size() > m_size-1)
		m_messages.resize(m_size-1);

	m_messages.push_back(Message(m_game->GetTime(), text, priority));
}
