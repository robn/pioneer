dofile(CurrentDirectory .. "/pistartup.lua")

function call_model (...) pi_model:call_model(...) end
function texture (file,pos,uaxis,vaxis)
	if file == nil then
		pi_model:texture()
        return
	end
	if pos == nil then
		pi_model:texture(file)
        return
	end
	pi_model:texture(file,pos,uaxis,vaxis)
end
function set_material (name,m1,...)
    if type(m1) == "table" then
        pi_model:set_material(name, m1)
    else
        pi_model:set_material(name, { m1,... })
    end
end
function use_material (...) pi_model:use_material(...) end
function get_arg_material (...) return pi_model:get_arg_material(...) end
function sphere (subdivs,trans)
    if trans == nil then
        pi_model:sphere(subdivs)
        return
    end
    pi_model:sphere(subdivs,trans)
end
function sphere_slice (...) pi_model:sphere_slice(...) end
function invisible_tri (...) pi_model:invisible_tri(...) end
function tri (...) pi_model:tri(...) end
function xref_tri (...) pi_model:xref_tri(...) end
function quad (...) pi_model:quad(...) end
function xref_quad (...) pi_model:xref_quad(...) end
function extrusion (...) pi_model:extrusion(...) end
function thruster (...) pi_model:thruster(...) end
function xref_thruster (...) pi_model:xref_thruster(...) end
function cylinder (...) pi_model:cylinder(...) end
function xref_cylinder (...) pi_model:xref_cylinder(...) end
function ring (...) pi_model:ring(...) end
function xref_ring (...) pi_model:xref_ring(...) end
function geomflag (...) pi_model:geomflag(...) end
function zbias (...) pi_model:zbias(...) end
function set_light (...) pi_model:set_light(...) end
function use_light (...) pi_model:use_light(...) end

function load_obj (...) pi_model:load_obj(...) end

--
-- Don't add models to this! Put them in ./models/
--

-- get_arg() indices
ARG_ALL_TIME_SECONDS = 1
ARG_ALL_TIME_MINUTES = 2
ARG_ALL_TIME_HOURS = 3
ARG_ALL_TIME_DAYS = 4

ARG_STATION_BAY1_STAGE = 6
ARG_STATION_BAY1_POS   = 10

ARG_SHIP_WHEEL_STATE = 0
ARG_SHIP_EQUIP_SCOOP = 5
ARG_SHIP_EQUIP_ENGINE = 6
ARG_SHIP_EQUIP_ECM = 7
ARG_SHIP_EQUIP_SCANNER = 8
ARG_SHIP_EQUIP_ATMOSHIELD = 9
ARG_SHIP_EQUIP_LASER0 = 10
ARG_SHIP_EQUIP_LASER1 = 11
ARG_SHIP_EQUIP_MISSILE0 = 12
ARG_SHIP_EQUIP_MISSILE1 = 13
ARG_SHIP_EQUIP_MISSILE2 = 14
ARG_SHIP_EQUIP_MISSILE3 = 15
ARG_SHIP_EQUIP_MISSILE4 = 16
ARG_SHIP_EQUIP_MISSILE5 = 17
ARG_SHIP_EQUIP_MISSILE6 = 18
ARG_SHIP_EQUIP_MISSILE7 = 19
ARG_SHIP_EQUIP_MISSILE8 = 20


-- First some useful utility functions! :D

math.clamp = function(v, min, max)
	return math.min(max, math.max(v,min))
end

function cuboid(pos, size)
	local sx = v(size:x(),0,0)
	local sy = v(0,size:y(),0)
	local sz = v(0,0,size:z())
	quad(pos, pos+sy, pos+sy+sx, pos+sx)
	quad(pos+sy, pos+sy+sz, pos+sy+sx+sz, pos+sy+sx)
	quad(pos, pos+sz, pos+sz+sy, pos+sy)
	quad(pos+sx, pos+sx+sy, pos+sx+sy+sz, pos+sx+sz)
	quad(pos+sz, pos+sz+sx, pos+sz+sx+sy, pos+sz+sy)
	quad(pos, pos+sx, pos+sz+sx, pos+sz)
end


