#include "Binding.hpp"

#include <Geode/Geode.hpp>
#include <algorithm>
#include <exception>
#include <string_view>
#include <vector>

namespace fishrng::lua {
    namespace {
        std::vector<Binding>& bindings() {
            static std::vector<Binding> value;
            return value;
        }

        int bindingPriority(char const* name) {
            using namespace std::string_view_literals;
            auto value = std::string_view(name);
            if (value == "CCObject"sv) return 0;
            if (value == "CCNode"sv) return 1;
            if (value == "CCAction"sv) return 2;
            if (value == "CCSpriteFrame"sv) return 2;
            return 10;
        }
    }

    void registerBinding(Binding const& binding) {
        bindings().push_back(binding);
    }

    void applyAllBindings(sol::state& lua) {
        auto ordered = bindings();
        std::stable_sort(ordered.begin(), ordered.end(), [](auto const& left, auto const& right) {
            return bindingPriority(left.name) < bindingPriority(right.name);
        });

        for (auto const& binding : ordered) {
            try {
                binding.fn(lua);
            } catch (std::exception const& e) {
                geode::log::error("[lua:bind:{}] {}", binding.name, e.what());
            } catch (...) {
                geode::log::error("[lua:bind:{}] unknown exception", binding.name);
            }
        }
    }

}
