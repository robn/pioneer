-- Pioneer Lua startup

-- XXX for the moment the individual start scripts call this. once we have a
--     single place in the engine to instantiate a lua context it will load
--     it up automatically

-- rename wrapper classes
fixed  = pi_fixed
vector = pi_vector
matrix = pi_matrix

-- convenience constructors
-- XXX do these belong in eg picompat.lua?
function v (x,y,z) return vector:new(x,y,z) end
function f (n,d) return fixed:new(n,d) end

-- math library extensions
function math.deg2rad (n) return n * math.pi / 180 end

-- XXX I think I want a fixed math lib. if I do make one it'll probably be
--     named like this
math.fixed = {}
function math.fixed.deg2rad (n) return n * fixed:new(31416,1800000) end
