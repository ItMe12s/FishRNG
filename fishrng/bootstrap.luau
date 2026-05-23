---@diagnostic disable-next-line: undefined-field
local geode = _G.geode
local cocos = geode.cocos2d
local actions = cocos.actions

print("FishRNG bootstrap loaded")

local overlay = geode.OverlayManager.get()
local director = cocos.CCDirector.sharedDirector()
local win_size = director:getWinSize()

local gm = geode.GameManager.get()
local icon_id = gm:getPlayerFrame()

local layer = cocos.CCLayer.create()

local player = geode.SimplePlayer.create(icon_id)
player:updatePlayerFrame(icon_id, geode.IconType.Cube)
player:setColor(gm:colorForIdx(gm:getPlayerColor()))
player:setSecondColor(gm:colorForIdx(gm:getPlayerColor2()))
if gm:getPlayerGlow() then
    player:setGlowOutline(gm:colorForIdx(gm:getPlayerGlowColor()))
end

player:setScale(1.25)
player:setPosition(win_size.width - 32, 40)

layer:addChild(player)
overlay:addChild(layer)

local bob_up = actions.CCMoveBy.create(0.65, 0, 5)
local bob_down = actions.CCMoveBy.create(0.65, 0, -5)
local bob = actions.CCSequence.create({ bob_up, bob_down })
player:runAction(actions.CCRepeatForever.create(bob))
