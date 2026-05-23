---@diagnostic disable-next-line: undefined-field
local geode = _G.geode
local cocos = geode.cocos2d
local actions = cocos.actions

local Player = require("./Player")

local overlay = geode.OverlayManager.get()
local director = cocos.CCDirector.sharedDirector()
local win_size = director:getWinSize()

local PlayerBoat = Player.create()
local layer = geode.TouchLayer.create()

layer:addChild(PlayerBoat)
overlay:addChild(layer)
layer:addTapTarget(PlayerBoat, function(tap)
    -- print(("tapped at %.1f %.1f"):format(tap.x, tap.y))
end)

PlayerBoat:setPosition(win_size.width - 50, 0)

local bob_up = actions.CCEaseSineInOut.create(actions.CCMoveBy.create(2, 0, 3))
local bob_down = actions.CCEaseSineInOut.create(actions.CCMoveBy.create(2, 0, -3))
local bob = actions.CCSequence.create({ bob_up, bob_down })
PlayerBoat:runAction(actions.CCRepeatForever.create(bob))

cocos.scheduler.schedule(function()
    PlayerBoat:setRotation(Player.getWaterWaveRotation())
end, 0.05)

print("FishRNG bootstrap loaded")