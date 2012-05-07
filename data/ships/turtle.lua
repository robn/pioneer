define_ship {
	name='Turtle',
	model='turtle',
	tags = { 'player_ship' },
	forward_thrust = -10e6,
	reverse_thrust = 10e6,
	up_thrust = 10e6,
	down_thrust = -10e6,
	left_thrust = -10e6,
	right_thrust = 10e6,
	angular_thrust = 30e6,
	gun_mounts =
	{
		{ v(0,-4,-10.2), v(0,0,-1) },
		{ v(0,-0.5,0), v(0,0,1) },
	},
	max_cargo = 90,
	max_laser = 2,
	max_missile = 4,
	max_cargoscoop = 0,
	capacity = 90,
	hull_mass = 50,
	fuel_tank_mass = 5,
	thruster_fuel_use = 0.0004,
	price = 250000,
	hyperdrive_class = 3,
}
