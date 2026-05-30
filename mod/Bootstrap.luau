local cocos = geode.cocos2d
local gd = geode.gd

local Player = require("./game_Player")

local alreadyInit = false

local function init()
    local director: CCDirector? = cocos.CCDirector.sharedDirector()
    if not director then return end
    local winWidth: number = director:getWinSize().width

    local playerRoot: CCSprite? = Player.create()
    if not playerRoot then return end
    playerRoot:setPosition({ x = winWidth - 50, y = 0 })

    local om: OverlayManager? = gd.OverlayManager.get()
    if not om then return end
    om:addChild(playerRoot)

    alreadyInit = true
end

local mlHook = geode.hook("geode.gd.MenuLayer:init/0", {
    after = function(self, result)
        if not alreadyInit then
            init()
        end
        return result
    end,
})
print("FishRNG loaded:", mlHook:isEnabled())
