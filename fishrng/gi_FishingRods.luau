local cocos = geode.cocos2d

type FishingRodInfo = {
    texture: string,
    offsetX: number,
    offsetY: number,
    rotation: number,
    description: string,
}

local rodData: { [string]: FishingRodInfo } = {
    ["Stick"] = {
        texture = "rodStick.png",
        offsetX = -15,
        offsetY = 0,
        rotation = -45,
        description = "Starter rod. Genuinely a stick.",
    },
}

local FishingRods = {}

function FishingRods.getInfo(id: string): FishingRodInfo?
    return rodData[id]
end

function FishingRods.spriteFromID(id: string): CCSprite?
    local info = rodData[id]
    if not info then return nil end
    local sprite = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName(info.texture))
    sprite:setAliasTexParameters()
    sprite:setScale(1)
    return sprite
end

return FishingRods
