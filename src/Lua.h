#ifndef _LUA_H
#define _LUA_H

#include "LuaObject.h"
#include "LuaEventQueue.h"
#include "LuaSerializer.h"
#include "LuaTimer.h"
#include "LuaUtils.h"

class Body;
class Ship;
class SpaceStation;
class CargoBody;

class Lua {
public:
	Lua();
	~Lua();

	lua_State *GetLuaState() { return m_lua; }

	void Tick();

	size_t GetMemoryUsage() const;
	void CollectGarbage();

	LuaSerializer *Serializer() const { return m_serializer; }
	LuaTimer      *Timer()      const { return m_timer; }

	LuaEventQueue<>                  *OnGameStart()            const { return m_onGameStart; }
	LuaEventQueue<>                  *OnGameEnd()              const { return m_onGameEnd; }
	LuaEventQueue<Ship>              *OnEnterSystem()          const { return m_onEnterSystem; }
	LuaEventQueue<Ship>              *OnLeaveSystem()          const { return m_onLeaveSystem; }
	LuaEventQueue<Body>              *OnFrameChanged()         const { return m_onFrameChanged; }
	LuaEventQueue<Ship,Body>         *OnShipDestroyed()        const { return m_onShipDestroyed; }
	LuaEventQueue<Ship,Body>         *OnShipHit()              const { return m_onShipHit; }
	LuaEventQueue<Ship,Body>         *OnShipCollided()         const { return m_onShipCollided; }
	LuaEventQueue<Ship,SpaceStation> *OnShipDocked()           const { return m_onShipDocked; }
	LuaEventQueue<Ship,SpaceStation> *OnShipUndocked()         const { return m_onShipUndocked; }
	LuaEventQueue<Ship,Body>         *OnShipLanded()           const { return m_onShipLanded; }
	LuaEventQueue<Ship,Body>         *OnShipTakeOff()          const { return m_onShipTakeOff; }
	LuaEventQueue<Ship,const char *> *OnShipAlertChanged()     const { return m_onShipAlertChanged; }
	LuaEventQueue<Ship,CargoBody>    *OnJettison()             const { return m_onJettison; }
	LuaEventQueue<Ship,const char *> *OnAICompleted()          const { return m_onAICompleted; }
	LuaEventQueue<SpaceStation>      *OnCreateBB()             const { return m_onCreateBB; }
	LuaEventQueue<SpaceStation>      *OnUpdateBB()             const { return m_onUpdateBB; }
	LuaEventQueue<>                  *OnSongFinished()         const { return m_onSongFinished; }
	LuaEventQueue<Ship>              *OnShipFlavourChanged()   const { return m_onShipFlavourChanged; }
	LuaEventQueue<Ship,const char *> *OnShipEquipmentChanged() const { return m_onShipEquipmentChanged; }

private:
	Lua(const Lua &);
	Lua &operator=(const Lua &);

	lua_State *m_lua;

	LuaSerializer *m_serializer;
	LuaTimer      *m_timer;

	LuaEventQueue<>                  *m_onGameStart;
	LuaEventQueue<>                  *m_onGameEnd;
	LuaEventQueue<Ship>              *m_onEnterSystem;
	LuaEventQueue<Ship>              *m_onLeaveSystem;
	LuaEventQueue<Body>              *m_onFrameChanged;
	LuaEventQueue<Ship,Body>         *m_onShipDestroyed;
	LuaEventQueue<Ship,Body>         *m_onShipHit;
	LuaEventQueue<Ship,Body>         *m_onShipCollided;
	LuaEventQueue<Ship,SpaceStation> *m_onShipDocked;
	LuaEventQueue<Ship,SpaceStation> *m_onShipUndocked;
	LuaEventQueue<Ship,Body>         *m_onShipLanded;
	LuaEventQueue<Ship,Body>         *m_onShipTakeOff;
	LuaEventQueue<Ship,const char *> *m_onShipAlertChanged;
	LuaEventQueue<Ship,CargoBody>    *m_onJettison;
	LuaEventQueue<Ship,const char *> *m_onAICompleted;
	LuaEventQueue<SpaceStation>      *m_onCreateBB;
	LuaEventQueue<SpaceStation>      *m_onUpdateBB;
	LuaEventQueue<>                  *m_onSongFinished;
	LuaEventQueue<Ship>              *m_onShipFlavourChanged;
	LuaEventQueue<Ship,const char *> *m_onShipEquipmentChanged;
};

#endif
