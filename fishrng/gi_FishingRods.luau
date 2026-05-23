local cocos = geode.cocos2d

type FishingRodInfo = {
    texture: string,
    description: string,
    cost: number,
    -- Custom data
    offsetX: number, offsetY: number, rotation: number,
    luckBoost: number,
}

local rodData: { [string]: FishingRodInfo } = {
    ["Stick"] = {
        texture = "rodStick.png",
        description = "Starter rod. Genuinely a stick.",
        cost = 0,
        offsetX = -15, offsetY = 0, rotation = -45,
        luckBoost = 0,
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
