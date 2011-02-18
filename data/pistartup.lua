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
function v (...) return vector:new(...) end
function norm (...) return vector:new(...):norm() end
function f (...) return fixed:new(...) end

-- compatibility
Matrix = {}
function Matrix.new (...) return matrix:new(...) end
function Matrix.orient (...) return matrix:orient(...) end
function Matrix.rotate(a,v) return matrix:rotation(a,v) end
function Matrix.scale(v) return matrix:scale(v) end
function Matrix.translate(v) return matrix:translation(v) end

-- math library extensions
function math.deg2rad (n) return n * math.pi / 180 end

-- XXX I think I want a fixed math lib. if I do make one it'll probably be
--     named like this
math.fixed = {}
function math.fixed.deg2rad (n) return n * fixed:new(31416,1800000) end
