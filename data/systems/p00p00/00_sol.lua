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
    }
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
