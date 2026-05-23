#pragma once

#include <cocos2d.h>
#include <stdexcept>
#include <string>

namespace fishrng::lua::bindings {
    template <class T>
    T* requirePtr(T* value, char const* method, char const* typeName) {
        if (!value) {
            throw std::runtime_error(std::string(method) + " expected " + typeName);
        }
        return value;
    }

    inline cocos2d::CCObject* requireObject(cocos2d::CCObject* object, char const* method) {
        return requirePtr(object, method, "CCObject");
    }

    inline cocos2d::CCNode* requireNode(cocos2d::CCNode* node, char const* method) {
        return requirePtr(node, method, "CCNode");
    }

    inline cocos2d::CCSprite* requireSprite(cocos2d::CCSprite* sprite, char const* method) {
        return requirePtr(sprite, method, "CCSprite");
    }

    inline cocos2d::CCLayer* requireLayer(cocos2d::CCLayer* layer, char const* method) {
        return requirePtr(layer, method, "CCLayer");
    }

    inline cocos2d::CCAction* requireAction(cocos2d::CCAction* action, char const* method) {
        return requirePtr(action, method, "CCAction");
    }

    inline cocos2d::CCFiniteTimeAction* requireFiniteTimeAction(cocos2d::CCFiniteTimeAction* action, char const* method) {
        return requirePtr(action, method, "CCFiniteTimeAction");
    }

    inline cocos2d::CCActionInterval* requireActionInterval(cocos2d::CCActionInterval* action, char const* method) {
        return requirePtr(action, method, "CCActionInterval");
    }

    inline cocos2d::CCSpriteFrame* requireSpriteFrame(cocos2d::CCSpriteFrame* frame, char const* method) {
        return requirePtr(frame, method, "CCSpriteFrame");
    }
}
