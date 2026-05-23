#include "../Binding.hpp"
#include "internal/Ref.hpp"
#include "internal/Validate.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <string>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCSprite(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCSprite>("CCSprite",
            sol::no_constructor,
            "create", []() {
                assertMainThread();
                return pushRef(cocos2d::CCSprite::create(), "CCSprite:create");
            },
            "createWithSpriteFrameName", [](std::string const& name) {
                assertMainThread();
                return pushRef(cocos2d::CCSprite::createWithSpriteFrameName(name.c_str()), "CCSprite:createWithSpriteFrameName");
            },
            "createWithSpriteFrame", [](cocos2d::CCSpriteFrame* frame) {
                assertMainThread();
                return pushRef(cocos2d::CCSprite::createWithSpriteFrame(
                    requireSpriteFrame(frame, "CCSprite:createWithSpriteFrame")
                ), "CCSprite:createWithSpriteFrame");
            },
            "setFlipX", [](cocos2d::CCSprite& self, bool flip) {
                assertMainThread();
                self.setFlipX(flip);
            },
            "setFlipY", [](cocos2d::CCSprite& self, bool flip) {
                assertMainThread();
                self.setFlipY(flip);
            },
            "isFlipX", &cocos2d::CCSprite::isFlipX,
            "isFlipY", &cocos2d::CCSprite::isFlipY,
            "setDisplayFrame", [](cocos2d::CCSprite& self, cocos2d::CCSpriteFrame* frame) {
                assertMainThread();
                self.setDisplayFrame(requireSpriteFrame(frame, "CCSprite:setDisplayFrame"));
            },
            sol::base_classes, sol::bases<cocos2d::CCNode, cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCSprite, bindCCSprite)
}
