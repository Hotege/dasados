#include "luaplugins.h"
#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <map>
#include "../../resolve/strings.h"
#include "../db/database.h"

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
};

namespace luaplugins
{
lua_State* L;
std::string directory;

LUALIB_API int sendBuffer(lua_State* l)
{
    auto conn = luaL_checkinteger(l, -3);
    auto data = luaL_checkstring(l, -2);
    auto len = luaL_checkinteger(l, -1);
    send(conn, data, len, 0);
    return 0;
}

LUALIB_API int getSelectAll(lua_State* l)
{
    auto table = luaL_checkstring(l, -1);
    lua_newtable(l);
    auto r = db::selectAll(table);
    for (auto v : r)
    {
        lua_pushstring(l, v["id"].c_str());
        lua_newtable(l);

//        lua_pushstring(l, "yummyK");
//        lua_pushstring(l, "yummyV");
//        lua_settable(l, -3);
        for (auto p : v)
        {
            lua_pushstring(l, p.first.c_str());
            lua_pushstring(l, p.second.c_str());
            lua_settable(l, -3);
        }

        lua_settable(l, -3);
    }
    return 1;
}

void initialize(const char* path, const char* dir)
{
    L = luaL_newstate();
    luaopen_base(L);
    luaopen_table(L);
    luaopen_package(L);
    luaopen_io(L);
    luaopen_string(L);
    luaL_openlibs(L);

    lua_pushcfunction(L, sendBuffer);
    lua_setglobal(L, "sendBuffer");

    lua_pushcfunction(L, getSelectAll);
    lua_setglobal(L, "getSelectAll");

    luaL_dofile(L, path);

    directory = dir;
}

void terminate()
{
    lua_close(L);
}

void resolveDirectory(const int& conn, const std::string& type, const std::string& dir, const std::map<std::string, std::string>& post)
{
    lua_getglobal(L, "resolveDirectory");

    auto sp = resolve::Split(dir, "?");
    auto d = sp[0];
    std::map<std::string, std::string> m;
    lua_pushinteger(L, conn);

    lua_newtable(L);
    lua_pushstring(L, "type");
    lua_pushstring(L, type.c_str());
    lua_settable(L, -3);
    lua_pushstring(L, "dir");
    lua_pushstring(L, d.c_str());
    lua_settable(L, -3);
    lua_pushstring(L, "filedir");
    lua_pushstring(L, directory.c_str());
    lua_settable(L, -3);
    lua_pushstring(L, "params");
    lua_newtable(L);
    lua_pushstring(L, "yummyK");
    lua_pushstring(L, "yummyV");
    lua_settable(L, -3);
    int count = 0;
    if (sp.size() > 1)
    {
        auto kvs = resolve::Split(sp[1], "&");
        for (auto p : kvs)
        {
            auto kv = resolve::Split(p, "=");
            if (m.count(kv[0]) == 0)
            {
                count++;
                lua_pushstring(L, kv[0].c_str());
                lua_pushstring(L, kv[1].c_str());
                lua_settable(L, -3);
                m[kv[0]] = kv[1];
            }
        }
    }
    for (auto p : post)
        if (m.count(p.first) == 0)
        {
            count++;
            lua_pushstring(L, p.first.c_str());
            lua_pushstring(L, p.second.c_str());
            lua_settable(L, -3);
            m[p.first] = p.second;
        }
    lua_settable(L, -3);
    lua_pushstring(L, "size");
    lua_pushinteger(L, count);
    lua_settable(L, -3);

    printf("--------------------------------------------------------------------------------\n");
    printf(">> Request directory is %s\n", d.c_str());
    if (count > 0)
        printf(">> There is/are %d parameters pair(s):\n", count);
    for (auto p : m)
        printf(">> key = %s, value = %s\n", p.first.c_str(), p.second.c_str());
    printf("--------------------------------------------------------------------------------\n");

    lua_pcall(L, 2, 0, 0);
}
}
