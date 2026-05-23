#include "Binding.hpp"

#include <Geode/Geode.hpp>
#include <exception>
#include <vector>

namespace fishrng::lua {
    namespace {
        constexpr int ApiVersion = 1;

        std::vector<Binding>& bindings() {
            static std::vector<Binding> value;
            return value;
        }
    }

    void registerBinding(Binding const& binding) {
        bindings().push_back(binding);
    }

    void applyAllBindings(sol::state& lua) {
        for (auto const& binding : bindings()) {
            try {
                binding.fn(lua);
            } catch (std::exception const& e) {
                geode::log::error("[lua:bind:{}] {}", binding.name, e.what());
            } catch (...) {
                geode::log::error("[lua:bind:{}] unknown exception", binding.name);
            }
        }
    }

    int bindingApiVersion() {
        return ApiVersion;
    }
}
