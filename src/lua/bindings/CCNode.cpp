#include "../Binding.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>

namespace {
    cocos2d::CCNode* requireNode(cocos2d::CCNode* node, char const* method) {
        if (!node) {
            throw std::runtime_error(std::string(method) + " expected CCNode");
        }
        return node;
    }

    cocos2d::CCAction* requireAction(cocos2d::CCAction* action, char const* method) {
        if (!action) {
            throw std::runtime_error(std::string(method) + " expected CCAction");
        }
        return action;
    }

    void bindCCNode(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCObject>("CCObject",
            sol::no_constructor,
            "getTag", &cocos2d::CCObject::getTag,
            "setTag", &cocos2d::CCObject::setTag,
            "retainCount", &cocos2d::CCObject::retainCount
        );

        auto ccNode = cocos.new_usertype<cocos2d::CCNode>("CCNode",
            sol::no_constructor,
            "addChild", sol::overload(
                [](cocos2d::CCNode& self, cocos2d::CCNode* child) {
                    self.addChild(requireNode(child, "CCNode:addChild"));
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, int z) {
                    self.addChild(requireNode(child, "CCNode:addChild"), z);
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, int z, int tag) {
                    self.addChild(requireNode(child, "CCNode:addChild"), z, tag);
                }
            ),
            "removeChild", sol::overload(
                [](cocos2d::CCNode& self, cocos2d::CCNode* child) {
                    self.removeChild(requireNode(child, "CCNode:removeChild"), true);
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, bool cleanup) {
                    self.removeChild(requireNode(child, "CCNode:removeChild"), cleanup);
                }
            ),
            "removeChildByTag", sol::overload(
                [](cocos2d::CCNode& self, int tag) { self.removeChildByTag(tag, true); },
                [](cocos2d::CCNode& self, int tag, bool cleanup) { self.removeChildByTag(tag, cleanup); }
            ),
            "removeAllChildrenWithCleanup", &cocos2d::CCNode::removeAllChildrenWithCleanup,
            "removeFromParentAndCleanup", &cocos2d::CCNode::removeFromParentAndCleanup,
            "getChildByTag", &cocos2d::CCNode::getChildByTag,
            "getChildByID", [](cocos2d::CCNode& self, std::string const& id) {
                return self.getChildByID(id);
            },
            "getChildrenCount", &cocos2d::CCNode::getChildrenCount,
            "getParent", &cocos2d::CCNode::getParent,
            "setPosition", [](cocos2d::CCNode& self, float x, float y) {
                self.setPosition({x, y});
            },
            "getPositionX", &cocos2d::CCNode::getPositionX,
            "getPositionY", &cocos2d::CCNode::getPositionY,
            "setPositionX", &cocos2d::CCNode::setPositionX,
            "setPositionY", &cocos2d::CCNode::setPositionY,
            "getScale", &cocos2d::CCNode::getScale,
            "setScale", sol::resolve<void(float)>(&cocos2d::CCNode::setScale),
            "getScaleX", &cocos2d::CCNode::getScaleX,
            "getScaleY", &cocos2d::CCNode::getScaleY,
            "setScaleX", &cocos2d::CCNode::setScaleX,
            "setScaleY", &cocos2d::CCNode::setScaleY,
            "getRotation", &cocos2d::CCNode::getRotation,
            "setRotation", &cocos2d::CCNode::setRotation,
            "getAnchorPoint", [](cocos2d::CCNode& self) {
                auto const& point = self.getAnchorPoint();
                return std::make_tuple(point.x, point.y);
            },
            "setAnchorPoint", [](cocos2d::CCNode& self, float x, float y) {
                self.setAnchorPoint({x, y});
            },
            "getContentSize", [](cocos2d::CCNode& self) {
                auto const& size = self.getContentSize();
                return std::make_tuple(size.width, size.height);
            },
            "setContentSize", [](cocos2d::CCNode& self, float width, float height) {
                self.setContentSize({width, height});
            },
            "isVisible", &cocos2d::CCNode::isVisible,
            "setVisible", &cocos2d::CCNode::setVisible,
            "getZOrder", &cocos2d::CCNode::getZOrder,
            "setZOrder", &cocos2d::CCNode::setZOrder,
            "getTag", &cocos2d::CCNode::getTag,
            "setTag", &cocos2d::CCNode::setTag,
            "getID", [](cocos2d::CCNode& self) {
                return std::string(std::string_view(self.getID()));
            },
            "setID", [](cocos2d::CCNode& self, std::string id) {
                self.setID(std::move(id));
            },
            "isRunning", &cocos2d::CCNode::isRunning,
            "runAction", [](cocos2d::CCNode& self, cocos2d::CCAction* action) {
                return self.runAction(requireAction(action, "CCNode:runAction"));
            },
            "stopAllActions", &cocos2d::CCNode::stopAllActions,
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );

        geode["CCNode"] = ccNode;
    }

    FISHRNG_LUA_BINDING(CCNode, bindCCNode)
}
