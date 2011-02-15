#include "LuaUtilFuncs.h"
#include "lua/lua.h"
#include "vector3.h"
#include "perlin.h"
#include "MyLuaMathTypes.h"
#include "libs.h"
#include <set>
#ifdef _WIN32
#include "win32-dirent.h"
#else
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace LuaUtilFuncs {

int noise(lua_State *L) {
	vector3f v;
	if (lua_isnumber(L, 1)) {
		v.x = lua_tonumber(L, 1);
		v.y = lua_tonumber(L, 2);
		v.z = lua_tonumber(L, 3);
	} else {
		v = *MyLuaVec::checkVec(L, 1);
	}
	lua_pushnumber(L, noise(v));
	return 1;
}

static void lua_traverse(lua_State *L, const char *fn) {
	DIR *dir;
	struct dirent *entry;
	char path[1024];
	struct stat info;
	// putting directory contents into sorted order so order of
	// model definition is consistent
	std::set<std::string> entries;

	lua_getglobal(L, "CurrentDirectory");
	std::string save_dir = luaL_checkstring(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, fn);
	lua_setglobal(L, "CurrentDirectory");

	if ((dir = opendir(fn)) == NULL)
		perror("opendir() error");
	else {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name[0] != '.') {
				entries.insert(entry->d_name);
			}
		}
		closedir(dir);
		for (std::set<std::string>::iterator i = entries.begin(); i!=entries.end(); ++i) {
			const std::string &name = *i;
			strcpy(path, fn);
			strcat(path, "/");
			strcat(path, name.c_str());
			if (stat(path, &info) != 0) {
				fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));
			} else {
				if (S_ISDIR(info.st_mode))
					lua_traverse(L, path);
				else {
					if ( name.size() >= strlen(".lua") && strcasecmp( name.c_str() + name.size()-4, ".lua") == 0) {
						lua_pushcfunction(L, mylua_panic);
						if (luaL_loadfile(L, path)) {
							mylua_panic(L);
						} else {
							lua_pcall(L, 0, LUA_MULTRET, -2);
						}
					}
				}
			}
		}
	}
	lua_pushstring(L, save_dir.c_str());
	lua_setglobal(L, "CurrentDirectory");
}

int load_lua(lua_State *L) {
	const char *fn = luaL_checkstring(L, 1);
	lua_traverse(L, fn);
	return 0;
}

} /* LuaUtilFuncs */
