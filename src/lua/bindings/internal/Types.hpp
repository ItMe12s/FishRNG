#pragma once

#include <cocos2d.h>
#include <sol/sol.hpp>
#include <stdexcept>
#include <string>

namespace fishrng::lua::bindings {
    inline float requireNumber(sol::table const& table, char const* key, char const* method) {
        sol::optional<float> value = table[key];
        if (!value) {
            throw std::runtime_error(std::string(method) + " expected number field " + key);
        }
        return *value;
    }

    inline cocos2d::CCPoint toPoint(float x, float y) {
        return {x, y};
    }

    inline cocos2d::CCPoint toPoint(sol::table const& table, char const* method) {
        return {requireNumber(table, "x", method), requireNumber(table, "y", method)};
    }

    inline cocos2d::CCSize toSize(float width, float height) {
        return {width, height};
    }

    inline cocos2d::CCSize toSize(sol::table const& table, char const* method) {
        return {requireNumber(table, "width", method), requireNumber(table, "height", method)};
    }

    inline cocos2d::CCRect toRect(float x, float y, float width, float height) {
        return {{x, y}, {width, height}};
    }

    inline cocos2d::CCRect toRect(sol::table const& table, char const* method) {
        return toRect(
            requireNumber(table, "x", method),
            requireNumber(table, "y", method),
            requireNumber(table, "width", method),
            requireNumber(table, "height", method)
        );
    }

    inline sol::table pushPoint(sol::this_state state, cocos2d::CCPoint const& point) {
        sol::state_view lua(state);
        auto table = lua.create_table();
        table["x"] = point.x;
        table["y"] = point.y;
        return table;
    }

    inline sol::table pushSize(sol::this_state state, cocos2d::CCSize const& size) {
        sol::state_view lua(state);
        auto table = lua.create_table();
        table["width"] = size.width;
        table["height"] = size.height;
        return table;
    }

    inline cocos2d::ccColor3B toColor3B(sol::table const& table, char const* method) {
        auto r = static_cast<unsigned char>(requireNumber(table, "r", method));
        auto g = static_cast<unsigned char>(requireNumber(table, "g", method));
        auto b = static_cast<unsigned char>(requireNumber(table, "b", method));
        return {r, g, b};
    }
}
