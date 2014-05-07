-- Copyright © 2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local utils = import("utils")
local Game = import_core("Game")
local Serializer = import("Serializer")
local Lang = import("Lang")

local l = Lang.GetResource("core")

local EquipType = utils.inherits(nil, "EquipType")

function EquipType.New (specs)
	local obj = {}
	for i,v in pairs(specs) do
		obj[i] = v
	end
	setmetatable(obj, EquipType.meta)
	if type(obj.slots) ~= "table" then
		obj.slots = {obj.slots}
	end
	return obj
end

function EquipType:GetDefaultSlot(ship)
	return self.slots[1]
end

function EquipType:IsValidSlot(ship, slot)
	for _, s in ipairs(self.slots) do
		if s == slot then
			return true
		end
	end
	return false
end

function EquipType:GetName()
	return l[self.l10n_key] or ""
end

function EquipType:GetDescription()
	return l[self.l10n_key.."_DESCRIPTION"] or ""
end

function __ApplyCapabilities(ship, capabilities, num, factor)
	if num <= 0 then return 0 end
	local factor = factor or 1
	for k,v in pairs(capabilities) do
		local full_name = k.."_cap"
		local prev = (ship:hasprop(full_name) and ship[full_name]) or 0
		ship:setprop(full_name, (factor*v)+prev)
	end
	return num
end

function EquipType:Install(ship, num, slot)
	return __ApplyCapabilities(ship, self.capabilities, num, 1)
end

function EquipType:Uninstall(ship, num, slot)
	return __ApplyCapabilities(ship, self.capabilities, num, -1)
end

-- Base type for weapons
LaserType = utils.inherits(EquipType, "LaserType")
function LaserType:Install(ship, num, slot)
	if __ApplyCapabilities(ship, self.capabilities, 1, 1) < 1 then return 0 end
    local prefix = slot..'_'
    for k,v in pairs(self.laser_stats) do
        ship:setprop(prefix..k, v)
    end
    return 1
end

function LaserType:Uninstall(ship, num, slot)
	if __ApplyCapabilities(ship, self.capabilities, 1, -1) < 1 then return 0 end
    local prefix = "front_"
    if slot == "laser_rear" then prefix = "_rear" end
    for k,v in pairs(self.laser_stats) do
        ship:setprop(prefix..k, nil)
    end
end

-- Single drive type, no support for slave drives.
HyperdriveType = utils.inherits(EquipType, "HyperdriveType")

HyperdriveType.GetMaximumRange = function (self, ship)
	return 0.625*ship.totalMass*(self.capabilities.hyperclass ^ 2)
end

-- range_max is as usual optional
HyperdriveType.GetDuration = function (self, ship, distance, range_max)
	local range_max = range_max or self:GetMaximumRange(ship)
	local hyperclass = self.capabilities.hyperclass
	return 0.45*distance^2/(range_max*hyperclass) * (3600*24*math.sqrt(ship.totalMass))
end

-- range_max is optional, distance defaults to the maximal range.
HyperdriveType.GetFuelUse = function (self, ship, distance, range_max)
	local range_max = range_max or self:GetMaximumRange(ship)
	local distance = distance or range_max
	local hyperclass_squared = self.capabilities.hyperclass^2
	return math.clamp(math.ceil(hyperclass_squared*distance / range_max), 1, hyperclass_squared);
end

-- returns nil if the destination isn't reachable, distance, fuel and duration if it is
HyperdriveType.CheckDestination = function (self, ship, destination)
	if not Game.system or ship:GetEquip('engine', 1) ~= self or destination:GetStarSystem() == Game.system then
		return nil
	end
	local distance = Game.system:DistanceTo(destination)
	local max_range = self:GetMaximumRange(ship) -- takes fuel into account
	if distance > max_range then
		return distance
	end
	local fuel = self:GetFuelUse(ship, distance, range_max) -- specify range_max to avoid unnecessary recomputing.

	local duration = self:GetDuration(ship, distance, range_max) -- same as above
	return distance, fuel, duration
end

-- Give the range for the given remaining fuel
-- If the fuel isn't specified, it takes the current value.
HyperdriveType.GetRange = function (self, ship, remaining_fuel)
	local range_max = self:GetMaximumRange(ship)
	local fuel_max = fuel_max or self:GetFuelUse(ship, range_max, range_max)
	local remaining_fuel = remaining_fuel or ship:CountEquip(self.fuel)

	if fuel_max <= remaining_fuel then
		return range_max
	end
	local range = range_max*remaining_fuel/fuel_max

	while range > 0 and self:GetFuelUse(ship, range, range_max) > remaining_fuel do
		range = range - 0.05
	end

	-- range is never negative
	range = math.max(range, 0)
	return range
