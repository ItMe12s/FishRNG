local cocos = geode.cocos2d
local gd = geode.gd

local Boats = require("./game_Boats")

local Player = {}

Player.Data = {
    boat = "Wooden",
}
local boatSprite: CCSprite? = nil

function Player:refreshBoat(root)
    if boatSprite then
        boatSprite:removeFromParentAndCleanup(true)
    end
    boatSprite = Boats.getSprite(Player.Data.boat)
    if not boatSprite then return end
    root:addChild(boatSprite)
end

function Player.create(): ()
    local root: CCSprite? = cocos.CCSprite.create()
    if not root then return end
    
    local gm: GameManager? = gd.GameManager.get()
    if not gm then return end
    local iconId: number = gm:getPlayerFrame()

    local sprite: SimplePlayer? = gd.SimplePlayer.create(iconId)
    if not sprite then return end

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
