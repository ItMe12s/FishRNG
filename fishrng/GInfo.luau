---@diagnostic disable-next-line: undefined-field
local geode = _G.geode
local cocos = geode.cocos2d

local GInfo = {}

--[[
id = {
    texture,
    x, y, rotation
}
]]
GInfo.FishingRodIDs = {
    ["Stick"] = {
        "rodStick.png",
        -15, 0, -45
    },
}

function GInfo.FishingRodSpriteFromID(id: string)
    if not GInfo.FishingRodIDs[id] then return nil end
    local rod = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName(GInfo.FishingRodIDs[id][1]))
    rod:setAliasTexParameters()
    rod:setScale(1)
    return rod
end

return GInfo