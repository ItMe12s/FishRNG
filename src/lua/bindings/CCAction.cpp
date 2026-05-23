#include "../Binding.hpp"
#include "internal/Ref.hpp"
#include "internal/Validate.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <stdexcept>
#include <string>

namespace {
    using namespace fishrng::lua::bindings;

    cocos2d::CCArray* actionsFromTable(sol::table const& table, char const* method) {
        auto count = static_cast<unsigned int>(table.size());
        if (count == 0) {
            throw std::runtime_error(std::string(method) + " expected at least one action");
        }

        auto array = cocos2d::CCArray::createWithCapacity(count);
        for (unsigned int i = 1; i <= count; ++i) {
            auto object = table.get<sol::object>(i);
            auto action = object.as<sol::optional<cocos2d::CCFiniteTimeAction*>>();
            array->addObject(requireFiniteTimeAction(action.value_or(nullptr), method));
        }
        return array;
    }

    void bindCCAction(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();
        auto actions = cocos["actions"].get_or_create<sol::table>();

        actions.new_usertype<cocos2d::CCAction>("CCAction",
            sol::no_constructor,
            "getTag", &cocos2d::CCAction::getTag,
            "setTag", [](cocos2d::CCAction& self, int tag) {
                assertMainThread();
                self.setTag(tag);
            },
            "isDone", &cocos2d::CCAction::isDone,
            "retain", [](cocos2d::CCAction& self) {
                retainLuaRef(&self, "CCAction:retain");
            },
            "release", [](cocos2d::CCAction& self) {
                releaseLuaRef(&self, "CCAction:release");
            },
            "retainCount", &cocos2d::CCAction::retainCount,
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCFiniteTimeAction>("CCFiniteTimeAction",
            sol::no_constructor,
            "getDuration", &cocos2d::CCFiniteTimeAction::getDuration,
            "setDuration", [](cocos2d::CCFiniteTimeAction& self, float duration) {
                assertMainThread();
                self.setDuration(duration);
            },
            sol::base_classes, sol::bases<cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCActionInterval>("CCActionInterval",
            sol::no_constructor,
            "getElapsed", &cocos2d::CCActionInterval::getElapsed,
            sol::base_classes, sol::bases<cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCMoveTo>("CCMoveTo",
            sol::no_constructor,
            "create", [](float duration, float x, float y) {
                assertMainThread();
                return pushRef(cocos2d::CCMoveTo::create(duration, {x, y}), "CCMoveTo:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCMoveBy>("CCMoveBy",
            sol::no_constructor,
            "create", [](float duration, float x, float y) {
                assertMainThread();
                return pushRef(cocos2d::CCMoveBy::create(duration, {x, y}), "CCMoveBy:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCScaleTo>("CCScaleTo",
            sol::no_constructor,
            "create", sol::overload(
                [](float duration, float scale) {
                    assertMainThread();
                    return pushRef(cocos2d::CCScaleTo::create(duration, scale), "CCScaleTo:create");
                },
                [](float duration, float scaleX, float scaleY) {
                    assertMainThread();
                    return pushRef(cocos2d::CCScaleTo::create(duration, scaleX, scaleY), "CCScaleTo:create");
                }
            ),
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCFadeIn>("CCFadeIn",
            sol::no_constructor,
            "create", [](float duration) {
                assertMainThread();
                return pushRef(cocos2d::CCFadeIn::create(duration), "CCFadeIn:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCFadeOut>("CCFadeOut",
            sol::no_constructor,
            "create", [](float duration) {
                assertMainThread();
                return pushRef(cocos2d::CCFadeOut::create(duration), "CCFadeOut:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCDelayTime>("CCDelayTime",
            sol::no_constructor,
            "create", [](float duration) {
                assertMainThread();
                return pushRef(cocos2d::CCDelayTime::create(duration), "CCDelayTime:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCSequence>("CCSequence",
            sol::no_constructor,
            "create", [](sol::table table) {
                assertMainThread();
                return pushRef(cocos2d::CCSequence::create(actionsFromTable(table, "CCSequence:create")), "CCSequence:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );

        actions.new_usertype<cocos2d::CCRepeatForever>("CCRepeatForever",
            sol::no_constructor,
            "create", [](cocos2d::CCActionInterval* action) {
                assertMainThread();
                return pushRef(cocos2d::CCRepeatForever::create(
                    requireActionInterval(action, "CCRepeatForever:create")
                ), "CCRepeatForever:create");
            },
            sol::base_classes, sol::bases<cocos2d::CCActionInterval, cocos2d::CCFiniteTimeAction, cocos2d::CCAction, cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCAction, bindCCAction)
}
