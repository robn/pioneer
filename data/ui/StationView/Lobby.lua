-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Translate = import("Translate")
local Engine = import("Engine")
local Game = import("Game")
local Rand = import("Rand")
local Character = import("Character")

local InfoFace = import("ui/InfoFace")

local ui = Engine.ui
local t = Translate:GetTranslator()

local lobby = function (tab)
	local station = Game.player:GetDockedWith()

	local rand = Rand.New(station.seed)
	local face = InfoFace.New(Character.New({ title = "Station manager" }, rand))

	return
		ui:Grid(2,1)
			:SetColumn(0, {
				ui:VBox(10)
					:PackEnd(ui:Label(station.label):SetFont("HEADING_LARGE"))
			})
			:SetColumn(1, {
				face.widget
			})
end

return lobby
