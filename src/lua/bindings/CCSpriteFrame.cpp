#include "../Binding.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>

namespace {
    void bindCCSpriteFrame(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCSpriteFrame>("CCSpriteFrame",
            sol::no_constructor,
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCSpriteFrame, bindCCSpriteFrame)
}