-- Only need to pass 8 points because on the side
-- we are joined to 'old' patch, those 6 get generated
-- {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16},
-- edge 0 = 4,3,2,1
-- edge 1 = 16,12,8,4
-- edge 2 = 13,14,15,16
-- edge 3 = 1,5,9,13
function joined_patch(p, edge, pts)
	if edge == 0 then
		return { p[4],p[3],p[2],p[1],
		       2*p[4]-p[8], 2*p[3]-p[7], 2*p[2]-p[6], 2*p[1]-p[5],
		       pts[1], pts[2], pts[3], pts[4],
		       pts[5], pts[6], pts[7], pts[8] }
	elseif edge == 1 then
		return { p[16], p[12], p[8], p[4],
			2*p[16]-p[15], 2*p[12]-p[11], 2*p[8]-p[7], 2*p[4]-p[3],
		       pts[1], pts[2], pts[3], pts[4],
		       pts[5], pts[6], pts[7], pts[8] }
	elseif edge == 2 then
		return { p[13], p[14], p[15], p[16],
			2*p[13]-p[9], 2*p[14]-p[10], 2*p[15]-p[11], 2*p[16]-p[12],
		       pts[1], pts[2], pts[3], pts[4],
		       pts[5], pts[6], pts[7], pts[8] }
	elseif edge == 3 then
		return { p[1], p[5], p[9], p[13],
			2*p[1]-p[2], 2*p[5]-p[6], 2*p[9]-p[10], 2*p[13]-p[14],
		       pts[1], pts[2], pts[3], pts[4],
		       pts[5], pts[6], pts[7], pts[8] }
	end
end

function vlerp(t, v1, v2)
	return t*v2 + (1.0-t)*v1
end

function lerp_materials(a, m1, m2)
	local out = {}
	a = math.fmod(a, 2.002)
	if a > 1.0 then a = 2.002 - a end
	local b = 1.0 - a
	for i = 1,11 do
		out[i] = a*m2[i] + b*m1[i]
	end
	return out
end

--dofile "data/models/adverts.lua"
load_lua(CurrentDirectory .. "/model_functions")
load_lua(CurrentDirectory .. "/sub_models")
load_lua(CurrentDirectory .. "/models")

poo = 0
define_model('test', {
	info = {
			lod_pixels={30,60,100,0},
			bounding_radius = 10.0,
			tags = {'building', 'turd'},
			materials = {'red', 'shinyred'}
		},
	static = function (lod)
		set_material("red", 1,0,0,1)
		set_material("shinyred", 1,0,0,1, 1,1,1,50)
		use_material("red")
		sphere(4, Matrix.translate(v(-2,0,0)))
		sphere_slice(20,10,3.141*0.1,3.141*0.9, Matrix.translate(v(0,4,0)) * Matrix.orient(v(0,0,0), v(-1,0,0), v(0,-1,0)))
--		texture("concrete.png", v(0.25,0.25,0), v(0.2,0,0), v(0,-0.5,0))
		xref_flat(16, v(0,0,1),
			{v(4,0,0)}, -- straight line bit
			{v(4.5,-0.5,0),v(5,0,0)}, -- quadric bezier bit
			{v(5,0.5,0),v(4,0,0),v(4,1,0)}, -- cubic bezier bit
			{v(3,0.5,0)}, -- straight line bit
			{v(3,0.3,0)} -- etc
			)
		zbias(1, v(0,5,0), v(0,0,1))
		geomflag(0x8000)
		text("LOD: " .. tostring(lod), v(0,5,0), v(0,0,1), v(1,1,0):norm(), 1.0)
		geomflag(0)
		zbias(0)
		use_material("red")
		for i = 1,10 do
			tapered_cylinder(lod*4, v(i,0,0), v(i+1,0,0),
				v(0,1,0), math.abs(noise(5*i,0,0))+1,
				math.abs(noise(5*(i+1),0,0))+1)
		end
		use_material("shinyred")
		xref_circle(9, v(4,5,0), v(0,0,1), v(1,0,0), 1.0)
		tri(v(12,3,0),v(13,3,0), v(12,4,0))
		xref_tri(v(13,3,0),v(14,3,0), v(13,4,0))
		xref_quad(v(6,6,0), v(7,6,0), v(7,7,0),v(6,7,0))
		texture(nil)
	--[[
		xref_cubic_bezier_tri(32, v(0,0,0), v(1,0,0), v(2,0,0), v(3,0,0),
					v(0,-1,0), v(1,-1,3), v(3,-1,0),
					v(0,-2,0), v(2,-2,0),
					v(0,-3,0))
					--]]
	---[[
		xref_quadric_bezier_tri(16, v(0,0,0), v(1,0,0), v(2,0,0),
					v(0,-1,0), v(1,-1,2),
					v(0,-2,0))
					--]]
	--[[	
		geomflag(0x8000)
		xref_quadric_bezier_quad(16, 16,
				v(0,0,0), v(1,-1,0), v(2,0,0),
				v(-1,1,0), v(1,1,8), v(3,1,0),
				v(0,2,0), v(1,3,0), v(2,2,0))
				--]]
	--[[	
		xref_cubic_bezier_quad(32, 32,
				v(0,0,0), v(1,0,0), v(2,0,0), v(3,0,0),
				v(0,1,0), v(1,1,5), v(2,1,0), v(3,1,0),
				v(0,2,0), v(1,2,0), v(2,2,0), v(3,2,0),
				v(0,4,0), v(1,4,0), v(1,4,0), v(1,3,0))
				--]]
	--[[
		extrusion(v(0,0,0), v(0,0,-5), v(0,1,0), 1.0,
			v(1,0,0), v(0.5, 0.8, 0), v(0,1,0), v(-0.5,0.8,0), v(-1,0,0),
			v(0,-0.5,0))
			--]]
	end,
	dynamic = function(lod)
		poo = poo + 0.005
		set_material("red", math.sin(poo)*math.sin(poo), 0.5, 0.5, 1)
		use_material("red")
		xref_cylinder(16, v(-8,0,0), v(-8,5,0), v(1,0,0), math.abs(math.sin(poo)))
		circle(9, v(5*math.sin(poo),5*math.cos(poo),0), v(0,0,1), v(1,0,0), 1.0)

		local ang = 2*math.pi*get_arg(0)
		--call_model("blob", v(0,0,-20), v(1,0,0), v(1,1,0),1.0)
		billboard('smoke.png', 5, v(.5,.5,1), { v(0,0,0), v(10,3,0) })
	end
})

