#pragma once

#include <sol/sol.hpp>

namespace fishrng::lua {
    using Registrar = void(*)(sol::state&);

    struct Binding {
        char const* name;
        Registrar fn;
    };

    void registerBinding(Binding const& binding);
    void applyAllBindings(sol::state& lua);
}

// Borrowed Cocos pointers stay non-owning. Factories return Lua-owned refs from bindings/internal/Ref.hpp.
#define FISHRNG_LUA_BINDING(NAME, FN)                          \
    namespace {                                                \
        struct AutoReg_##NAME {                                \
            AutoReg_##NAME() {                                 \
                ::fishrng::lua::registerBinding({#NAME, &FN}); \
            }                                                  \
        } _autoreg_##NAME;                                     \
    }
