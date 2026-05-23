#include "../Binding.hpp"
#include "internal/Ref.hpp"
#include "internal/Validate.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCNode(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        auto ccNode = cocos.new_usertype<cocos2d::CCNode>("CCNode",
            sol::no_constructor,
            "create", []() {
                assertMainThread();
                return pushRef(cocos2d::CCNode::create(), "CCNode:create");
            },
            "addChild", sol::overload(
                [](cocos2d::CCNode& self, cocos2d::CCNode* child) {
                    assertMainThread();
                    self.addChild(requireNode(child, "CCNode:addChild"));
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, int z) {
                    assertMainThread();
                    self.addChild(requireNode(child, "CCNode:addChild"), z);
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, int z, int tag) {
                    assertMainThread();
                    self.addChild(requireNode(child, "CCNode:addChild"), z, tag);
                }
            ),
            "removeChild", sol::overload(
                [](cocos2d::CCNode& self, cocos2d::CCNode* child) {
                    assertMainThread();
                    self.removeChild(requireNode(child, "CCNode:removeChild"), true);
                },
                [](cocos2d::CCNode& self, cocos2d::CCNode* child, bool cleanup) {
                    assertMainThread();
                    self.removeChild(requireNode(child, "CCNode:removeChild"), cleanup);
                }
            ),
            "removeChildByTag", sol::overload(
                [](cocos2d::CCNode& self, int tag) {
                    assertMainThread();
                    self.removeChildByTag(tag, true);
                },
                [](cocos2d::CCNode& self, int tag, bool cleanup) {
                    assertMainThread();
                    self.removeChildByTag(tag, cleanup);
                }
            ),
            "removeAllChildrenWithCleanup", [](cocos2d::CCNode& self, bool cleanup) {
                assertMainThread();
                self.removeAllChildrenWithCleanup(cleanup);
            },
            "removeFromParentAndCleanup", [](cocos2d::CCNode& self, bool cleanup) {
                assertMainThread();
                self.removeFromParentAndCleanup(cleanup);
            },
            "getChildByTag", &cocos2d::CCNode::getChildByTag,
            "getChildByID", [](cocos2d::CCNode& self, std::string const& id) {
                return self.getChildByID(id);
            },
            "getChildrenCount", &cocos2d::CCNode::getChildrenCount,
            "getParent", &cocos2d::CCNode::getParent,
            "setPosition", [](cocos2d::CCNode& self, float x, float y) {
                assertMainThread();
                self.setPosition({x, y});
            },
            "getPositionX", &cocos2d::CCNode::getPositionX,
            "getPositionY", &cocos2d::CCNode::getPositionY,
            "setPositionX", [](cocos2d::CCNode& self, float x) {
                assertMainThread();
                self.setPositionX(x);
            },
            "setPositionY", [](cocos2d::CCNode& self, float y) {
                assertMainThread();
                self.setPositionY(y);
            },
            "getScale", &cocos2d::CCNode::getScale,
            "setScale", [](cocos2d::CCNode& self, float scale) {
                assertMainThread();
                self.setScale(scale);
            },
            "getScaleX", &cocos2d::CCNode::getScaleX,
            "getScaleY", &cocos2d::CCNode::getScaleY,
            "setScaleX", [](cocos2d::CCNode& self, float scale) {
                assertMainThread();
                self.setScaleX(scale);
            },
            "setScaleY", [](cocos2d::CCNode& self, float scale) {
                assertMainThread();
                self.setScaleY(scale);
            },
            "getRotation", &cocos2d::CCNode::getRotation,
            "setRotation", [](cocos2d::CCNode& self, float rotation) {
                assertMainThread();
                self.setRotation(rotation);
            },
            "getAnchorPoint", [](cocos2d::CCNode& self) {
                auto const& point = self.getAnchorPoint();
                return std::make_tuple(point.x, point.y);
            },
            "setAnchorPoint", [](cocos2d::CCNode& self, float x, float y) {
                assertMainThread();
                self.setAnchorPoint({x, y});
            },
            "getContentSize", [](cocos2d::CCNode& self) {
                auto const& size = self.getContentSize();
                return std::make_tuple(size.width, size.height);
            },
            "setContentSize", [](cocos2d::CCNode& self, float width, float height) {
                assertMainThread();
                self.setContentSize({width, height});
            },
            "isVisible", &cocos2d::CCNode::isVisible,
            "setVisible", [](cocos2d::CCNode& self, bool visible) {
                assertMainThread();
                self.setVisible(visible);
            },
            "getZOrder", &cocos2d::CCNode::getZOrder,
            "setZOrder", [](cocos2d::CCNode& self, int z) {
                assertMainThread();
                self.setZOrder(z);
            },
            "getTag", &cocos2d::CCNode::getTag,
            "setTag", [](cocos2d::CCNode& self, int tag) {
                assertMainThread();
                self.setTag(tag);
            },
            "getID", [](cocos2d::CCNode& self) {
                return std::string(std::string_view(self.getID()));
            },
            "setID", [](cocos2d::CCNode& self, std::string id) {
                assertMainThread();
                self.setID(std::move(id));
            },
            "isRunning", &cocos2d::CCNode::isRunning,
            "runAction", [](cocos2d::CCNode& self, cocos2d::CCAction* action) {
                assertMainThread();
                return self.runAction(requireAction(action, "CCNode:runAction"));
            },
            "stopAllActions", [](cocos2d::CCNode& self) {
                assertMainThread();
                self.stopAllActions();
            },
            "scheduleUpdate", [](cocos2d::CCNode& self) {
                assertMainThread();
                self.scheduleUpdate();
            },
            "unscheduleUpdate", [](cocos2d::CCNode& self) {
                assertMainThread();
                self.unscheduleUpdate();
            },
            sol::base_classes, sol::bases<cocos2d::CCObject>()
        );

        geode["CCNode"] = ccNode;
    }

    FISHRNG_LUA_BINDING(CCNode, bindCCNode)
}
