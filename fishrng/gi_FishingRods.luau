local cocos = geode.cocos2d

--[[
id = {
    texture,
    x, y, rotation offsets,
    description
}
]]
local FishingRods = {
    ["Stick"] = {
        "rodStick.png",
        -15, 0, -45,
        "Starter rod. Genuinely a stick."
    },
}

function FishingRods.getInfo(id: string)
    return FishingRods[id]
end

function FishingRods.spriteFromID(id: string)
    if not FishingRods[id] then return nil end
    local sprite = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName(FishingRods[id][1]))
    sprite:setAliasTexParameters()
    sprite:setScale(1)
    return sprite
end

return FishingRods
