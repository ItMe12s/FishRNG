#include <Geode/Geode.hpp>
#include <sol/sol.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static sol::state g_lua;
static bool g_luaReady = false;

$on_mod(Loaded) {
    g_lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::math,
        sol::lib::table,
        sol::lib::os,
        sol::lib::io
    );

    g_luaReady = true;
    log::info("sol2 lua runtime {} {}", g_luaReady, LUA_RELEASE);
}

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        if (g_luaReady) g_lua.script(R"(print("Hihi, I'm the FishRNG mod!!!"))");
        return true;
    }
};