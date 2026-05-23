---@diagnostic disable-next-line: undefined-field
local geode = _G.geode
local cocos = geode.cocos2d

local Player = {}

function Player.create()
    local root = cocos.CCSprite.create()

    local boat = cocos.CCSprite.createWithSpriteFrameName(geode.expandSpriteName("playerBoat.png"))
    boat:setScale(3)
    root:addChild(boat)

    local gm = geode.GameManager.get()
    local icon_id = gm:getPlayerFrame()

    local plr = geode.SimplePlayer.create(icon_id)
    plr:updatePlayerFrame(icon_id, geode.IconType.Cube)
    plr:setColor(gm:colorForIdx(gm:getPlayerColor()))
    plr:setSecondColor(gm:colorForIdx(gm:getPlayerColor2()))
    if gm:getPlayerGlow() then
        plr:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
    end
    root:addChild(plr)

    return root
end

return Player
