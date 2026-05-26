local cocos = geode.cocos2d
local gd = geode.gd

local Player = require("./Player")

local function init()
    local director = cocos.CCDirector.sharedDirector()
    local winWidth = director:getWinSize().width

    local player = Player.create()
    player:setPosition(winWidth / 2, 20)
    
    local om = gd.OverlayManager.get()
    om:addChild(player)
end

local omHook = geode.hook("geode.gd.MenuLayer:init/0", function(self)
    init()
end)
print("FishRNG loaded:", omHook:isEnabled())