#include "SystemConstants.h"

namespace SystemConstants {

// indexed by enum type turd
const float starColors[SystemBody::TYPE_LAST][3] = {
	{ 0, 0, 0 }, // gravpoint
	{ 0.5f, 0.0f, 0.0f }, // brown dwarf
	{ 0.4f, 0.4f, 0.8f }, // white dwarf
	{ 1.0f, 0.2f, 0.0f }, // M
	{ 1.0f, 0.6f, 0.1f }, // K
	{ 1.0f, 1.0f, 0.4f }, // G
	{ 1.0f, 1.0f, 0.8f }, // F
	{ 1.0f, 1.0f, 1.0f }, // A
	{ 0.7f, 0.7f, 1.0f }, // B
	{ 1.0f, 0.7f, 1.0f }, // O
	{ 1.0f, 0.2f, 0.0f }, // M Giant
	{ 1.0f, 0.6f, 0.1f }, // K Giant
	{ 1.0f, 1.0f, 0.4f }, // G Giant
	{ 1.0f, 1.0f, 0.8f }, // F Giant
	{ 1.0f, 1.0f, 1.0f }, // A Giant
	{ 0.7f, 0.7f, 1.0f }, // B Giant
	{ 1.0f, 0.7f, 1.0f }, // O Giant
	{ 1.0f, 0.2f, 0.0f }, // M Super Giant
	{ 1.0f, 0.6f, 0.1f }, // K Super Giant
	{ 1.0f, 1.0f, 0.4f }, // G Super Giant
	{ 1.0f, 1.0f, 0.8f }, // F Super Giant
	{ 1.0f, 1.0f, 1.0f }, // A Super Giant
	{ 0.7f, 0.7f, 1.0f }, // B Super Giant
	{ 1.0f, 0.7f, 1.0f }, // O Super Giant
	{ 1.0f, 0.2f, 0.0f }, // M Hyper Giant
	{ 1.0f, 0.6f, 0.1f }, // K Hyper Giant
	{ 1.0f, 1.0f, 0.4f }, // G Hyper Giant
	{ 1.0f, 1.0f, 0.8f }, // F Hyper Giant
	{ 1.0f, 1.0f, 1.0f }, // A Hyper Giant
	{ 0.7f, 0.7f, 1.0f }, // B Hyper Giant
	{ 1.0f, 0.7f, 1.0f }, // O Hyper Giant
	{ 1.0f, 0.2f, 0.0f }, // Red/M Wolf Rayet Star
	{ 0.7f, 0.7f, 1.0f }, // Blue/B Wolf Rayet Star
	{ 1.0f, 0.7f, 1.0f }, // Purple-Blue/O Wolf Rayet Star
	{ 0.3f, 0.7f, 0.3f }, // Stellar Blackhole
	{ 0.2f, 0.9f, 0.2f }, // Intermediate mass Black-hole
	{ 0.0f, 1.0f, 0.0f }, // Super massive black hole
};

// indexed by enum type turd
const float starRealColors[][3] = {
	{ 0, 0, 0 }, // gravpoint
	{ 0.5f, 0.0f, 0.0f }, // brown dwarf
	{ 1.0f, 1.0f, 1.0f }, // white dwarf
	{ 1.0f, 0.5f, 0.2f }, // M
	{ 1.0f, 1.0f, 0.4f }, // K
	{ 1.0f, 1.0f, 0.95f }, // G
	{ 1.0f, 1.0f, 1.0f }, // F
	{ 1.0f, 1.0f, 1.0f }, // A
	{ 0.8f, 0.8f, 1.0f }, // B
	{ 1.0f, 0.8f, 1.0f },  // O
	{ 1.0f, 0.5f, 0.2f }, // M Giant
	{ 1.0f, 1.0f, 0.4f }, // K Giant
	{ 1.0f, 1.0f, 0.95f }, // G Giant
	{ 1.0f, 1.0f, 1.0f }, // F Giant
	{ 1.0f, 1.0f, 1.0f }, // A Giant
	{ 0.8f, 0.8f, 1.0f }, // B Giant
	{ 1.0f, 0.8f, 1.0f },  // O Giant
	{ 1.0f, 0.5f, 0.2f }, // M Super Giant
	{ 1.0f, 1.0f, 0.4f }, // K Super Giant
	{ 1.0f, 1.0f, 0.95f }, // G Super Giant
	{ 1.0f, 1.0f, 1.0f }, // F Super Giant
	{ 1.0f, 1.0f, 1.0f }, // A Super Giant
	{ 0.8f, 0.8f, 1.0f }, // B Super Giant
	{ 1.0f, 0.8f, 1.0f },  // O Super Giant
	{ 1.0f, 0.5f, 0.2f }, // M Hyper Giant
	{ 1.0f, 1.0f, 0.4f }, // K Hyper Giant
	{ 1.0f, 1.0f, 0.95f }, // G Hyper Giant
	{ 1.0f, 1.0f, 1.0f }, // F Hyper Giant
	{ 1.0f, 1.0f, 1.0f }, // A Hyper Giant
	{ 0.8f, 0.8f, 1.0f }, // B Hyper Giant
	{ 1.0f, 0.8f, 1.0f },  // O Hyper Giant
	{ 1.0f, 0.6f, 0.6f }, // M WF
	{ 0.8f, 0.8f, 1.0f }, // B WF
	{ 1.0f, 0.8f, 1.0f },  // O WF
	{ 1.0f, 1.0f, 1.0f },  // small Black hole
	{ 0.06f, 0.0f, 0.08f }, // med BH
	{ 0.04f, 0.0f, 0.06f }, // massive BH
};

const double starLuminosities[] = {
	0,
	0.0003, // brown dwarf
	0.1, // white dwarf
	0.08, // M0
	0.38, // K0
	1.2, // G0
	5.1, // F0
	24.0, // A0
	100.0, // B0
	200.0, // O5
	1000.0, // M0 Giant
	2000.0, // K0 Giant
	4000.0, // G0 Giant
	6000.0, // F0 Giant
	8000.0, // A0 Giant
	9000.0, // B0 Giant
	12000.0, // O5 Giant
	12000.0, // M0 Super Giant
	14000.0, // K0 Super Giant
	18000.0, // G0 Super Giant
	24000.0, // F0 Super Giant
	30000.0, // A0 Super Giant
	50000.0, // B0 Super Giant
	100000.0, // O5 Super Giant
	125000.0, // M0 Hyper Giant
	150000.0, // K0 Hyper Giant
	175000.0, // G0 Hyper Giant
	200000.0, // F0 Hyper Giant
	200000.0, // A0 Hyper Giant
	200000.0, // B0 Hyper Giant
	200000.0, // O5 Hyper Giant
	50000.0, // M WF
	100000.0, // B WF
	200000.0, // O WF
	0.0003, // Stellar Black hole
	0.00003, // IM Black hole
	0.000003, // Supermassive Black hole
};

const float starScale[] = {  // Used in sector view
	0,
	0.6f, // brown dwarf
	0.5f, // white dwarf
	0.7f, // M
	0.8f, // K
	0.8f, // G
	0.9f, // F
	1.0f, // A
	1.1f, // B
	1.1f, // O
	1.3f, // M Giant
	1.2f, // K G
	1.2f, // G G
	1.2f, // F G
	1.1f, // A G
	1.1f, // B G
	1.2f, // O G
	1.8f, // M Super Giant
	1.6f, // K SG
	1.5f, // G SG
	1.5f, // F SG
	1.4f, // A SG
	1.3f, // B SG
	1.3f, // O SG
	2.5f, // M Hyper Giant
	2.2f, // K HG
	2.2f, // G HG
	2.1f, // F HG
	2.1f, // A HG
	2.0f, // B HG
	1.9f, // O HG
	1.1f, // M WF
	1.3f, // B WF
	1.6f, // O WF
	1.0f, // Black hole
	2.5f, // Intermediate-mass blackhole
	4.0f  // Supermassive blackhole
};

const fixed starMetallicities[] = {
	fixed(1,1), // GRAVPOINT - for planets that orbit them
	fixed(9,10), // brown dwarf
	fixed(5,10), // white dwarf
	fixed(7,10), // M0
	fixed(6,10), // K0
	fixed(5,10), // G0
	fixed(4,10), // F0
	fixed(3,10), // A0
	fixed(2,10), // B0
	fixed(1,10), // O5
	fixed(8,10), // M0 Giant
	fixed(65,100), // K0 Giant
	fixed(55,100), // G0 Giant
	fixed(4,10), // F0 Giant
	fixed(3,10), // A0 Giant
	fixed(2,10), // B0 Giant
	fixed(1,10), // O5 Giant
	fixed(9,10), // M0 Super Giant
	fixed(7,10), // K0 Super Giant
	fixed(6,10), // G0 Super Giant
	fixed(4,10), // F0 Super Giant
	fixed(3,10), // A0 Super Giant
	fixed(2,10), // B0 Super Giant
	fixed(1,10), // O5 Super Giant
	fixed(1,1), // M0 Hyper Giant
	fixed(7,10), // K0 Hyper Giant
	fixed(6,10), // G0 Hyper Giant
	fixed(4,10), // F0 Hyper Giant
	fixed(3,10), // A0 Hyper Giant
	fixed(2,10), // B0 Hyper Giant
	fixed(1,10), // O5 Hyper Giant
	fixed(1,1), // M WF
	fixed(8,10), // B WF
	fixed(6,10), // O WF
	fixed(1,1), //  S BH	Blackholes, give them high metallicity,
	fixed(1,1), // IM BH	so any rocks that happen to be there
	fixed(1,1)  // SM BH	may be mining hotspots. FUN :)
};

const StarTypeInfo starTypeInfo[] = {
	{
		SystemBody::SUPERTYPE_NONE, {}, {},
        0, 0
	}, {
		SystemBody::SUPERTYPE_STAR, //Brown Dwarf
		{2,8}, {10,30},
		1000, 2000
	}, {
		SystemBody::SUPERTYPE_STAR,  //white dwarf
		{20,100}, {1,2},
		4000, 40000
	}, {
		SystemBody::SUPERTYPE_STAR, //M
		{10,47}, {30,60},
		2000, 3500
	}, {
		SystemBody::SUPERTYPE_STAR, //K
		{50,78}, {60,100},
		3500, 5000
	}, {
		SystemBody::SUPERTYPE_STAR, //G
		{80,110}, {80,120},
		5000, 6000
	}, {
		SystemBody::SUPERTYPE_STAR, //F
		{115,170}, {110,150},
		6000, 7500
	}, {
		SystemBody::SUPERTYPE_STAR, //A
		{180,320}, {120,220},
		7500, 10000
	}, {
		SystemBody::SUPERTYPE_STAR,  //B
		{200,300}, {120,290},
		10000, 30000
	}, {
		SystemBody::SUPERTYPE_STAR, //O
		{300,400}, {200,310},
		30000, 60000
	}, {
		SystemBody::SUPERTYPE_STAR, //M Giant
		{60,357}, {2000,5000},
		2500, 3500
	}, {
		SystemBody::SUPERTYPE_STAR, //K Giant
		{125,500}, {1500,3000},
		3500, 5000
	}, {
		SystemBody::SUPERTYPE_STAR, //G Giant
		{200,800}, {1000,2000},
		5000, 6000
	}, {
		SystemBody::SUPERTYPE_STAR, //F Giant
		{250,900}, {800,1500},
		6000, 7500
	}, {
		SystemBody::SUPERTYPE_STAR, //A Giant
		{400,1000}, {600,1000},
		7500, 10000
	}, {
		SystemBody::SUPERTYPE_STAR,  //B Giant
		{500,1000}, {600,1000},
		10000, 30000
	}, {
		SystemBody::SUPERTYPE_STAR, //O Giant
		{600,1200}, {600,1000},
		30000, 60000
	}, {
		SystemBody::SUPERTYPE_STAR, //M Super Giant
		{1050,5000}, {7000,15000},
		2500, 3500
	}, {
		SystemBody::SUPERTYPE_STAR, //K Super Giant
		{1100,5000}, {5000,9000},
		3500, 5000
	}, {
		SystemBody::SUPERTYPE_STAR, //G Super Giant
		{1200,5000}, {4000,8000},
		5000, 6000
	}, {
		SystemBody::SUPERTYPE_STAR, //F Super Giant
		{1500,6000}, {3500,7000},
		6000, 7500
	}, {
		SystemBody::SUPERTYPE_STAR, //A Super Giant
		{2000,8000}, {3000,6000},
		7500, 10000
	}, {
		SystemBody::SUPERTYPE_STAR,  //B Super Giant
		{3000,9000}, {2500,5000},
		10000, 30000
	}, {
		SystemBody::SUPERTYPE_STAR, //O Super Giant
		{5000,10000}, {2000,4000},
		30000, 60000
	}, {
		SystemBody::SUPERTYPE_STAR, //M Hyper Giant
		{5000,15000}, {20000,40000},
		2500, 3500
	}, {
		SystemBody::SUPERTYPE_STAR, //K Hyper Giant
		{5000,17000}, {17000,25000},
		3500, 5000
	}, {
		SystemBody::SUPERTYPE_STAR, //G Hyper Giant
		{5000,18000}, {14000,20000},
		5000, 6000
	}, {
		SystemBody::SUPERTYPE_STAR, //F Hyper Giant
		{5000,19000}, {12000,17500},
		6000, 7500
	}, {
		SystemBody::SUPERTYPE_STAR, //A Hyper Giant
		{5000,20000}, {10000,15000},
		7500, 10000
	}, {
		SystemBody::SUPERTYPE_STAR,  //B Hyper Giant
		{5000,23000}, {6000,10000},
		10000, 30000
	}, {
		SystemBody::SUPERTYPE_STAR, //O Hyper Giant
		{10000,30000}, {4000,7000},
		30000, 60000
	}, {
		SystemBody::SUPERTYPE_STAR,  // M WF
		{2000,5000}, {2500,5000},
		25000, 35000
	}, {
		SystemBody::SUPERTYPE_STAR,  // B WF
		{2000,7500}, {2500,5000},
		35000, 45000
	}, {
		SystemBody::SUPERTYPE_STAR,  // O WF
		{2000,10000}, {2500,5000},
		45000, 60000
	}, {
		SystemBody::SUPERTYPE_STAR,  // S BH
		{20,2000}, {0,0},	// XXX black holes are < 1 Sol radii big; this is clamped to a non-zero value later
		10, 24
	}, {
		SystemBody::SUPERTYPE_STAR,  // IM BH
		{900000,1000000}, {100,500},
		1, 10
	}, {
		SystemBody::SUPERTYPE_STAR,  // SM BH
		{2000000,5000000}, {10000,20000},
		10, 24
	}
/*	}, {
		SystemBody::SUPERTYPE_GAS_GIANT,
		{}, 950, Lang::MEDIUM_GAS_GIANT,
	}, {
		SystemBody::SUPERTYPE_GAS_GIANT,
		{}, 1110, Lang::LARGE_GAS_GIANT,
	}, {
		SystemBody::SUPERTYPE_GAS_GIANT,
		{}, 1500, Lang::VERY_LARGE_GAS_GIANT,
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 1, Lang::ASTEROID,
		"icons/object_planet_asteroid.png"
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 2, "Large asteroid",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 26, "Small, rocky dwarf planet", // moon radius
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 26, "Small, rocky dwarf planet", // dwarf2 for moon-like colours
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 52, "Small, rocky planet with a thin atmosphere", // mars radius
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Rocky frozen planet with a thin nitrogen atmosphere", // earth radius
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Dead world that once housed it's own intricate ecosystem.", // earth radius
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Rocky planet with a carbon dioxide atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Rocky planet with a methane atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Water world with vast oceans and a thick nitrogen atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Rocky planet with a thick carbon dioxide atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Rocky planet with a thick methane atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "Highly volcanic world",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 100, "World with indigenous life and an oxygen atmosphere",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 60, "Marginal terraformed world with minimal plant life",
	}, {
		SystemBody::SUPERTYPE_ROCKY_PLANET,
		{}, 90, "Fully terraformed world with introduced species from numerous successful colonies",
	}, {
		SystemBody::SUPERTYPE_STARPORT,
		{}, 0, Lang::ORBITAL_STARPORT,
	}, {
		SystemBody::SUPERTYPE_STARPORT,
		{}, 0, Lang::STARPORT,
	}*/
};

}
