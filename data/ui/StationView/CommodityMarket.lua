-- Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Translate = import("Translate")
local Engine = import("Engine")

local ui = Engine.ui
local t = Translate:GetTranslator()

local commodityMarket = function (args)
    return ui:Label("commodity market")
end

return commodityMarket
