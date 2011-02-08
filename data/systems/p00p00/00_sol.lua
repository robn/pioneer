earth_children = {
	{
		name = 'Shanghai',
		type = STARPORT_SURFACE,
		latitude = deg2rad(31),
		longitude = deg2rad(-121),
	}, {
		name = 'Mexico City',
		type = STARPORT_SURFACE,
		latitude = deg2rad(19),
		longitude = deg2rad(99),
	}, {
		name = 'London',
		type = STARPORT_SURFACE,
		latitude = deg2rad(51),
		longitude = 0,
	}, {
		name = 'Moscow',
		type = STARPORT_SURFACE,
		latitude = deg2rad(55),
		longitude = deg2rad(-37.5),
	}, {
		name = 'Brasilia',
		type = STARPORT_SURFACE,
		latitude = deg2rad(-15.5),
		longitude = deg2rad(48),
	}, {
		name = 'Los Angeles',
		type = STARPORT_SURFACE,
		latitude = deg2rad(34),
		longitude = deg2rad(118),
	}, {
		name = 'Gates Spaceport',
		type = STARPORT_ORBITAL,
		semi_major_axis = fixed(100,100000),
		rotation_period = fixed(1,24*60*3)
	}, {
		name = 'Moon',
		type = PLANET_DWARF,
		radius = fixed(273,1000),
		mass = fixed(12,1000),
		temp = 220,
		semi_major_axis = fixed(257,100000),
		eccentricity = fixed(549,10000),
		latitude = deg2rad(5.145),
		rotation_period = fixed(273,10),
		axial_tilt = fixed_deg2rad(668,100),
		children = { {
			name = 'Lunar City',
			type = STARPORT_SURFACE,
			latitude = deg2rad(19),
			longitude = deg2rad(99),
		} },
	},
}

mars_children = {
	{
		name = 'Cydonia',
		type = STARPORT_SURFACE,
		latitude = deg2rad(-29),
		longitude = deg2rad(124),
	}, {
		name = 'Olympus Mons',
		type = STARPORT_SURFACE,
		latitude = deg2rad(30),
		longitude = deg2rad(-37),
	}, {
		name = 'Mars High',
		type = STARPORT_ORBITAL,
		semi_major_axis = fixed(5068,100000000),
		rotation_period = fixed(11,24),
	}, {
		name = 'Phobos',
		type = PLANET_LARGE_ASTEROID,
		radius = fixed(21,10000),
		mass = fixed(18,10000000000),
		temp = 233,
		semi_major_axis = fixed(6268,100000000),
		eccentricity = fixed(151,10000),
		latitude = deg2rad(1.093),
		rotation_period = fixed(11,24),
		children = { {
			name = 'Phobos Base',
			type = STARPORT_SURFACE,
			latitude = deg2rad(5),
			longitude = deg2rad(-5),
		} },
	}, {
		name = 'Deimos',
		type = PLANET_ASTEROID,
		radius = fixed(12,10000),
		mass = fixed(25,10000000000),
		temp = 233,
		semi_major_axis = fixed(1568,100000000),
		eccentricity = fixed(2,10000),
		latitude = deg2rad(0.93),
		rotation_period = fixed(30,24),
		children = { {
			name = 'Tomm\'s Sanctuary',
			type = STARPORT_SURFACE,
		} },
	},
}

jupiter_children = {
}

saturn_children = {
}

uranus_children = {
}

neptune_children = {
}

pluto_children = {
}

