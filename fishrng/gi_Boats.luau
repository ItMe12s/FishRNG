local cocos = geode.cocos2d

type BoatInfo = {
	texture: string,
	description: string,
	cost: number,
	-- Custom data
	maxFish: number,
}

local boatData: { [string]: BoatInfo } = {
	["Wooden"] = {
		texture = "imes.fishrng/boatWooden.png",
		description = "Starter boat. A wooden one.",
		cost = 0,
		maxFish = 5,
	},
}

local Boats = {}

function Boats.getInfo(id: string): BoatInfo?
	return boatData[id]
end

function Boats.spriteFromID(id: string): CCSprite?
	local info = boatData[id]
	if not info then return nil end

	local sprite = cocos.CCSprite.createWithSpriteFrameName(info.texture)
	if not sprite then return nil end

	local texture = sprite:getTexture()
	if texture then
		texture:setAliasTexParameters()
	end

	sprite:setScale(1.25)
	return sprite
end

return Boats