end

HyperdriveType.HyperjumpTo = function (self, ship, destination)
	-- First off, check that this is the primary engine.
	local engines = ship:GetEquip('engine')
	local primary_index = 0
	for i,e in ipairs(engines) do
		if e == self then
			primary_index = i
			break
		end
	end
	if primary_index == 0 then
		-- wrong ship
		return "WRONG_SHIP"
	end
	local distance, fuel_use, duration = self:CheckDestination(ship, destination)
	return ship:InitiateHyperjumpTo(destination, self.capabilities.hyperclass, duration), fuel_use, duration
end

cargo = {
	hydrogen = EquipType.New({
		l10n_key = 'HYDROGEN', slots="cargo", price=100,
		capabilities={mass=1}, economy_type="mining"
	}),
	liquid_oxygen = EquipType.New({
		l10n_key="LIQUID_OXYGEN", slots="cargo", price=150,
		capabilities={mass=1}, economy_type="mining"
	}),
	water = EquipType.New({
		l10n_key="WATER", slots="cargo", price=120,
		capabilities={mass=1}, economy_type="mining"
	}),
	carbon_ore = EquipType.New({
		l10n_key="CARBON_ORE",
		slots="cargo", price=500, capabilities={mass=1},
		economy_type="mining"
	}),
	metal_ore = EquipType.New({
		l10n_key="METAL_ORE", slots="cargo", price=300,
		capabilities={mass=1}, economy_type="mining"
	}),
	metal_alloys = EquipType.New({
		l10n_key="METAL_ALLOYS", slots="cargo", price=800,
		capabilities={mass=1}, economy_type="industry"
	}),
	precious_metals = EquipType.New({
		l10n_key="PRECIOUS_METALS", slots="cargo", price=18000,
		capabilities={mass=1}, economy_type="industry"
	}),
	plastics = EquipType.New({
		l10n_key="PLASTICS", slots="cargo", price=1200,
		capabilities={mass=1}, economy_type="industry"
	}),
	fruit_and_veg = EquipType.New({
		l10n_key="FRUIT_AND_VEG", slots="cargo", price=1200,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	animal_meat = EquipType.New({
		l10n_key="ANIMAL_MEAT", slots="cargo", price=1800,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	live_animals = EquipType.New({
		l10n_key="LIVE_ANIMALS", slots="cargo", price=3200,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	liquor = EquipType.New({
		l10n_key="LIQUOR", slots="cargo", price=800,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	grain = EquipType.New({
		l10n_key="GRAIN", slots="cargo", price=1000,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	slaves = EquipType.New({
		l10n_key="SLAVES", slots="cargo", price=23200,
		capabilities={mass=1}, economy_type="agriculture"
	}),
	textiles = EquipType.New({
		l10n_key="TEXTILES", slots="cargo", price=850,
		capabilities={mass=1}, economy_type="industry"
	}),
	fertilizer = EquipType.New({
		l10n_key="FERTILIZER", slots="cargo", price=400,
		capabilities={mass=1}, economy_type="industry"
	}),
	medicines = EquipType.New({
		l10n_key="MEDICINES", slots="cargo", price=2200,
		capabilities={mass=1}, economy_type="industry"
	}),
	consumer_goods = EquipType.New({
		l10n_key="CONSUMER_GOODS", slots="cargo", price=14000,
		capabilities={mass=1}, economy_type="industry"
	}),
	computers = EquipType.New({
		l10n_key="COMPUTERS", slots="cargo", price=8000,
		capabilities={mass=1}, economy_type="industry"
	}),
	rubbish = EquipType.New({
		l10n_key="RUBBISH", slots="cargo", price=-10,
		capabilities={mass=1}, economy_type="industry"
	}),
	radioactives = EquipType.New({
		l10n_key="RADIOACTIVES", slots="cargo", price=-35,
		capabilities={mass=1}, economy_type="industry"
	}),
	narcotics = EquipType.New({
		l10n_key="NARCOTICS", slots="cargo", price=15700,
		capabilities={mass=1}, economy_type="industry"
	}),
	nerve_gas = EquipType.New({
		l10n_key="NERVE_GAS", slots="cargo", price=26500,
		capabilities={mass=1}, economy_type="industry"
	}),
	military_fuel = EquipType.New({
		l10n_key="MILITARY_FUEL", slots="cargo", price=6000,
		capabilities={mass=1}, economy_type="industry"
	}),
	robots = EquipType.New({
		l10n_key="ROBOTS", slots="cargo", price=6300,
		capabilities={mass=1}, economy_type="industry"
	}),
	hand_weapons = EquipType.New({
		l10n_key="HAND_WEAPONS", slots="cargo", price=12400,
		capabilities={mass=1}, economy_type="industry"
	}),
	air_processors = EquipType.New({
		l10n_key="AIR_PROCESSORS", slots="cargo", price=2000,
		capabilities={mass=1}, economy_type="industry"
	}),
	farm_machinery = EquipType.New({
		l10n_key="FARM_MACHINERY", slots="cargo", price=1100,
		capabilities={mass=1}, economy_type="industry"
	}),
	mining_machinery = EquipType.New({
		l10n_key="MINING_MACHINERY", slots="cargo", price=1200,
		capabilities={mass=1}, economy_type="industry"
	}),
	battle_weapons = EquipType.New({
		l10n_key="BATTLE_WEAPONS", slots="cargo", price=22000,
		capabilities={mass=1}, economy_type="industry"
	}),
	industrial_machinery = EquipType.New({
		l10n_key="INDUSTRIAL_MACHINERY", slots="cargo", price=1300,
		capabilities={mass=1}, economy_type="industry"
	}),
}

cargo.liquid_oxygen.requirements = { cargo.water, cargo.industrial_machinery }
cargo.battle_weapons.requirements = { cargo.metal_alloys, cargo.industrial_machinery }
cargo.farm_machinery.requirements = { cargo.metal_alloys, cargo.robots }
cargo.mining_machinery.requirements = { cargo.metal_alloys, cargo.robots }
cargo.industrial_machinery.requirements = { cargo.metal_alloys, cargo.robots }
cargo.air_processors.requirements = { cargo.plastics, cargo.industrial_machinery }
cargo.robots.requirements = { cargo.plastics, cargo.computers }
cargo.hand_weapons.requirements = { cargo.computers }
cargo.computers.requirements = { cargo.precious_metals, cargo.industrial_machinery }
cargo.metal_ore.requirements = { cargo.mining_machinery }
cargo.carbon_ore.requirements = { cargo.mining_machinery }
cargo.metal_alloys.requirements = { cargo.mining_machinery }
cargo.precious_metals.requirements = { cargo.mining_machinery }
cargo.water.requirements = { cargo.mining_machinery }
cargo.plastics.requirements = { cargo.carbon_ore, cargo.industrial_machinery }
cargo.fruit_and_veg.requirements = { cargo.farm_machinery, cargo.fertilizer }
cargo.animal_meat.requirements = { cargo.farm_machinery, cargo.fertilizer }
cargo.live_animals.requirements = { cargo.farm_machinery, cargo.fertilizer }
cargo.liquor.requirements = { cargo.farm_machinery, cargo.fertilizer }
cargo.grain.requirements = { cargo.farm_machinery, cargo.fertilizer }
cargo.textiles.requirements = { cargo.plastics }
cargo.military_fuel.requirements = { cargo.hydrogen }
cargo.fertilizer.requirements = { cargo.carbon_ore }
cargo.medicines.requirements = { cargo.computers, cargo.carbon_ore }
cargo.consumer_goods.requirements = { cargo.plastics, cargo.textiles }

local misc = {}
misc.missile_unguided = EquipType.New({
	l10n_key="MISSILE_UNGUIDED", slots="missile", price=3000,
	capabilities={mass=1, missile=1}
})
misc.missile_guided = EquipType.New({
	l10n_key="MISSILE_GUIDED", slots="missile", price=5000,
	capabilities={mass=1}
})
misc.missile_smart = EquipType.New({
	l10n_key="MISSILE_SMART", slots="missile", price=9500,
	capabilities={mass=1}
})
misc.missile_naval = EquipType.New({
	l10n_key="MISSILE_NAVAL", slots="missile", price=16000,
	capabilities={mass=1}
})
misc.atmospheric_shielding = EquipType.New({
	l10n_key="ATMOSPHERIC_SHIELDING", slots="atmo_shield", price=20000,
	capabilities={mass=1, atmo_shield=1}
})
misc.ecm_basic = EquipType.New({
	l10n_key="ECM_BASIC", slots="ecm", price=600000,
	capabilities={mass=2, ecm_power=2, ecm_recharge=5}
})
misc.ecm_advanced = EquipType.New({
	l10n_key="ECM_ADVANCED", slots="ecm", price=1520000,
	capabilities={mass=2, ecm_power=3, ecm_recharge=5}
})
misc.scanner = EquipType.New({
	l10n_key="SCANNER", slots="scanner", price=68000,
	capabilities={mass=1, scanner=1}
})
misc.cabin = EquipType.New({
	l10n_key="UNOCCUPIED_CABIN", slots="cabin", price=135000,
	capabilities={mass=1, cabin=1}
})
misc.cabin_occupied = EquipType.New({
	l10n_key="PASSENGER_CABIN", slots="cabin", price=0,
	capabilities={mass=1}
})
misc.shield_generator = EquipType.New({
	l10n_key="SHIELD_GENERATOR", slots="shield", price=250000,
	capabilities={mass=4, shield=1}
})
misc.laser_cooling_booster = EquipType.New({
	l10n_key="LASER_COOLING_BOOSTER", slots="laser_cooler", price=38000,
	capabilities={mass=1, laser_cooler=2}
})
misc.cargo_life_support = EquipType.New({
	l10n_key="CARGO_LIFE_SUPPORT", slots="cargo_life_support", price=70000,
	capabilities={mass=1, cargo_life_support=1}
})
misc.autopilot = EquipType.New({
	l10n_key="AUTOPILOT", slots="autopilot", price=140000,
	capabilities={mass=1, set_speed=1, autopilot=1}
})
misc.radar_mapper = EquipType.New({
	l10n_key="RADAR_MAPPER", slots="radar", price=90000,
	capabilities={mass=1, radar_mapper=1}
})
misc.fuel_scoop = EquipType.New({
	l10n_key="FUEL_SCOOP", slots="fuel_scoop", price=350000,
	capabilities={mass=6, fuel_scoop=1}
})
misc.cargo_scoop = EquipType.New({
	l10n_key="CARGO_SCOOP", slots="cargo_scoop", price=390000,
	capabilities={mass=7, cargo_scoop=1}
})
misc.hypercloud_analyzer = EquipType.New({
	l10n_key="HYPERCLOUD_ANALYZER", slots="hypercloud", price=150000,
	capabilities={mass=1, hypercloud_analyzer=1}
})
misc.shield_energy_booster = EquipType.New({
	l10n_key="SHIELD_ENERGY_BOOSTER", slots="energy_booster", price=1000000,
	capabilities={mass=8, shield_energy_booster=1}
})
misc.hull_autorepair = EquipType.New({
	l10n_key="HULL_AUTOREPAIR", slots="hull_autorepair", price=1600000,
	capabilities={mass=40, hull_autorepair=1}
})

local hyperspace = {}
hyperspace.hyperdrive_1 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS1", fuel=cargo.hydrogen, slots="engine",
	price=70000, capabilities={mass=4, hyperclass=1},
})
hyperspace.hyperdrive_2 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS2", fuel=cargo.hydrogen, slots="engine",
	price=130000, capabilities={mass=10, hyperclass=2}
})
hyperspace.hyperdrive_3 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS3", fuel=cargo.hydrogen, slots="engine",
	price=250000, capabilities={mass=20, hyperclass=3}
})
hyperspace.hyperdrive_4 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS4", fuel=cargo.hydrogen, slots="engine",
	price=500000, capabilities={mass=40, hyperclass=4}
})
hyperspace.hyperdrive_5 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS5", fuel=cargo.hydrogen, slots="engine",
	price=1000000, capabilities={mass=120, hyperclass=5}
})
hyperspace.hyperdrive_6 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS6", fuel=cargo.hydrogen, slots="engine",
	price=2000000, capabilities={mass=225, hyperclass=6}
})
hyperspace.hyperdrive_7 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS7", fuel=cargo.hydrogen, slots="engine",
	price=3000000, capabilities={mass=400, hyperclass=7}
})
hyperspace.hyperdrive_8 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS8", fuel=cargo.hydrogen, slots="engine",
	price=6000000, capabilities={mass=580, hyperclass=8}
})
hyperspace.hyperdrive_9 = HyperdriveType.New({
	l10n_key="DRIVE_CLASS9", fuel=cargo.hydrogen, slots="engine",
	price=12000000, capabilities={mass=740, hyperclass=9}
})
hyperspace.hyperdrive_mil1 = HyperdriveType.New({
	l10n_key="DRIVE_MIL1", fuel=cargo.military_fuel, slots="engine",
	price=2300000, capabilities={mass=3, hyperclass=1}
})
hyperspace.hyperdrive_mil2 = HyperdriveType.New({
	l10n_key="DRIVE_MIL2", fuel=cargo.military_fuel, slots="engine",
	price=4700000, capabilities={mass=8, hyperclass=2}
})
hyperspace.hyperdrive_mil3 = HyperdriveType.New({
	l10n_key="DRIVE_MIL3", fuel=cargo.military_fuel, slots="engine",
	price=8500000, capabilities={mass=16, hyperclass=3}
})
hyperspace.hyperdrive_mil4 = HyperdriveType.New({
	l10n_key="DRIVE_MIL4", fuel=cargo.military_fuel, slots="engine",
	price=21400000, capabilities={mass=30, hyperclass=4}
})

