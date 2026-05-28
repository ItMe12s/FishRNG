local cocos = geode.cocos2d
local gd = geode.gd

local Boats = require("./game_Boats")

local Player = {}

Player.Data = {
    boat = "Wooden",
}
local boatSprite = nil

function Player:refreshBoat(root)
    if boatSprite then
        boatSprite:removeFromParentAndCleanup()
    end
    boatSprite = Boats.getSprite(Player.Data.boat)
    root:addChild(boatSprite)
end

function Player.create()
    local root = cocos.CCSprite.create()

    local gm = gd.GameManager.get()
    local iconId = gm:getPlayerFrame()

    local sprite = gd.SimplePlayer.create(iconId)
    sprite:updatePlayerFrame(iconId, 0)
    sprite:setColors(gm:colorForIdx(gm:getPlayerColor()), gm:colorForIdx(gm:getPlayerColor2()))
    if gm:getPlayerGlow() then
        sprite:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
    end
    sprite:setPosition({ x = 0, y = 20 })
    root:addChild(sprite)

    Player:refreshBoat(root)

    return root
end

return Player
