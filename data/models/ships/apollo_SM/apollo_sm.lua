--
-- apollo service module
--
define_model('apollo_sm', {
	info = {
		scale = 4,
		lod_pixels = { 1, 10, 50, 0 },
		bounding_radius = 75,
		materials = {'gold', 'black', 'brown', 'metal', 'silver'},
		tags = {'ship'},
		},
	static = function(lod)
		set_material('black', 0.1, 0.1, 0.1, 1, 0, 0, 0, 0)
		set_material('gold', 0.615, 0.424, 0.04706, 1, 0, 0, 0, 0)
		set_material('silver', 0.599, 0.599, 0.599, 1, 0, 0, 0, 0)
		set_material('brown', 0.720, 0.490, 0.339, 1, 0, 0, 0, 0)
		set_material('metal', 0.543, 0.543, 0.543, 1, 0, 0, 0, 0)
		
		if (lod == 1) then
			load_obj('apollo-SM-LOD1.obj')
		else
			use_material('black')
			load_obj('black.obj')
			use_material('brown')
			load_obj('brown.obj')
			use_material('metal')
			load_obj('metal.obj')
			use_material('silver')
			load_obj('silver.obj')
			use_material('gold')
			load_obj('gold.obj')
		end
	end,
	dynamic = function(lod)
		-- Nothing
	end,
})
