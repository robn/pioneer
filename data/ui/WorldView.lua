-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Engine = import("Engine")

local InfoPanel = import("InfoPanel")

local ui = Engine.ui

local widget

ui.templates.WorldView = function (args)
	return InfoPanel.Get()
end
