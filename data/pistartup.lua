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
function norm (x,y,z) return vector:new(x,y,z):norm() end
function f (n,d) return fixed:new(n,d) end

-- compatibility
Matrix = {}
function Matrix.new (x,y,z) return matrix:new(x,y,z) end
function Matrix.orient (x,y,z) return matrix:orient(x,y,z) end
function Matrix.rotate(a,v) return matrix:rotation(a,v) end
function Matrix.scale(v) return matrix:scale(v) end
function Matrix.translate(v) return matrix:translation(v) end

-- math library extensions
function math.deg2rad (n) return n * math.pi / 180 end

-- XXX I think I want a fixed math lib. if I do make one it'll probably be
--     named like this
math.fixed = {}
function math.fixed.deg2rad (n) return n * fixed:new(31416,1800000) end
