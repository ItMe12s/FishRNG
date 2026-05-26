local cocos = geode.cocos2d
local gd = geode.gd

local Player = require("./Player")

local function init()
    local director = cocos.CCDirector.sharedDirector()
    local winWidth = director:getWinSize().width

    local player = Player.create()
    player:setPosition({ x = winWidth - 20, y = 20 })
    
    local om = gd.OverlayManager.get()
    om:addChild(player)
end

local mlHook = geode.hook("geode.gd.MenuLayer:init/0", function(self)
    init()
end)
print("FishRNG loaded:", mlHook:isEnabled())