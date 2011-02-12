define_model('viper_pol_body', {     
   	info = {
			scale = 1,
			lod_pixels={.1,80,600,0},
			bounding_radius = 15,
			materials={'col1', 'dark', 'glass', 'inside'},
			
        	},
                    
   	static = function(lod)

   	if lod > 1 then
		set_material('col1', .5,.5,.5,1,.95,.98,1,90)
		set_material('dark', .1,.1,.1,1,.1,.1,.1,10)
		set_material('inside', .5,.5,.5,1,.95,.98,1,20)
		set_material('glass', .5,.5,.5,.4,.95,.98,1,50)
		
		if lod > 3 then
		
			use_material('col1')
			texture('viper_pol1.png')	
			load_obj('viper.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			load_obj('viper_cockpit.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			use_material('dark')
			--texture(nil)
			load_obj('viper_intakepart1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
			set_light(1, 0.00000000008, v(0,0.19,-1.43), v(1,1,1))
			set_local_lighting(true)
			use_material('inside')
			texture('viper_pol1.png')
			load_obj('viper_inside.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			call_model('pilot1', v(0,0.18,-1.45),v(1,0,0),v(0,1,0),0.012)
			set_local_lighting(false)
			
			--texture(nil)
			use_material('glass')
			load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
		elseif lod > 2 then

			use_material('col1')
			texture('viper_pol2.png')	
			load_obj('viper.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			load_obj('viper_cockpit.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			use_material('dark')
			--texture(nil)
			load_obj('viper_intakepart1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
			--set_light(1, 0.00000000008, v(0,1.533,0.23), v(1,1,1))
			--set_local_lighting(true)
			--use_material('inside')
			--texture('viper.png')
			--load_obj('viper_inside.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			--call_model('pilot1', v(0,0.18,-1.46,v(1,0,0),v(0,1,0),0.012))
			--set_local_lighting(false)
			
			--texture(nil)
			use_material('col1')
			load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		else
			use_material('col1')
			texture('viper_pol3.png')	
			load_obj('viper_lod.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		end
	
	else
		load_obj('viper_col.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
	end

	end

															
})

define_model('viper_body', {     
   	info = {
			scale = 1,
			lod_pixels={.1,80,600,0},
			bounding_radius = 15,
			materials={'col1', 'dark', 'glass', 'inside'},
			
        	},
                    
   	static = function(lod)

		if lod > 1 then
		
		
			set_material('col1', .5,.5,.5,1,.95,.98,1,90)
			set_material('dark', .1,.1,.1,1,.1,.1,.1,10)
			set_material('inside', .5,.5,.5,1,.95,.98,1,20)
			set_material('glass', .5,.5,.5,.4,.95,.98,1,50)
		--[[
			selector2()
			select2 = 10
			if select2 < 56 then	
				if lod > 3 then
					texture('viper_a1.png')
				elseif lod > 2 then
					texture('viper_a2.png')
				else
					texture('viper_a3.png')
				end
			else
				if lod > 3 then
					texture('viper_pol1.png')
				elseif lod > 2 then
					texture('viper_pol2.png')
				else
					texture('viper_pol3.png')
				end
			end
		--]]
			
			if lod > 3 then
		
				use_material('col1')
				texture('viper_a1.png')	
				load_obj('viper.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				load_obj('viper_cockpit.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				use_material('dark')
				--texture(nil)
				load_obj('viper_intakepart1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
				set_light(1, 0.00000000008, v(0,0.19,-1.44), v(1,1,1))
				set_local_lighting(true)
				use_material('inside')
				--texture('viper_pol1.png')
				load_obj('viper_inside.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				call_model('pilot1', v(0,0.18,-1.45),v(1,0,0),v(0,1,0),0.012)
				set_local_lighting(false)
			
				--texture(nil)
				use_material('glass')
				load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
			elseif lod > 2 then

				use_material('col1')
				texture('viper_a2.png')	
				load_obj('viper.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				load_obj('viper_cockpit.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				use_material('dark')
				--texture(nil)
				load_obj('viper_intakepart1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			
				--set_light(1, 0.00000000008, v(0,1.533,0.23), v(1,1,1))
				--set_local_lighting(true)
				--use_material('inside')
				--texture('viper.png')
				--load_obj('viper_inside.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				--call_model('pilot1', v(0,0.18,-1.46,v(1,0,0),v(0,1,0),0.012))
				--set_local_lighting(false)
			
				--texture(nil)
				use_material('col1')
				load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			else
				use_material('col1')
				texture('viper_a3.png')	
				load_obj('viper_lod.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
				load_obj('viper_window.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
			end
		else
			load_obj('viper_col.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		end
		

	end

															
})

define_model('viper_pol_gear', {     
   	info = {
			scale = 1,
			lod_pixels={.1,80,160,0},
			bounding_radius = 5,
			materials={'col1'},
			
        	},
                    
   	static = function(lod)

   	--if lod > 1 then
		set_material('col1', .5,.5,.5,1,.95,.98,1,90)
	end,
		
	dynamic = function(lod)	
		use_material('col1')
		if lod > 3 then
			texture('models/ships/viper/viper_pol1.png')
		elseif lod > 2 then
			texture('models/ships/viper/viper_pol2.png')
		else
			texture('models/ships/viper/viper_pol3.png')
		end
		--texture('models/ships/viper/viper_pol1.png')	
		if get_arg(0) > 0.95 then
			load_obj('/models/ships/viper/gear20.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.9 then
			load_obj('/models/ships/viper/gear19.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.85 then
			load_obj('/models/ships/viper/gear18.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.8 then
			load_obj('/models/ships/viper/gear17.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.75 then
			load_obj('/models/ships/viper/gear16.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.7 then
			load_obj('/models/ships/viper/gear15.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.65 then
			load_obj('/models/ships/viper/gear14.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.6 then
			load_obj('/models/ships/viper/gear13.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.55 then
			load_obj('/models/ships/viper/gear12.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.5 then
			load_obj('/models/ships/viper/gear11.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.45 then
			load_obj('/models/ships/viper/gear10.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.4 then
			load_obj('/models/ships/viper/gear9.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.35 then
			load_obj('/models/ships/viper/gear8.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.3 then
			load_obj('/models/ships/viper/gear7.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.25 then
			load_obj('/models/ships/viper/gear6.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.2 then
			load_obj('/models/ships/viper/gear5.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.15 then
			load_obj('/models/ships/viper/gear4.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.1 then
			load_obj('/models/ships/viper/gear3.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.05 then
			load_obj('/models/ships/viper/gear2.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0 then
			load_obj('/models/ships/viper/gear1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		else
			load_obj('/models/ships/viper/gear.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))			
		end

		


	end

															
})

define_model('viper_gear', {     
   	info = {
			scale = 1,
			lod_pixels={.1,80,160,0},
			bounding_radius = 5,
			materials={'col1'},
			
        	},
                    
   	static = function(lod)

   	--if lod > 1 then
		set_material('col1', .5,.5,.5,1,.95,.98,1,90)
	end,
		
	dynamic = function(lod)	
		use_material('col1')
		if lod > 3 then
			texture('models/ships/viper/viper_a1.png')
		elseif lod > 2 then
			texture('models/ships/viper/viper_a2.png')
		else
			texture('models/ships/viper/viper_a3.png')
		end
		if get_arg(0) > 0.95 then
			load_obj('/models/ships/viper/gear20.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.9 then
			load_obj('/models/ships/viper/gear19.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.85 then
			load_obj('/models/ships/viper/gear18.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.8 then
			load_obj('/models/ships/viper/gear17.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.75 then
			load_obj('/models/ships/viper/gear16.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.7 then
			load_obj('/models/ships/viper/gear15.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.65 then
			load_obj('/models/ships/viper/gear14.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.6 then
			load_obj('/models/ships/viper/gear13.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.55 then
			load_obj('/models/ships/viper/gear12.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.5 then
			load_obj('/models/ships/viper/gear11.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.45 then
			load_obj('/models/ships/viper/gear10.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.4 then
			load_obj('/models/ships/viper/gear9.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.35 then
			load_obj('/models/ships/viper/gear8.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.3 then
			load_obj('/models/ships/viper/gear7.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.25 then
			load_obj('/models/ships/viper/gear6.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.2 then
			load_obj('/models/ships/viper/gear5.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.15 then
			load_obj('/models/ships/viper/gear4.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.1 then
			load_obj('/models/ships/viper/gear3.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0.05 then
			load_obj('/models/ships/viper/gear2.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		elseif get_arg(0) > 0 then
			load_obj('/models/ships/viper/gear1.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
		else
			load_obj('/models/ships/viper/gear.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))			
		end

		


	end

															
})

define_model('viperpol', {     
   	info = {
			scale = 10,
			lod_pixels={.1,80,600,0},
			bounding_radius = 20,
			materials={'col1'},
			tags = { 'ship' },
                 ship_defs = {
                	{
					name='Viper Police Craft',
					forward_thrust = -9e6,
					reverse_thrust = 3e6,
					up_thrust = 1e6,
					down_thrust = -1e6,
					left_thrust = -1e6,
					right_thrust = 1e6,
					angular_thrust = 1e6,
					gun_mounts = 
					{
					{ v(0,-2,-46), v(0,0,-1) },
					{ v(0,0,0), v(0,0,1) },
					},
					max_cargo = 60,
					max_laser = 1,
					max_missile = 4,
					capacity = 60,
					hull_mass = 60,
					price = 70000,
					hyperdrive_class = 3,
                    }
                  }
        	},
                    
   	static = function(lod)

   	if lod > 1 then
		set_material('col1', .5,.5,.5,1,.95,.98,1,90)		
		
		use_material('col1')
		--texture('viper_pol1.png')	
		call_model('viper_pol_body', v(0,0.22,0.38),v(1,0,0),v(0,1,0),1)
		call_model('viper_pol_gear', v(0,0.22,0.38),v(1,0,0),v(0,1,0),1)
		
		




		local vMainThruster = v(0.4,0.221,1.97)
		local vRetroThruster = v(0.71,0.231,-0.1)
		local vLeftThruster = v(-1.02,0.231,0.79)
		local vRightThruster = v(1.02,0.231,0.79)
		local vTopThruster = v(0,0.711,0.8)
		local vBottomThruster = v(0,-0.199,0.8)
		
		xref_thruster(vMainThruster, v(0,0,1), 3, true)
		xref_thruster(vRetroThruster, v(0,0,-1), 1.4, true)
		thruster(vLeftThruster, v(-1,0,0), 1)
		thruster(vRightThruster, v(1,0,0), 1)
		thruster(vTopThruster, v(0,1,0), 1)
		thruster(vBottomThruster, v(0,-1,0), 1)
		
	else
		--load_obj('viper_col.obj')
		load_obj('viper_col.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
	end


	end,
	
	
	dynamic = function(lod)
	
		if lod > 2 then
			local M_1 = v(0.202,0.807,1.294)
            local M_2 = v(-0.202,0.807,1.294)
            local M_3 = v(0.411,0.806,1.294)
            local M_4 = v(-0.411,0.806,1.294)
            
               
            if get_arg(12) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_1,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(13) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_2,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(14) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_3,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(15) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_4,v(1,0,0), v(0,.95,.05),0.16)
            end
            
		end
		
	end

															
})


define_model('viper', {     
   	info = {
			scale = 10,
			lod_pixels={.1,80,600,0},
			bounding_radius = 20,
			materials={'col1'},
			tags = { 'ship' },
                 ship_defs = {
                	{
					name='Viper Defence Craft',
					forward_thrust = -9e6,
					reverse_thrust = 3e6,
					up_thrust = 1e6,
					down_thrust = -1e6,
					left_thrust = -1e6,
					right_thrust = 1e6,
					angular_thrust = 1e6,
					gun_mounts = 
					{
					{ v(0,-2,-46), v(0,0,-1) },
					{ v(0,0,0), v(0,0,1) },
					},
					max_cargo = 60,
					max_laser = 1,
					max_missile = 4,
					capacity = 60,
					hull_mass = 60,
					price = 70000,
					hyperdrive_class = 3,
                    }
                  }
        	},
                    
   	static = function(lod)

   	if lod > 1 then
		set_material('col1', .5,.5,.5,1,.95,.98,1,90)		
		
		use_material('col1')
		--texture('viper_pol1.png')	
		call_model('viper_body', v(0,0.22,0.38),v(1,0,0),v(0,1,0),1)
		call_model('viper_gear', v(0,0.22,0.38),v(1,0,0),v(0,1,0),1)
		
		




		local vMainThruster = v(0.4,0.221,1.97)
		local vRetroThruster = v(0.71,0.231,-0.1)
		local vLeftThruster = v(-1.02,0.231,0.79)
		local vRightThruster = v(1.02,0.231,0.79)
		local vTopThruster = v(0,0.711,0.8)
		local vBottomThruster = v(0,-0.199,0.8)
		
		xref_thruster(vMainThruster, v(0,0,1), 3, true)
		xref_thruster(vRetroThruster, v(0,0,-1), 1.4, true)
		thruster(vLeftThruster, v(-1,0,0), 1)
		thruster(vRightThruster, v(1,0,0), 1)
		thruster(vTopThruster, v(0,1,0), 1)
		thruster(vBottomThruster, v(0,-1,0), 1)
		
	else
		--load_obj('viper_col.obj')
		load_obj('viper_col.obj', Matrix.rotate(0.5*math.pi,v(-1,0,0)))
	end


	end,
	
	
	dynamic = function(lod)
	
		if lod > 2 then
			local M_1 = v(0.202,0.807,1.294)
            local M_2 = v(-0.202,0.807,1.294)
            local M_3 = v(0.411,0.806,1.294)
            local M_4 = v(-0.411,0.806,1.294)
            
               
            if get_arg(12) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_1,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(12) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_1,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(13) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_2,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(13) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_2,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(14) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_3,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(14) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_3,v(1,0,0), v(0,.95,.05),0.16)
            end
            
            if get_arg(15) == Equip.Type.MISSILE_UNGUIDED  then
                call_model('m_unguided',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_GUIDED  then
                call_model('m_guided',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_SMART  then
                call_model('m_smart',M_4,v(1,0,0), v(0,.95,.05),0.16)
            elseif get_arg(15) == Equip.Type.MISSILE_NAVAL  then
                call_model('m_naval',M_4,v(1,0,0), v(0,.95,.05),0.16)
            end
            
		end
		
	end

															
})
