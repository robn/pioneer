dofile(CurrentDirectory .. "/pistartup.lua")

-- define_model(name, { info = {...}, static = function (lod) {...}, dynamic = function(lod) {...} } )
function define_model (name,data)
	m = pi_model:new(name)
	if data.info then m:info(data.info) end
	if data.static then
		m:static(function (m,lod)
			_current_model = m
			data.static(lod)
		end)
	end
	if data.static then
		m:dynamic(function (m,lod)
			_current_model = m
			data.dynamic(lod)
		end)
	end
	pi_model:add(m)
end

-- call_model(name, pos, xaxis, yaxis, scale)
function call_model (...) _current_model:call_model(...) end
-- use_material(name)
function use_material (...) _current_model:use_material(...) end
-- t = get_arg_material(n)
function get_arg_material (...) return _current_model:get_arg_material(...) end
-- sphere_slice(long_segs, lat_segs, slice_angle1, slice_angle2, transform)
function sphere_slice (...) _current_model:sphere_slice(...) end
-- invisible_tri(v1, v2, v3)
function invisible_tri (...) _current_model:invisible_tri(...) end
-- tri(v1, v2, v3)
function tri (...) _current_model:tri(...) end
-- xref_tri(v1, v2, v3)
function xref_tri (...) _current_model:xref_tri(...) end
-- quad(v1, v2, v3, v4)
function quad (...) _current_model:quad(...) end
-- quad(v1, v2, v3, v4)
function xref_quad (...) _current_model:xref_quad(...) end
-- cylinder(steps, start, end, updir, radius)
function cylinder (...) _current_model:cylinder(...) end
-- xref_cylinder(steps, start, end, updir, radius)
function xref_cylinder (...) _current_model:xref_cylinder(...) end
-- tapered_cylinder(steps, start, end, updir, radius1, radius2)
function tapered_cylinder (...) _current_model:tapered_cylinder(...) end
-- xref_tapered_cylinder(steps, start, end, updir, radius1, radius2)
function xref_tapered_cylinder (...) _current_model:xref_tapered_cylinder(...) end
-- tube(steps, start, end, updir, inner_radius, outer_radius)
function tube (...) _current_model:tube(...) end
-- xref_tube(steps, start, end, updir, inner_radius, outer_radius)
function xref_tube (...) _current_model:xref_tube(...) end
-- ring(steps, start, end, updir, radius)
function ring (...) _current_model:ring(...) end
-- xref_ring(steps, start, end, updir, radius)
function xref_ring (...) _current_model:xref_ring(...) end
-- circle(steps, center, normal, updir, radius)
function circle (...) _current_model:circle(...) end
-- xref_circle(steps, center, normal, updir, radius)
function xref_circle (...) _current_model:xref_circle(...) end
-- geomflag(flag)
function geomflag (...) _current_model:geomflag(...) end
-- billboard(name, size, color, { points })
function billboard (...) _current_model:billboard(...) end
-- set_local_lighting(flag)
function set_local_lighting (...) _current_model:set_local_lighting(...) end
-- set_light(num, quadratic_attenuation, pos, color)
function set_light (...) _current_model:set_light(...) end
-- use_light(num)
function use_light (...) _current_model:use_light(...) end

-- extrusion(start, end, updir, radius, points...)
function extrusion (start,_end,updir,radius,...) _current_model:extrusion(start,_end,updir,radius,{...}) end
-- quadric_bezier_quad(divs_v, divs_u, points...)
function quadric_bezier_quad (divs_v,divs_u,...) _current_model:quadric_bezier_quad(divs_v,divs_u,{...}) end
-- xref_quadric_bezier_quad(divs_v, divs_u, points...)
function xref_quadric_bezier_quad (divs_v,divs_u,...) _current_model:xref_quadric_bezier_quad(divs_v,divs_u,{...}) end
-- flat(divs, norm, segments...)
function flat (divs,norm,...) _current_model:flat(divs,norm,{...}) end
-- xref_flat(divs, norm, segments...)
function xref_flat (divs,norm,...) _current_model:xref_flat({...}) end

-- thruster(pos, dir, power, linear_only)
function thruster (pos,dir,power,linear_only)
	if linear_only == nil then
		_current_model:thruster(pos,dir,power,false)
	else
		_current_model:thruster(pos,dir,power,linear_only)
	end
end
-- xref_thruster(pos, dir, power, linear_only)
function thruster (pos,dir,power,linear_only)
	if linear_only == nil then
		_current_model:xref_thruster(pos,dir,power,false)
	else
		_current_model:xref_thruster(pos,dir,power,linear_only)
	end
end
-- text(str, pos, norm, textdir, scale, { center=..., xoffset=..., yoffset=... })
function text (str,pos,norm,textdir,scale,opts)
	if opts == nil then
		_current_model:text(str,pos,norm,textdir,scale,{})
	else
		_current_model:text(str,pos,norm,textdir,scale,opts)
	end
end
-- zbias(amount, pos, norm) or zbias(0)
function zbias (amount,pos,norm)
	if amount == 0 then
		_current_model:zbias(0,v(0,0,0),v(0,0,0))
		return
	end
	if pos == nil or norm == nil then
		error("zbias: non-zero bias amount requires position and normal")
	end
	_current_model:zbias(0,pos,norm)
end
-- texture(file, pos, uaxis, vaxis) or texture(file) or texture()
function texture (file,pos,uaxis,vaxis)
	if file == nil then
		_current_model:clear_texture()
		return
	end
	_current_model:texture(file)
	if pos ~= nil then
		_current_model:texture_transform(pos,uaxis,vaxis)
	end
end
-- set_material(name, args...) or set_material(name, { args... })
function set_material (name,m1,...)
    if type(m1) == "table" then
        _current_model:set_material(name, m1)
    else
        _current_model:set_material(name, { m1,... })
    end
end
-- sphere(subdivs, trans) or sphere(subdivs)
function sphere (subdivs,trans)
    if trans == nil then
        _current_model:sphere(subdivs,matrix:identity())
        return
    end
	_current_model:sphere(subdivs,trans)
end
-- cubic_bezier_quads(divs_v, divs_u, points...) or cubic_bezier_quad(divs_v, divs_u, { points... })
function cubic_bezier_quad (divs_v,divs_u,pt1,...)
    if type(pt1) == "table" then
        pi_model:cubic_bezier_quad(divs_v, divs_u, pt1)
    else
		pi_model:cubic_bezier_quad(divs_v, divs_u, { pt1,... })
    end
end
-- xref_cubic_bezier_quads(divs_v, divs_u, points...) or xref_cubic_bezier_quads(divs_v, divs_u, { points... })
function xref_cubic_bezier_quad (divs_v,divs_u,pt1,...)
    if type(pt1) == "table" then
        pi_model:xref_cubic_bezier_quad(divs_v, divs_u, pt1)
    else
		pi_model:xref_cubic_bezier_quad(divs_v, divs_u, { pt1,... })
    end
end
-- load_obj(name, trans)
function load_obj (name, trans)
    if trans == nil then
        _current_model:load_lua(name,matrix:identity())
        return
    end
	_current_model:load_obj(name,trans)
end

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

