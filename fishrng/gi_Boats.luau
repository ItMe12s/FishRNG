local cocos = geode.cocos2d

type BoatInfo = {
    texture: string,
}

local boatData: { [string]: BoatInfo } = {
    ["Wooden"] = {
        texture = "boatWooden.png",
    },
}

local Boats = {}

function Boats.getInfo(id: string): BoatInfo?
    return boatData[id]
end

function Boats.spriteFromID(id: string): CCSprite?
    local info = boatData[id]
    if not info then return nil end
    local sprite = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName(info.texture))
    sprite:setAliasTexParameters()
    sprite:setScale(1.25)
    return sprite
end

return Boats