local laser = {LaserType = LaserType}
laser.pulsecannon_1mw = LaserType.New({
	l10n_key="PULSECANNON_1MW", price=60000, capabilities={mass=1},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=1000, rechargeTime=0.25, length=30,
		width=5, dual=0, mining=0, rgba_r = 255, rgba_g = 51, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_dual_1mw = LaserType.New({
	l10n_key="PULSECANNON_DUAL_1MW", price=110000, capabilities={mass=4},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=1000, rechargeTime=0.25, length=30,
		width=5, dual=1, mining=0, rgba_r = 255, rgba_g = 51, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_2mw = LaserType.New({
	l10n_key="PULSECANNON_2MW", price=100000, capabilities={mass=3},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=2000, rechargeTime=0.25, length=30,
		width=5, dual=0, mining=0, rgba_r = 255, rgba_g = 127, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_rapid_2mw = LaserType.New({
	l10n_key="PULSECANNON_RAPID_2MW", price=180000, capabilities={mass=7},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=2000, rechargeTime=0.13, length=30,
		width=5, dual=0, mining=0, rgba_r = 255, rgba_g = 127, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_4mw = LaserType.New({
	l10n_key="PULSECANNON_4MW", price=220000, capabilities={mass=10},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=4000, rechargeTime=0.25, length=30,
		width=5, dual=0, mining=0, rgba_r = 255, rgba_g = 255, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_10mw = LaserType.New({
	l10n_key="PULSECANNON_10MW", price=490000, capabilities={mass=30},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=10000, rechargeTime=0.25, length=30,
		width=5, dual=0, mining=0, rgba_r = 51, rgba_g = 255, rgba_b = 51, rgba_a = 255
	}
})
laser.pulsecannon_20mw = LaserType.New({
	l10n_key="PULSECANNON_20MW", price=1200000, capabilities={mass=65},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=20000, rechargeTime=0.25, length=30,
		width=5, dual=0, mining=0, rgba_r = 0.1, rgba_g = 51, rgba_b = 255, rgba_a = 255
	}
})
laser.miningcannon_17mw = LaserType.New({
	l10n_key="MININGCANNON_17MW", price=1060000, capabilities={mass=10},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=17000, rechargeTime=2, length=30,
		width=5, dual=0, mining=1, rgba_r = 51, rgba_g = 127, rgba_b = 0, rgba_a = 255
	}
})
laser.small_plasma_accelerator = LaserType.New({
	l10n_key="SMALL_PLASMA_ACCEL", price=12000000, capabilities={mass=22},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=50000, rechargeTime=0.3, length=42,
		width=7, dual=0, mining=0, rgba_r = 51, rgba_g = 255, rgba_b = 255, rgba_a = 255
	}
})
laser.large_plasma_accelerator = LaserType.New({
	l10n_key="LARGE_PLASMA_ACCEL", price=39000000, capabilities={mass=50},
	slots = {"laser_front", "laser_rear"}, laser_stats = {
		lifespan=8, speed=1000, damage=100000, rechargeTime=0.3, length=42,
		width=7, dual=0, mining=0, rgba_r = 127, rgba_g = 255, rgba_b = 255, rgba_a = 255
	}
})
local equipment = {
    cargo=cargo,
    laser=laser,
    hyperspace=hyperspace,
    misc=misc,
    LaserType=LaserType,
    HyperdriveType=HyperdriveType,
    EquipType=EquipType,
}

local serialize = function()
    local ret = {}
    for _,k in ipairs{"cargo","laser", "hyperspace", "misc"} do
        local tmp = {}
        for kk, vv in pairs(equipment[k]) do
            tmp[kk] = vv
        end
        ret[k] = tmp
    end
    return ret
end

local unserialize = function (data)
    for _,k in ipairs{"cargo","laser", "hyperspace", "misc"} do
        local tmp = equipment[k]
        for kk, vv in pairs(data[k]) do
            tmp[kk] = vv
        end
    end
end

Serializer:Register("Equipment", serialize, unserialize)
Serializer:RegisterClass("LaserType", LaserType)
Serializer:RegisterClass("EquipType", EquipType)
Serializer:RegisterClass("HyperdriveType", HyperdriveType)

return equipment
