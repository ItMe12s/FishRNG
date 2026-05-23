#include "../Binding.hpp"
#include "internal/Ref.hpp"
#include "internal/Types.hpp"

#include <Geode/Geode.hpp>
#include <cocos2d.h>
#include <string>

namespace {
    using namespace fishrng::lua::bindings;

    void bindCCLabelBMFont(sol::state& lua) {
        auto geode = lua["geode"].get_or_create<sol::table>();
        auto cocos = geode["cocos2d"].get_or_create<sol::table>();

        cocos.new_usertype<cocos2d::CCLabelBMFont>("CCLabelBMFont",
            sol::no_constructor,
            "create", [](std::string const& text, std::string const& fontFile) {
                assertMainThread();
                return pushRef(cocos2d::CCLabelBMFont::create(text.c_str(), fontFile.c_str()), "CCLabelBMFont:create");
            },
            "setString", [](cocos2d::CCLabelBMFont& self, std::string const& text) {
                assertMainThread();
                self.setString(text.c_str());
            },
            "getString", [](cocos2d::CCLabelBMFont& self) {
                return std::string(self.getString());
            },
            "setColor", [](cocos2d::CCLabelBMFont& self, sol::table color) {
                assertMainThread();
                self.setColor(toColor3B(color, "CCLabelBMFont:setColor"));
            },
            sol::base_classes, sol::bases<cocos2d::CCNode, cocos2d::CCObject>()
        );
    }

    FISHRNG_LUA_BINDING(CCLabelBMFont, bindCCLabelBMFont)
}
