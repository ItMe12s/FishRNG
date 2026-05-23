local cocos = geode.cocos2d

--[[
id = {
    texture,
}
]]
local Boats = {
    ["Wooden"] = {
        "boatWooden.png",
    },
}

function Boats.getInfo(id: string)
    return Boats[id]
end

function Boats.spriteFromID(id: string)
    if not Boats[id] then return nil end
    local sprite = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName(Boats[id][1]))
    sprite:setAliasTexParameters()
    sprite:setScale(1.25)
    return sprite
end

return Boats
