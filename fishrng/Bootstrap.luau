local cocos = geode.cocos2d
local actions = cocos.actions

local Player = require("./Player")

local overlay: OverlayManager = geode.OverlayManager.get()
local director: CCDirector = cocos.CCDirector.sharedDirector()
local win_width: number = director:getWinSize().width

local playerRoot: CCSprite = Player.create()
local layer: TouchLayer = geode.TouchLayer.create()

layer:addChild(playerRoot)
overlay:addChild(layer)
layer:addTapTarget(playerRoot, function(_tap: TapEvent)
    -- print(("tapped at %.1f %.1f"):format(tap.x, tap.y))
end)
playerRoot:setPosition(win_width - 50, 0)

local bob_up: CCActionInterval = actions.CCEaseSineInOut.create(actions.CCMoveBy.create(2, 0, 3))
local bob_down: CCActionInterval = actions.CCEaseSineInOut.create(actions.CCMoveBy.create(2, 0, -3))
local bob: CCActionInterval = actions.CCSequence.create({ bob_up, bob_down })
playerRoot:runAction(actions.CCRepeatForever.create(bob))

cocos.scheduler.schedule(function()
    playerRoot:setRotation(Player.getWaterWaveRotation())
end, 0.01)
