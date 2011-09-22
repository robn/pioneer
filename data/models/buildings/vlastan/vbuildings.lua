--since the buildings are very similar, we can use this wrapper function
local building = function(name, modelname, bradius, lodpixels, info)
	define_model(name, {
		info = {
			 scale = 1,
			 bounding_radius = bradius,
			 lod_pixels = lodpixels,
			 materials = {'wall'},
			 tags = {'building'},
			 building_info = info,
		},
		static = function(lod)
			--name, diffuse rgba, spec rgb+intensity, emit rgb
			set_material('wall', 1,1,1,1, .3,.3,.3,5, 0,0,0)
			texture('newbuilding.png')
			texture_glow('glowmap.png')
			use_material('wall')
			--uncomment this to identify buildings
			--text(modelname, v(0, bradius, 0), v(0,0,1), v(1,0,0), 8)
			if lod == 1 then
				load_obj(modelname .. 'LQ.obj')
			elseif lod == 2 then
				load_obj(modelname .. 'MQ.obj')
			else
				load_obj(modelname .. 'HQ.obj')
			end
		end
	})
end

--define ten buildings
building('vbuilding01', 'newbuilding1', 320, {12, 40, 100}, { population = 0.5, min_city_size = "MEDIUM", max_city_size = "HUGE" })
building('vbuilding02', 'newbuilding2', 250, {10, 40, 100}, { population = 0.5, min_city_size = "MEDIUM", max_city_size = "HUGE" })
building('vbuilding03', 'newbuilding3', 230, {8, 40, 100}, { population = 0.4, min_city_size = "MEDIUM", max_city_size = "HUGE" })
building('vbuilding04', 'newbuilding4', 220, {8, 40, 100}, { population = 0.4, min_city_size = "MEDIUM", max_city_size = "HUGE" })
building('vbuilding05', 'newbuilding5', 130, {6, 30, 100}, { population = 0.3, min_city_size = "SMALL", max_city_size = "LARGE" })
building('vbuilding06', 'newbuilding6', 120, {6, 30, 100}, { population = 0.25, min_city_size = "SMALL", max_city_size = "LARGE" })
building('vbuilding07', 'newbuilding7', 100, {5, 20, 100}, { population = 0.2, min_city_size = "TINY", max_city_size = "MEDIUM" })
building('vbuilding08', 'newbuilding8', 70,  {4, 10, 100}, { population = 0.2, min_city_size = "TINY", max_city_size = "MEDIUM" })
building('vbuilding09', 'newbuilding9', 100, {5, 15, 100}, { population = 0.1, min_city_size = "TINY", max_city_size = "HUGE", type = "POWER" })
building('vbuilding10', 'newbuilding10', 40, {2, 5, 100}, { population = 0.05, min_city_size = "TINY", max_city_size = "SMALL" })