sol_children = {
	{
		name = 'Mercury',
		type = PLANET_SMALL,
		radius = fixed(38,100),
		mass = fixed(55,1000),
		temp = 340,
		semi_major_axis = fixed(387,1000),
		eccentricity = fixed(205,1000),
		latitude = deg2rad(7.0),
		rotation_period = fixed(59,1),
		axial_tilt = fixed_deg2rad(1,100),
	}, {
		name = 'Venus',
		type = PLANET_CO2_THICK_ATMOS,
		radius = fixed(95,100),
		mass = fixed(815,1000),
		temp = 735,
		semi_major_axis = fixed(723,1000),
		eccentricity = fixed(7,1000),
		latitude = deg2rad(3.39),
		rotation_period = fixed(243,1),
		axial_tilt = fixed_deg2rad(26,10),
	}, {
		name = 'Earth',
		type = PLANET_INDIGENOUS_LIFE,
		radius = fixed(1,1),
		mass = fixed(1,1),
		temp = 288,
		semi_major_axis = fixed(1,1),
		eccentricity = fixed(167,10000),
		rotation_period = fixed(1,1),
		axial_tilt = fixed_deg2rad(2344,100),
		economy = INDUSTRY,
		height_map = 'data/earth.hmap',
		children = earth_children,
	}, {
		name = 'Mars',
		type = PLANET_TERRAFORMED_GOOD,
		radius = fixed(533,1000),
		mass = fixed(107,1000),
		temp = 274,
		semi_major_axis = fixed(152,1000),
		eccentricity = fixed(933,10000),
		latitude = deg2rad(1.85),
		rotation_period = fixed(1027,1000),
		axial_tilt = fixed_deg2rad(2519,100),
		economy = INDUSTRY,
		children = mars_children,
	}, {
		name = 'Jupiter',
		type = PLANET_LARGE_GAS_GIANT,
		radius = fixed(11,1),
		mass = fixed(3178,10),
		temp = 165,
		semi_major_axis = fixed(5204,1000),
		eccentricity = fixed(488,10000),
		latitude = deg2rad(1.305),
		rotation_period = fixed(4,10),
		axial_tilt = fixed_deg2rad(313,100),
		children = jupiter_children,
	}, {
		name = 'Saturn',
		type = PLANET_MEDIUM_GAS_GIANT,
		radius = fixed(9,1),
		mass = fixed(95152,1000),
		temp = 134,
		semi_major_axis = fixed(9582,1000),
		eccentricity = fixed(557,10000),
		latitude = deg2rad(2.485),
		rotation_period = fixed(4,10),
		axial_tilt = fixed_deg2rad(2673,100),
		children = saturn_children,
	}, {
		name = 'Uranus',
		type = PLANET_SMALL_GAS_GIANT,
		radius = fixed(4,1),
		mass = fixed(145,10),
		temp = 76,
		semi_major_axis = fixed(19229,1000),
		eccentricity = fixed(444,10000),
		latitude = deg2rad(0.772),
		rotation_period = fixed(7,10),
		axial_tilt = fixed_deg2rad(9777,100),
		children = uranus_children,
	}, {
		name = 'Neptune',
		type = PLANET_SMALL_GAS_GIANT,
		radius = fixed(38,10),
		mass = fixed(17147,100),
		temp = 72,
		semi_major_axis = fixed(30104,1000),
		eccentricity = fixed(112,10000),
		latitude = deg2rad(1.768),
		rotation_period = fixed(75,100),
		axial_tilt = fixed_deg2rad(2832,100),
		children = neptune_children,
	}, {
		name = 'Pluto',
		type = PLANET_PLANET_WATER,
		radius = fixed(18,100),
		mass = fixed(21,10000),
		temp = 44,
		semi_major_axis = fixed(394,10),
		eccentricity = fixed(249,1000),
		latitude = deg2rad(11.88),
		rotation_period = fixed(153,24),
		axial_tilt = fixed_deg2rad(296,10),
		children = pluto_children,
	},
}

sol_bodies = {
	name = 'Sol',
	type = TYPE_STAR_G,
	radius = fixed(1,1),
	mass = fixed(1,1),
	temp = 5700,
	children = sol_children,
}

define_system('Sol', {
	bodies = sol_bodies,
	type = { TYPE_STAR_G },
	sector = { 0, 0 },
	pos = v(0.5,0.5,0),
	short_desc = 'The historical birthplace of humankind',
	long_desc = [[Sol is a fine joint]],
})
