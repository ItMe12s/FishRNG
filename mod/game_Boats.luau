local cocos = geode.cocos2d

local Boats = {}

type BoatData = {
    texture: string,
    description: string,
    cost: number,
    maxWeight: number,
}

local boatData: {[string]: BoatData} = {
    ["Wooden"] = {
        texture = "imes.fishrng/boatWooden.png",
        description = "A wooden boat. Starter boat.",
        cost = 0,
        -- Custom data
        maxWeight = 5,
    }
}

function Boats.getData(boatName: string): BoatData
    if not boatData[boatName] then error("boat not found") end
    return boatData[boatName]
end

function Boats.getSprite(boatName: string): CCSprite?
    local sprite: CCSprite? = cocos.CCSprite.createWithSpriteFrameName(Boats.getData(boatName).texture)
    if not sprite then return end
    sprite:setZOrder(1)
    sprite:setScale(1.25)
    return sprite
end

return Boats