local cocos = geode.cocos2d
local gd = geode.gd

local Player = {}

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

    return root
end

return Player