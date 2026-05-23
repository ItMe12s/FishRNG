#include "../Binding.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <string>

namespace {
    void bindModApi(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        geode["modResourcesPath"] = []() {
            return geode::Mod::get()->getResourcesDir().string();
        };
    }

    FISHRNG_LUA_BINDING(ModApi, bindModApi)
}
