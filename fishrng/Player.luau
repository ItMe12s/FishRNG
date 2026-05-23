local cocos = geode.cocos2d

local FishingRods = require("./gi_FishingRods")
local Boats = require("./gi_Boats")

local Player = {}

Player.RodID = "Stick"
Player.RodSprite = nil :: CCSprite?
Player.BoatID = "Wooden"
Player.BoatSprite = nil :: CCSprite?

function Player.getWaterWaveRotation(): number
    local t = os.clock() * 0.2
    return math.noise(t) * 15
end

function Player.refreshRod(playerSprite: CCSprite): ()
    if Player.RodSprite then
        local parent = Player.RodSprite:getParent()
        if parent then
            parent:removeChild(Player.RodSprite, true)
        end
        Player.RodSprite = nil
    end

    local rodInfo = FishingRods.getInfo(Player.RodID)
    if not rodInfo then return end

    local rod = FishingRods.spriteFromID(Player.RodID)
    if not rod then return end

    rod:setAnchorPoint(0.5, 0)
    rod:setPosition(rodInfo.offsetX, rodInfo.offsetY)
    rod:setRotation(rodInfo.rotation)

    playerSprite:addChild(rod, 0)
    Player.RodSprite = rod
end

function Player.refreshBoat(rootSprite: CCSprite): ()
    if Player.BoatSprite then
        local parent = Player.BoatSprite:getParent()
        if parent then
            parent:removeChild(Player.BoatSprite, true)
        end
        Player.BoatSprite = nil
    end

    local boatInfo = Boats.getInfo(Player.BoatID)
    if not boatInfo then return end

    local boat = Boats.spriteFromID(Player.BoatID)
    if not boat then return end

    rootSprite:addChild(boat, 1)
    Player.BoatSprite = boat
end

function Player.create(): CCSprite
    local rootSprite = cocos.CCSprite.create()

    Player.refreshBoat(rootSprite)

    local gm = geode.GameManager.get()
    local icon_id = gm:getPlayerFrame()

    local playerSprite = geode.SimplePlayer.create(icon_id)
    playerSprite:updatePlayerFrame(icon_id, geode.IconType.Cube)
    playerSprite:setColor(gm:colorForIdx(gm:getPlayerColor()))
    playerSprite:setSecondColor(gm:colorForIdx(gm:getPlayerColor2()))
    if gm:getPlayerGlow() then
        playerSprite:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
    end
    playerSprite:setPosition(0, 20)
    rootSprite:addChild(playerSprite, 0)

    Player.refreshRod(playerSprite)

    return rootSprite
end

return Player
