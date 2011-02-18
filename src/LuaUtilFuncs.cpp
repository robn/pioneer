#include "LuaUtilFuncs.h"
#include "PiLuaClasses.h"
#include "OOLuaStatic.h"
#include "vector3.h"
#include "perlin.h"
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

	static float noise(const pi_vector& v)
	{
		return ::noise(vector3d(v));
	}
	static float noise(float x, float y, float z)
	{
		return ::noise(vector3d(x,y,z));
	}

#if 0
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
#endif

	static void load_lua(const std::string& filename)
	{
		assert(0);
	}
}

EXPORT_OOLUA_NO_FUNCTIONS(pi_utils)

namespace static_utils {
	STATIC_DISPATCH_START(noise)
		STATIC_FUNC_1(float, LuaUtilFuncs::noise, const pi_vector&)
		STATIC_FUNC_3(float, LuaUtilFuncs::noise, float, float, float)
	STATIC_DISPATCH_END

	STATIC_DISPATCH_START(load_lua)
		STATIC_FUNC_1(void, LuaUtilFuncs::load_lua, const std::string&)
	STATIC_DISPATCH_END
}

void LuaUtilFuncs::RegisterClasses(lua_State *l)
{
	OOLUA::register_class<pi_utils>(l);

	OOLUA::register_class_static<pi_utils>(l, "noise",    &static_utils::noise);
	OOLUA::register_class_static<pi_utils>(l, "load_lua", &static_utils::load_lua);
}
