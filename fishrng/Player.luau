local cocos = geode.cocos2d

local GInfo = require("./GInfo")

local Player = {}

Player.FishingRodID = "Stick"
Player.FishingRodSprite = nil

function Player.getWaterWaveRotation()
    local t = os.clock() * 0.2
    return math.noise(t) * 15
end

function Player.refreshFishingRod(playerSprite)
    if Player.FishingRodSprite then
        local parent = Player.FishingRodSprite:getParent()
        if parent then
            parent:removeChild(Player.FishingRodSprite, true)
        end
        Player.FishingRodSprite = nil
    end

    local rodInfo = GInfo.FishingRodIDs[Player.FishingRodID]
    if not rodInfo then return end

    local rod = GInfo.FishingRodSpriteFromID(Player.FishingRodID)
    if not rod then return end

    rod:setAnchorPoint(0.5, 0)
    rod:setPosition(rodInfo[2], rodInfo[3])
    rod:setRotation(rodInfo[4])

    playerSprite:addChild(rod, 0)
    Player.FishingRodSprite = rod
end

function Player.create()
    local root = cocos.CCSprite.create()

    local boat = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName("playerBoat.png"))
    boat:setAliasTexParameters()
    boat:setScale(1.25)
    root:addChild(boat, 1)

    local gm = geode.GameManager.get()
    local icon_id = gm:getPlayerFrame()

    local plr = geode.SimplePlayer.create(icon_id)
    plr:updatePlayerFrame(icon_id, geode.IconType.Cube)
    plr:setColor(gm:colorForIdx(gm:getPlayerColor()))
    plr:setSecondColor(gm:colorForIdx(gm:getPlayerColor2()))
    if gm:getPlayerGlow() then
        plr:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
    end
    plr:setPosition(0, 20)
    root:addChild(plr, 0)

    Player.refreshFishingRod(plr)

    return root
end

return Player