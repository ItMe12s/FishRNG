#include "../Binding.hpp"
#include "internal/Types.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCDirector(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCDirector>("CCDirector",
            sol::no_constructor,
            "sharedDirector", []() {
                return cocos2d::CCDirector::sharedDirector();
            },
            "getWinSize", [](sol::this_state state, cocos2d::CCDirector& self) {
                return pushSize(state, self.getWinSize());
            },
            "getVisibleOrigin", [](sol::this_state state, cocos2d::CCDirector& self) {
                return pushPoint(state, self.getVisibleOrigin());
            },
            "getVisibleSize", [](sol::this_state state, cocos2d::CCDirector& self) {
                return pushSize(state, self.getVisibleSize());
            },
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCDirector, bindCCDirector)
}
