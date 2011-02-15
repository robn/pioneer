vector = vector3f
function v (x,y,z) return vector:new(x,y,z) end
function f (n,d) return fixed:new(n,d) end

load_lua(CurrentDirectory .. "/systems")
