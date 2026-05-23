#pragma once

#include <sol/sol.hpp>

#include <functional>
#include <optional>
#include <string_view>
#include <thread>

namespace fishrng::lua {
    // Runtime state is owned by the thread that constructs it. Cocos bindings assert this owner thread before mutation.
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
        void assertMainThread() const;

    private:
        sol::state m_state;
        std::thread::id m_ownerThread;
        bool m_ready = false;
    };
}
