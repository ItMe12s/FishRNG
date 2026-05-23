#pragma once

#include "../../Runtime.hpp"

#include <cocos2d.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace fishrng::lua::bindings {
    template <class T>
    using RefPtr = std::shared_ptr<T>;

    inline std::unordered_map<cocos2d::CCObject*, unsigned int>& luaRetains() {
        static auto* value = new std::unordered_map<cocos2d::CCObject*, unsigned int>();
        return *value;
    }

    inline void assertMainThread() {
        fishrng::lua::Runtime::instance().assertMainThread();
    }

    inline void releaseLuaRetain(cocos2d::CCObject* object, char const* method, bool explicitRelease) {
        if (!object) {
            return;
        }

        auto& retains = luaRetains();
        auto found = retains.find(object);
        if (found == retains.end() || found->second == 0) {
            if (explicitRelease) {
                throw std::runtime_error(std::string(method) + " has no Lua-owned retain");
            }
            return;
        }

        found->second -= 1;
        if (found->second == 0) {
            retains.erase(found);
        }
        object->release();
    }

    inline void retainLuaRef(cocos2d::CCObject* object, char const* method) {
        if (!object) {
            throw std::runtime_error(std::string(method) + " expected CCObject");
        }
        assertMainThread();
        object->retain();
        luaRetains()[object] += 1;
    }

    inline void releaseLuaRef(cocos2d::CCObject* object, char const* method) {
        assertMainThread();
        releaseLuaRetain(object, method, true);
    }

    template <class T>
    RefPtr<T> pushRef(T* object, char const* method) {
        static_assert(std::is_base_of_v<cocos2d::CCObject, T>);
        if (!object) {
            throw std::runtime_error(std::string(method) + " returned null");
        }

        retainLuaRef(object, method);
        return RefPtr<T>(object, [](T* value) {
            releaseLuaRetain(value, "__gc", false);
        });
    }
}
