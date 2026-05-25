local cocos = geode.cocos2d

local Player = require("./Player")

geode.hook("geode.gd.MenuLayer:init/0", function(self)
    local director = cocos.CCDirector.sharedDirector()
    if not director then return end

    local winSize = director:getWinSize()

    local overlay = director:getNotificationNode()
    if not overlay then return end

    local playerRoot = Player.create()
    if not playerRoot then return end
    overlay:addChild(playerRoot)
    playerRoot:setPosition({ x = winSize.width - 50, y = 0 })

    local moveUp = cocos.CCMoveBy.create(2, { x = 0, y = 3 })
    local moveDown = cocos.CCMoveBy.create(2, { x = 0, y = -3 })
    if not moveUp or not moveDown then return end
    
    local bobUp = cocos.CCEaseSineInOut.create(moveUp)
    local bobDown = cocos.CCEaseSineInOut.create(moveDown)
    if not bobUp or not bobDown then return end

    local bob = cocos.CCSequence.createWithTwoActions(bobUp, bobDown)
    if not bob then return end
    
    local repeatBob = cocos.CCRepeatForever.create(bob)
    if not repeatBob then return end

    playerRoot:runAction(repeatBob)

    local scheduler = director:getScheduler()
    if not scheduler then return end
    
    (scheduler :: any):scheduleScriptFunc(function(_dt: number)
        playerRoot:setRotation(Player.getWaterWaveRotation())
    end, 0.01, false)
end)

print("FishRNG loaded")