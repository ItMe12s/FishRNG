local cocos = geode.cocos2d
local gd = geode.gd

local FishingRods = require("./gi_FishingRods")
local Boats = require("./gi_Boats")

local Player = {}

Player.RodID = "Stick"
Player.RodSprite = nil :: CCSprite?
Player.BoatID = "Wooden"
Player.BoatSprite = nil :: CCSprite?

function Player.getWaterWaveRotation(): number
	local t = os.clock() * 0.2
	return math.noise(t) * 15
end

function Player.refreshRod(playerSprite: CCSprite): ()
	if Player.RodSprite then
		local parent = Player.RodSprite:getParent()
		if parent then
			parent:removeChild(Player.RodSprite, true)
		end
		Player.RodSprite = nil
	end

	local rodInfo = FishingRods.getInfo(Player.RodID)
	if not rodInfo then return end

	local rod = FishingRods.spriteFromID(Player.RodID)
	if not rod then return end

	rod:setAnchorPoint({ x = 0.5, y = 0 })
	rod:setPosition({ x = rodInfo.offsetX, y = rodInfo.offsetY })
	rod:setRotation(rodInfo.rotation)

	playerSprite:addChild(rod, 0)
	Player.RodSprite = rod
end

function Player.refreshBoat(rootSprite: CCSprite): ()
	if Player.BoatSprite then
		local parent = Player.BoatSprite:getParent()
		if parent then
			parent:removeChild(Player.BoatSprite, true)
		end
		Player.BoatSprite = nil
	end

	local boatInfo = Boats.getInfo(Player.BoatID)
	if not boatInfo then return end

	local boat = Boats.spriteFromID(Player.BoatID)
	if not boat then return end

	rootSprite:addChild(boat, 1)
	Player.BoatSprite = boat
end

function Player.create(): CCSprite?
	local rootSprite = cocos.CCSprite.create()
	if not rootSprite then
		return nil
	end

	Player.refreshBoat(rootSprite)

	local gm = gd.GameManager.get()
	if not gm then
		return rootSprite
	end

	local icon_id = gm:getIntGameVariable("playerFrame")
	if icon_id <= 0 then
		icon_id = 1
	end

	local playerSprite = gd.SimplePlayer.create(icon_id)
	if not playerSprite then
		return rootSprite
	end

	local color1 = gm:getIntGameVariable("playerColor")
	local color2 = gm:getIntGameVariable("playerColor2")
	playerSprite:setColor(gm:colorForIdx(color1))
	playerSprite:setSecondColor(gm:colorForIdx(color2))
	if gm:getGameVariable("playerGlow") then
		local glowColor = gm:getIntGameVariable("playerColor3")
		if glowColor < 0 then
			glowColor = color2
		end
		playerSprite:enableCustomGlowColor(gm:colorForIdx(glowColor))
	end
	playerSprite:setPosition({ x = 0, y = 20 })
	rootSprite:addChild(playerSprite, 0)

	Player.refreshRod(playerSprite)

	return rootSprite
end

function Player.transitionIn(rootSprite: CCSprite): ()
	local moveIn = cocos.CCMoveBy.create(0, { x = 100, y = 0 })
	local moveOut = cocos.CCMoveBy.create(10, { x = -100, y = 0 })
	if not moveIn or not moveOut then return end

	local elastic = cocos.CCEaseElasticOut.create(moveOut)
	if not elastic then return end

	local action = cocos.CCSequence.createWithTwoActions(moveIn, elastic)
	if not action then return end

	rootSprite:runAction(action)
end

return Player
