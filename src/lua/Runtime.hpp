#pragma once

#include <sol/sol.hpp>

#include <functional>
#include <optional>
#include <string_view>
#include <thread>

namespace fishrng::lua {
    // Owns a Lua state, main thread only, debug checks thread.
    class Runtime final {
    public:
        Runtime();
        ~Runtime();

        Runtime(Runtime const&) = delete;
        Runtime& operator=(Runtime const&) = delete;

        static Runtime& instance();

        sol::state& state();
        bool ready() const;
        bool runScript(std::string_view src, std::string_view chunkName);
        void runOnMain(std::function<void()> fn);

    private:
        void assertMainThread() const;

        sol::state m_state;
        std::thread::id m_ownerThread;
        bool m_ready = false;
    };
}