define_model('blob', {
	info = {
			bounding_radius=8,
			materials={'blue'}
		},
	static = function(lod)
		set_material("blue", 0,0,1,1)
		use_material("blue")
		cylinder(16, v(-5,0,0), v(-5,5,0), v(1,0,0), 1.0)
		text("blob_static()", v(-5,-2,0), v(0,0,1), v(1,0,0), 0.5)
		xref_thruster(v(5,0,0), v(0,0,-1), 10)
		xref_thruster(v(5,0,0), v(0,0,1), 10)
		xref_thruster(v(5,0,0), v(0,1,0), 5)
		xref_thruster(v(5,0,0), v(0,-1,0), 5)
		thruster(v(5,0,-5), v(1,0,0), 5, true)
		thruster(v(5,0,5), v(1,0,0), 5, true)
		thruster(v(-5,0,-5), v(-1,0,0), 5, true)
		thruster(v(-5,0,5), v(-1,0,0), 5, true)
		text("HELLO FROM BLOB", v(0,0,0), v(0,0,1), v(1,0,0), 10.0)
	end
})

define_model('tombstone', {
	info = {
			scale = 10.0,
			bounding_radius = 1,
			materials={'stone', 'text'}
		},
	static = function(lod)
		local v06 = v(0.6, 1.0, -0.1)
		local v07 = v(0.6, -1.0, -0.1)
		local v08 = v(-0.6, -1.0, -0.1)
		local v09 = v(-0.6, 1.0, -0.1)
		local v10 = v(0, 1, 0.1)
		local v11 = v(0, 1, -0.1)
		local v12 = v(0.6, 1.0, 0.1)
		local v13 = v(0.6, -1.0, 0.1)
		local v14 = v(-0.6, -1.0, 0.1)
		local v15 = v(-0.6, 1.0, 0.1)
		set_material('text', 1,1,1,1)
		set_material('stone', .5,.5,.5,1)
		use_material('stone')
		quad(v06, v07, v08, v09)
		quad(v15, v14, v13, v12)
		quad(v06, v12, v13, v07)
		quad(v09, v08, v14, v15)
		quad(v08, v07, v13, v14)
		cylinder(16, v10, v11, v(0,1,0), .6)
	end,
	dynamic = function(lod)
		local v16 = v(0, 0.5, -0.1)
		local v17 = v(0, 0.5, 0.1)
		use_material('text')
		zbias(1, v16, v(0,0,-1))
		text(get_arg_string(0), v16, v(0,0,-1), v(-1,0,0), .1, {center=true})
		zbias(1, v17, v(0,0,1))
		text(get_arg_string(0), v17, v(0,0,1), v(1,0,0), .1, {center=true})
		zbias(0)
	end
})

m = Matrix.rotate(math.pi*0.25,v(1,1,1))
--m:print()
m = m:inverse()
--m:print()
a = (m*v(1,0,0))
--a:print()

define_model('cargo', {
	info = {
			lod_pixels = {20, 50, 0},
			bounding_radius = 1.5,
			materials = {'body', 'text'}
		},
	static = function(lod)
		local divs = 8*lod
		set_material('body', .5,.5,.5,1, 0,0,0, 0, 0,0,0)
		set_material('text', 1,0,0,1, 0,0,0, 0, 0,0,0)
		local top = v(0,1,0)
		local bottom = v(0,-1,0)
		use_material('body')
		cylinder(divs, top, bottom, v(1,0,0), 1.0)
	end,
	dynamic = function(lod)
		if lod == 3 then
			local textpos = v(0,1,0)
			use_material('text')
			zbias(1, textpos, v(0,1,0))
			text(get_arg_string(0), textpos, v(0,1,0), v(1,0,0), 0.1, {center=true})
		end
	end
})

