#include "../Binding.hpp"
#include "internal/Ref.hpp"
#include "internal/Stack.hpp"
#include "internal/TableUtil.hpp"
#include "internal/Usertype.hpp"
#include "internal/Validate.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <lua.h>
#include <stdexcept>
#include <string>

namespace {
    using namespace luax;

    cocos2d::CCArray* actionsFromTable(lua_State* L, int idx, char const* method) {
        if (lua_type(L, idx) != LUA_TTABLE) {
            luaL_error(L, "%s expected table of actions", method);
        }
        int count = static_cast<int>(lua_objlen(L, idx));
        if (count == 0) {
            luaL_error(L, "%s expected at least one action", method);
        }
        auto array = cocos2d::CCArray::createWithCapacity(static_cast<unsigned int>(count));
        for (int i = 1; i <= count; ++i) {
            lua_rawgeti(L, idx, i);
            auto action = Usertype<cocos2d::CCFiniteTimeAction>::check(L, -1, method);
            array->addObject(requireFiniteTimeAction(action, method));
            lua_pop(L, 1);
        }
        return array;
    }

    int ccaction_getTag(lua_State* L) {
        push(L, Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:getTag")->getTag());
        return 1;
    }
    int ccaction_setTag(lua_State* L) {
        auto self = Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:setTag");
        assertMainThread();
        self->setTag(check<int>(L, 2, "CCAction:setTag"));
        return 0;
    }
    int ccaction_isDone(lua_State* L) {
        push(L, Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:isDone")->isDone());
        return 1;
    }
    int ccaction_retain(lua_State* L) {
        auto self = Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:retain");
        retainLuaRef(self, "CCAction:retain");
        return 0;
    }
    int ccaction_release(lua_State* L) {
        auto self = Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:release");
        releaseLuaRef(self, "CCAction:release");
        return 0;
    }
    int ccaction_retainCount(lua_State* L) {
        push(L, Usertype<cocos2d::CCAction>::check(L, 1, "CCAction:retainCount")->retainCount());
        return 1;
    }

    int ccfinitetimeaction_getDuration(lua_State* L) {
        push(L, Usertype<cocos2d::CCFiniteTimeAction>::check(L, 1, "CCFiniteTimeAction:getDuration")->getDuration());
        return 1;
    }
    int ccfinitetimeaction_setDuration(lua_State* L) {
        auto self = Usertype<cocos2d::CCFiniteTimeAction>::check(L, 1, "CCFiniteTimeAction:setDuration");
        assertMainThread();
        self->setDuration(check<float>(L, 2, "CCFiniteTimeAction:setDuration"));
        return 0;
    }

    int ccactioninterval_getElapsed(lua_State* L) {
        push(L, Usertype<cocos2d::CCActionInterval>::check(L, 1, "CCActionInterval:getElapsed")->getElapsed());
        return 1;
    }

    int ccmoveto_create(lua_State* L) {
        assertMainThread();
        float d = check<float>(L, 1, "CCMoveTo:create");
        float x = check<float>(L, 2, "CCMoveTo:create");
        float y = check<float>(L, 3, "CCMoveTo:create");
        Usertype<cocos2d::CCMoveTo>::pushOwned(L, cocos2d::CCMoveTo::create(d, { x, y }));
        return 1;
    }
    int ccmoveby_create(lua_State* L) {
        assertMainThread();
        float d = check<float>(L, 1, "CCMoveBy:create");
        float x = check<float>(L, 2, "CCMoveBy:create");
        float y = check<float>(L, 3, "CCMoveBy:create");
        Usertype<cocos2d::CCMoveBy>::pushOwned(L, cocos2d::CCMoveBy::create(d, { x, y }));
        return 1;
    }
    int ccscaleto_create(lua_State* L) {
        assertMainThread();
        float d = check<float>(L, 1, "CCScaleTo:create");
        if (lua_gettop(L) == 2) {
            Usertype<cocos2d::CCScaleTo>::pushOwned(L, cocos2d::CCScaleTo::create(d, check<float>(L, 2, "CCScaleTo:create")));
        } else {
            float sx = check<float>(L, 2, "CCScaleTo:create");
            float sy = check<float>(L, 3, "CCScaleTo:create");
            Usertype<cocos2d::CCScaleTo>::pushOwned(L, cocos2d::CCScaleTo::create(d, sx, sy));
        }
        return 1;
    }
    int ccfadein_create(lua_State* L) {
        assertMainThread();
        Usertype<cocos2d::CCFadeIn>::pushOwned(L, cocos2d::CCFadeIn::create(check<float>(L, 1, "CCFadeIn:create")));
        return 1;
    }
    int ccfadeout_create(lua_State* L) {
        assertMainThread();
        Usertype<cocos2d::CCFadeOut>::pushOwned(L, cocos2d::CCFadeOut::create(check<float>(L, 1, "CCFadeOut:create")));
        return 1;
    }
    int ccdelaytime_create(lua_State* L) {
        assertMainThread();
        Usertype<cocos2d::CCDelayTime>::pushOwned(L, cocos2d::CCDelayTime::create(check<float>(L, 1, "CCDelayTime:create")));
        return 1;
    }
    int ccsequence_create(lua_State* L) {
        assertMainThread();
        auto arr = actionsFromTable(L, 1, "CCSequence:create");
        Usertype<cocos2d::CCSequence>::pushOwned(L, cocos2d::CCSequence::create(arr));
        return 1;
    }
    int ccrepeatforever_create(lua_State* L) {
        assertMainThread();
        auto action = Usertype<cocos2d::CCActionInterval>::check(L, 1, "CCRepeatForever:create");
        Usertype<cocos2d::CCRepeatForever>::pushOwned(L,
            cocos2d::CCRepeatForever::create(requireActionInterval(action, "CCRepeatForever:create")));
        return 1;
    }

    // Caller must leave the geode.cocos2d.actions table on top of the stack.
    void exposeFactoryTable(lua_State* L, char const* name, lua_CFunction createFn) {
        lua_createtable(L, 0, 1);
        lua_pushcfunction(L, createFn, name);
        lua_setfield(L, -2, "create");
        lua_setfield(L, -2, name);
    }

    void bindCCAction(lua_State* L) {
        Usertype<cocos2d::CCAction>::registerType(L, "CCAction", { Usertype<cocos2d::CCObject>::tag() });
        Usertype<cocos2d::CCAction>::method(L, "getTag", &ccaction_getTag);
        Usertype<cocos2d::CCAction>::method(L, "setTag", &ccaction_setTag);
        Usertype<cocos2d::CCAction>::method(L, "isDone", &ccaction_isDone);
        Usertype<cocos2d::CCAction>::method(L, "retain", &ccaction_retain);
        Usertype<cocos2d::CCAction>::method(L, "release", &ccaction_release);
        Usertype<cocos2d::CCAction>::method(L, "retainCount", &ccaction_retainCount);

        Usertype<cocos2d::CCFiniteTimeAction>::registerType(L, "CCFiniteTimeAction", { Usertype<cocos2d::CCAction>::tag() });
        Usertype<cocos2d::CCFiniteTimeAction>::method(L, "getDuration", &ccfinitetimeaction_getDuration);
        Usertype<cocos2d::CCFiniteTimeAction>::method(L, "setDuration", &ccfinitetimeaction_setDuration);

        Usertype<cocos2d::CCActionInterval>::registerType(L, "CCActionInterval", { Usertype<cocos2d::CCFiniteTimeAction>::tag() });
        Usertype<cocos2d::CCActionInterval>::method(L, "getElapsed", &ccactioninterval_getElapsed);

        Usertype<cocos2d::CCMoveTo>::registerType(L, "CCMoveTo", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCMoveBy>::registerType(L, "CCMoveBy", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCScaleTo>::registerType(L, "CCScaleTo", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCFadeIn>::registerType(L, "CCFadeIn", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCFadeOut>::registerType(L, "CCFadeOut", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCDelayTime>::registerType(L, "CCDelayTime", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCSequence>::registerType(L, "CCSequence", { Usertype<cocos2d::CCActionInterval>::tag() });
        Usertype<cocos2d::CCRepeatForever>::registerType(L, "CCRepeatForever", { Usertype<cocos2d::CCActionInterval>::tag() });

        getOrCreateTable(L, "geode.cocos2d.actions");
        exposeFactoryTable(L, "CCMoveTo", &ccmoveto_create);
        exposeFactoryTable(L, "CCMoveBy", &ccmoveby_create);
        exposeFactoryTable(L, "CCScaleTo", &ccscaleto_create);
        exposeFactoryTable(L, "CCFadeIn", &ccfadein_create);
        exposeFactoryTable(L, "CCFadeOut", &ccfadeout_create);
        exposeFactoryTable(L, "CCDelayTime", &ccdelaytime_create);
        exposeFactoryTable(L, "CCSequence", &ccsequence_create);
        exposeFactoryTable(L, "CCRepeatForever", &ccrepeatforever_create);
        lua_pop(L, 1);
    }

    LUAX_BINDING_PRIORITY(CCAction, bindCCAction, 2)
}
