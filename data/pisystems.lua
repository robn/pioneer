-- XXX this is where I do my lua hacks until I'm settled on how I want libs
--    and things like that to work

-- vector is much more readable
vector = vector3f

-- convenience constructors
function v (x,y,z) return vector:new(x,y,z) end
function f (n,d) return fixed:new(n,d) end

-- degrees->radians for numbers
function math.deg2rad (n) return n * math.pi / 180 end

-- XXX I think I want a fixed math lib. if I do make one it'll probably be
--     named like this
math.fixed = {}

-- degrees->radians for fixeds
function math.fixed.deg2rad (n) return n * fixed:new(31416,1800000) end

load_lua(CurrentDirectory .. "/systems")
