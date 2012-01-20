#include "GameLog.h"
#include "Game.h"

GameLog::GameLog(Game *game, unsigned int size) :
	m_game(game),
	m_size(size),
	m_index(0)
{
	assert(game);
	assert(m_size > 0);

}
void GameLog::AddMessage(const std::string &text, Priority priority)
{
	if (m_messages.size() == m_size)
		m_messages.erase(m_messages.begin());

	m_messages.push_back(Message(m_game->GetTime(), text, priority));

	onNewMessage.emit();
}
