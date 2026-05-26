local cocos = geode.cocos2d
local gd = geode.gd

local Player = {}

function Player.create()
    local root = cocos.CCSprite.create()

    local gm = gd.GameManager.get()
    local iconId = gm:getPlayerFrame()

    local sprite = gd.SimplePlayer.create(iconId)
    sprite:updatePlayerFrame(iconId, 1)
    sprite:setColor(gm:colorForIdx(gm:getPlayerColor()), gm:colorForIdx(gm:getPlayerColor2()))
    if gm:getPlayerGlow() then
        sprite:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
    end
    sprite:setPosition(0, 20)
    root:addCustomChild(sprite)

    return root
end

return Player