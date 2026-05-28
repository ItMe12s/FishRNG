local cocos = geode.cocos2d
local gd = geode.gd

local Player = require("./game_Player")

local alreadyInit = false

local function init()
    local director = cocos.CCDirector.sharedDirector()
    local winWidth = director:getWinSize().width

    local playerRoot = Player.create()
    playerRoot:setPosition({ x = winWidth - 50, y = 0 })

    local om = gd.OverlayManager.get()
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
