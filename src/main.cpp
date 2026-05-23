#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "lua/Runtime.hpp"

#include <fstream>
#include <sstream>
#include <string>

using namespace geode::prelude;

$on_mod(Loaded) {
    fishrng::lua::Runtime::instance();
}

namespace {
    std::string readBootstrapScript() {
        auto path = Mod::get()->getResourcesDir() / "bootstrap.lua";
        std::ifstream file(path);
        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        fishrng::lua::Runtime::instance().runScript(readBootstrapScript(), "bootstrap.lua");
        return true;
    }
};
