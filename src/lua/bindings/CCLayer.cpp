#include "../Binding.hpp"
#include "internal/Ref.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCLayer(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCLayer>("CCLayer",
            sol::no_constructor,
            "create", []() {
                assertMainThread();
                return pushRef(cocos2d::CCLayer::create(), "CCLayer:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCNode, cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCLayer, bindCCLayer)
}
