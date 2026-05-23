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
player:setScale(0.55)
player:setPosition(win_size.width - 32, 40)

local label = cocos.CCLabelBMFont.create("FishRNG", "bigFont.fnt")
label:setScale(0.25)
label:setPosition(win_size.width - 32, 14)
label:setColor({ r = 255, g = 230, b = 120 })

layer:addChild(player)
layer:addChild(label)
overlay:addChild(layer)

local bob_up = actions.CCMoveBy.create(0.65, 0, 5)
local bob_down = actions.CCMoveBy.create(0.65, 0, -5)
local bob = actions.CCSequence.create({ bob_up, bob_down })
player:runAction(actions.CCRepeatForever.create(bob))
