#include "../Binding.hpp"

#include <Geode/Geode.hpp>
#include <Geode/ui/OverlayManager.hpp>
#include <cocos2d.h>

namespace {
    void bindOverlayManager(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        geode.new_usertype<geode::OverlayManager>("OverlayManager",
            sol::no_constructor,
            "get", &geode::OverlayManager::get,
            sol::base_classes, sol::bases<cocos2d::CCNode, cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(OverlayManager, bindOverlayManager)
}
