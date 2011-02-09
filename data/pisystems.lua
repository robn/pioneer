-- XXX These suck. Rather than keep them in sync lets get them auto-populated
---    into the Lua environment

TYPE_BROWN_DWARF  = 1
TYPE_STAR_M       = 2
TYPE_STAR_K       = 3
TYPE_STAR_G       = 4
TYPE_STAR_F       = 5
TYPE_STAR_A       = 6
TYPE_STAR_B       = 7
TYPE_STAR_O       = 8
TYPE_STAR_M_GIANT = 9
TYPE_WHITE_DWARF  = 10

GOV_NONE          = 1
GOV_EARTHCOLONIAL = 2
GOV_EARTHDEMOC    = 3
GOV_EMPIRERULE    = 4
GOV_CISLIBDEM     = 5
GOV_CISSOCDEM     = 6
GOV_LIBDEM        = 7
GOV_CORPORATE     = 8
GOV_SOCDEM        = 9
GOV_EARTHMILDICT  = 10
GOV_MILDICT1      = 11
GOV_MILDICT2      = 12
GOV_EMPIREMILDICT = 13
GOV_COMMUNIST     = 14
GOV_PLUTOCRATIC   = 15
GOV_DISORDER      = 16

function deg2rad () end
function fixed () end
function fixed_deg2rad () end

load_lua(CurrentDirectory .. "/systems")
