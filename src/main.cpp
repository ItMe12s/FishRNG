#include <Geode/Geode.hpp>
#include <sol/sol.hpp>

using namespace geode::prelude;

static sol::state g_lua;

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

    log::info("sol2 lua runtime ready ({})", LUA_RELEASE);
}
