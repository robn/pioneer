#include "LuaManager.h"
#include "LuaBody.h"
#include "LuaShip.h"
#include "LuaSpaceStation.h"
#include "LuaCargoBody.h"
#include <stdlib.h> // for abort

bool instantiated = false;

LuaManager::LuaManager() : m_lua(NULL) {
	if (instantiated) {
		fprintf(stderr, "Can't instantiate more than one LuaManager");
		abort();
	}

	m_lua = lua_open();

	luaL_openlibs(m_lua);

	lua_atpanic(m_lua, pi_lua_panic);

	int ret = luaL_loadfile(m_lua, PIONEER_DATA_DIR "/pidebug.lua");
	if (ret) {
		if (ret == LUA_ERRFILE)
			fprintf(stderr, "Can't load '" PIONEER_DATA_DIR "/pidebug.lua'");
		else if (ret == LUA_ERRSYNTAX) {
			const char* message = lua_tostring(m_lua, -1);
			fprintf(stderr, "Syntax error in '" PIONEER_DATA_DIR "/pidebug.lua':\n%s\n", message);
		}
		else
			fprintf(stderr, "Error while loading '" PIONEER_DATA_DIR "/pidebug.lua'");
		abort();
	}
	if (lua_pcall(m_lua, 0, 1, 0)) {
		fprintf(stderr, "Fatal Lua error: pidebug.lua failed to initialise.");
		abort();
	}
	if (lua_type(m_lua, -1) != LUA_TTABLE) {
		fprintf(stderr, "Fatal Lua error: pidebug.lua did not return module table.");
		abort();
	}
	lua_getfield(m_lua, -1, "error_handler");
	if (lua_type(m_lua, -1) != LUA_TFUNCTION) {
		fprintf(stderr, "Fatal Lua error: pidebug.lua did not define error_handler function.");
		abort();
	}
	lua_pop(m_lua, 1);
	lua_setfield(m_lua, LUA_REGISTRYINDEX, "PiDebug");

	m_serializer = new LuaSerializer();
	m_timer = new LuaTimer();

	m_onGameStart = new LuaEventQueue<>(this, "onGameStart");
	m_onGameEnd = new LuaEventQueue<>(this, "onGameEnd");
	m_onEnterSystem = new LuaEventQueue<Ship>(this, "onEnterSystem");
	m_onLeaveSystem = new LuaEventQueue<Ship>(this, "onLeaveSystem");
	m_onFrameChanged = new LuaEventQueue<Body>(this, "onFrameChanged");
	m_onShipDestroyed = new LuaEventQueue<Ship,Body>(this, "onShipDestroyed");
	m_onShipHit = new LuaEventQueue<Ship,Body>(this, "onShipHit");
	m_onShipCollided = new LuaEventQueue<Ship,Body>(this, "onShipCollided");
	m_onShipDocked = new LuaEventQueue<Ship,SpaceStation>(this, "onShipDocked");
	m_onShipUndocked = new LuaEventQueue<Ship,SpaceStation>(this, "onShipUndocked");
	m_onShipLanded = new LuaEventQueue<Ship,Body>(this, "onShipLanded");
	m_onShipTakeOff = new LuaEventQueue<Ship,Body>(this, "onShipTakeOff");
	m_onShipAlertChanged = new LuaEventQueue<Ship,const char *>(this, "onShipAlertChanged");
	m_onJettison = new LuaEventQueue<Ship,CargoBody>(this, "onJettison");
	m_onAICompleted = new LuaEventQueue<Ship,const char *>(this, "onAICompleted");
	m_onCreateBB = new LuaEventQueue<SpaceStation>(this, "onCreateBB");
	m_onUpdateBB = new LuaEventQueue<SpaceStation>(this, "onUpdateBB");
	m_onSongFinished = new LuaEventQueue<>(this, "onSongFinished");
	m_onShipFlavourChanged = new LuaEventQueue<Ship>(this, "onShipFlavourChanged");
	m_onShipEquipmentChanged = new LuaEventQueue<Ship,const char *>(this, "onShipEquipmentChanged");

	m_onGameStart->RegisterEventQueue();
	m_onGameEnd->RegisterEventQueue();
	m_onEnterSystem->RegisterEventQueue();
	m_onLeaveSystem->RegisterEventQueue();
	m_onFrameChanged->RegisterEventQueue();
	m_onShipDestroyed->RegisterEventQueue();
	m_onShipHit->RegisterEventQueue();
	m_onShipCollided->RegisterEventQueue();
	m_onShipDocked->RegisterEventQueue();
	m_onShipLanded->RegisterEventQueue();
	m_onShipTakeOff->RegisterEventQueue();
	m_onShipUndocked->RegisterEventQueue();
	m_onShipAlertChanged->RegisterEventQueue();
	m_onJettison->RegisterEventQueue();
	m_onAICompleted->RegisterEventQueue();
	m_onCreateBB->RegisterEventQueue();
	m_onUpdateBB->RegisterEventQueue();
	m_onSongFinished->RegisterEventQueue();
	m_onShipFlavourChanged->RegisterEventQueue();
	m_onShipEquipmentChanged->RegisterEventQueue();

	instantiated = true;
}

LuaManager::~LuaManager() {
	delete m_onGameStart;
	delete m_onGameEnd;
	delete m_onEnterSystem;
	delete m_onLeaveSystem;
	delete m_onFrameChanged;
	delete m_onShipDestroyed;
	delete m_onShipHit;
	delete m_onShipCollided;
	delete m_onShipDocked;
	delete m_onShipUndocked;
	delete m_onShipLanded;
	delete m_onShipTakeOff;
	delete m_onShipAlertChanged;
	delete m_onJettison;
	delete m_onAICompleted;
	delete m_onCreateBB;
	delete m_onUpdateBB;
	delete m_onSongFinished;
	delete m_onShipFlavourChanged;
	delete m_onShipEquipmentChanged;

	delete m_serializer;
	delete m_timer;

	lua_close(m_lua);

	instantiated = false;
}

void LuaManager::Tick() {
	m_onEnterSystem->Emit();
	m_onLeaveSystem->Emit();
	m_onFrameChanged->Emit();
	m_onShipHit->Emit();
	m_onShipCollided->Emit();
	m_onShipDestroyed->Emit();
	m_onShipDocked->Emit();
	m_onShipAlertChanged->Emit();
	m_onShipUndocked->Emit();
	m_onShipLanded->Emit();
	m_onShipTakeOff->Emit();
	m_onJettison->Emit();
	m_onAICompleted->Emit();
	m_onCreateBB->Emit();
	m_onUpdateBB->Emit();
	m_onShipFlavourChanged->Emit();
	m_onShipEquipmentChanged->Emit();

	m_timer->Tick();
}

size_t LuaManager::GetMemoryUsage() const {
	int kb = lua_gc(m_lua, LUA_GCCOUNT, 0);
	int b = lua_gc(m_lua, LUA_GCCOUNTB, 0);
	return (size_t(kb) * 1024) + b;
}

void LuaManager::CollectGarbage() {
	lua_gc(m_lua, LUA_GCCOLLECT, 0);
}
