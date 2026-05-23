#include "../Binding.hpp"
#include "internal/Ref.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCObject(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCObject>("CCObject",
            sol::no_constructor,
            "getTag", &cocos2d::CCObject::getTag,
            "setTag", [](cocos2d::CCObject& self, int tag) {
                assertMainThread();
                self.setTag(tag);
            },
            "retain", [](cocos2d::CCObject& self) {
                retainLuaRef(&self, "CCObject:retain");
            },
            "release", [](cocos2d::CCObject& self) {
                releaseLuaRef(&self, "CCObject:release");
            },
            "retainCount", &cocos2d::CCObject::retainCount
        );
    }

    FISHRNG_LUA_BINDING(CCObject, bindCCObject)
}
