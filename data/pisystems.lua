vector = vector3f
function v (x,y,z) return vector:new(x,y,z) end

function f (n,d) return fixed:new(n,d) end
function fixed_deg2rad (n,d) return fixed:new(n,d) * fixed:new(31416,1800000) end

load_lua(CurrentDirectory .. "/systems")
