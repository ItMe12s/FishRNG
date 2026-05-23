---@diagnostic disable-next-line: undefined-field
local geode = _G.geode
local cocos = geode.cocos2d
local actions = cocos.actions

print("FishRNG bootstrap loaded")

-- Testing demo, I gotta figure it out.
local overlay = geode.OverlayManager.get()
local director = cocos.CCDirector.sharedDirector()
local win_size = director:getWinSize()

local layer = cocos.CCLayer.create()
local sprite = cocos.CCSprite.createWithSpriteFrameName("GJ_likeBtn_001.png")
local label = cocos.CCLabelBMFont.create("FishRNG", "bigFont.fnt")

sprite:setScale(0.55)
sprite:setPosition(win_size.width - 32, 34)

label:setScale(0.25)
label:setPosition(win_size.width - 32, 12)
label:setColor({ r = 255, g = 230, b = 120 })

layer:addChild(sprite)
layer:addChild(label)
overlay:addChild(layer)

local bob_up = actions.CCMoveBy.create(0.65, 0, 5)
local bob_down = actions.CCMoveBy.create(0.65, 0, -5)
local bob = actions.CCSequence.create({ bob_up, bob_down })
sprite:runAction(actions.CCRepeatForever.create(bob))
