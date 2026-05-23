#include "../Binding.hpp"
#include "internal/Ref.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <string>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCSpriteFrameCache(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCSpriteFrameCache>("CCSpriteFrameCache",
            sol::no_constructor,
            "sharedSpriteFrameCache", []() {
                return cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
            },
            "addSpriteFramesWithFile", [](cocos2d::CCSpriteFrameCache& self, std::string const& plist) {
                assertMainThread();
                self.addSpriteFramesWithFile(plist.c_str());
            },
            "spriteFrameByName", [](cocos2d::CCSpriteFrameCache& self, std::string const& name) {
                return self.spriteFrameByName(name.c_str());
            },
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCSpriteFrameCache, bindCCSpriteFrameCache)
}
