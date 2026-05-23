#include "Runtime.hpp"

#include "Binding.hpp"

#include <Geode/Geode.hpp>
#include <cassert>
#include <cstdlib>
#include <exception>
#include <string>
#include <utility>

namespace fishrng::lua {
    namespace {
        int luaPanic(lua_State* lua) {
            char const* message = lua_tostring(lua, -1);
            geode::log::error("[lua:panic] {}", message ? message : "unknown panic");
            std::abort();
        }

        int luaExceptionHandler(lua_State* lua, sol::optional<std::exception const&> maybeException, sol::string_view what) {
            std::string message;
            if (maybeException) {
                message = maybeException->what();
            } else {
                message.assign(what.data(), what.size());
            }
            lua_pushlstring(lua, message.data(), message.size());
            return 1;
        }

        void sandboxOs(sol::state& lua) {
            sol::table os = lua["os"];
            os["execute"] = sol::nil;
            os["exit"] = sol::nil;
            os["remove"] = sol::nil;
            os["rename"] = sol::nil;
            os["setlocale"] = sol::nil;
            os["getenv"] = sol::nil;
            os["tmpname"] = sol::nil;
        }
    }

    Runtime::Runtime() : m_ownerThread(std::this_thread::get_id()) {
        m_state.set_panic(&luaPanic);
        m_state.set_exception_handler(&luaExceptionHandler);

        // os, only time/date/clock/difftime
        m_state.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::string,
            sol::lib::math,
            sol::lib::table,
            sol::lib::os
        );
        sandboxOs(m_state);

        auto geode = m_state["geode"].get_or_create<sol::table>();
        applyAllBindings(m_state);
        geode["__api_version"] = bindingApiVersion();

        m_ready = true;
        geode::log::info("sol2 lua runtime ready {}", LUA_RELEASE);
    }

    Runtime::~Runtime() {
        geode::log::info("sol2 lua runtime shutdown");
    }

    Runtime& Runtime::instance() {
        static std::optional<Runtime> runtime;
        if (!runtime) {
            runtime.emplace();
        }
        return *runtime;
    }

    sol::state& Runtime::state() {
        assertMainThread();
        return m_state;
    }

    bool Runtime::ready() const {
        return m_ready;
    }

    bool Runtime::runScript(std::string_view src, std::string_view chunkName) {
        assertMainThread();
        auto chunk = std::string(chunkName);
        auto result = m_state.safe_script(sol::string_view(src.data(), src.size()), sol::script_pass_on_error, chunk);
        if (!result.valid()) {
            sol::error err = result;
            geode::log::error("[lua:{}] {}", chunk, err.what());
            return false;
        }
        return true;
    }

    void Runtime::runOnMain(std::function<void()> fn) {
        geode::queueInMainThread(std::move(fn));
    }

    void Runtime::assertMainThread() const {
#ifndef NDEBUG
        assert(std::this_thread::get_id() == m_ownerThread);
#endif
    }
}
