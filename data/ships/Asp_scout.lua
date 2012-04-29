--cloned from 'asp_explorer' 
-- Intended as a spy/smuggle type, carries fuel for long flights
-- long range, stealth features if implemented, extra sensors if implemented
-- somewhat heavy controls, poor yaw characteristics.  Good acceleration.



define_ship {
	name='Asp Scout',
	model='Asp_scout',
	forward_thrust = -220e5,
	reverse_thrust = 100e5,
	up_thrust = 60e5,
	down_thrust = -60e5,
	left_thrust = -60e5,
	right_thrust = 60e5,
	angular_thrust = 600e5,
	gun_mounts =
	{
		{ v(0,0.68,-2.573), v(0,0,-1) },
		{ v(0,-0.4,4.1), v(0,0,1) },
	},
	max_cargo = 120,
	max_missile = 1,
	max_laser = 2,
	max_cargoscoop = 1,
	max_fuelscoop = 1,
	capacity = 120,
	hull_mass = 60,
	fuel_tank_mass = 40,
	thruster_fuel_use = 0.00014,
	price = 187000,
	hyperdrive_class = 3,
}
