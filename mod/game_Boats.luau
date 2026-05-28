local cocos = geode.cocos2d

local Boats = {}

local boatData = {
    ["Wooden"] = {
        texture = "imes.fishrng/boatWooden.png",
        description = "A wooden boat. Starter boat.",
        cost = 0,
        -- Custom data
        maxWeight = 5,
    }
}

function Boats.getData(boatName: string)
    if not boatData[boatName] then error("boat not found") end
    return boatData[boatName]
end

function Boats.getSprite(boatName: string)
    local sprite = cocos.CCSprite.createWithSpriteFrameName(Boats.getData(boatName).texture)
    sprite:setZOrder(1)
    sprite:setScale(1.25)
    return sprite
end

return Boats