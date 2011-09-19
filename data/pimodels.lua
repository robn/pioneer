--
-- Don't add models to this! Put them in ./models/
--
dofile(CurrentDirectory .. "/pienums.lua")

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
ARG_SHIP_FLIGHT_STATE = 20


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

--dofile "models/adverts.lua"
load_lua(CurrentDirectory .. "/sub_models")
load_lua(CurrentDirectory .. "/models")

print("If you see Error IMG_Load below, delete your ~/.pioneer/model_cache folder.");

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

