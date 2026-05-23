#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "lua/Runtime.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    fishrng::lua::Runtime::instance();
}

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto& runtime = fishrng::lua::Runtime::instance();
        if (runtime.ready()) {
            runtime.runScript(R"(print("Hihi, I'm the FishRNG mod!!!"))");
        }
        return true;
    }
};
