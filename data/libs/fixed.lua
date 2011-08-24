-- additional fixed-point utilities

-- f - shorthand method to create a new fixed-point number
function f (...) return Fixed:New(...) end


-- math library equivalent for fixed-point numbers
math.fixed = {}

-- math.fixed.deg2rad - convert degrees to radians (fixed)
math.fixed.deg2rad = function (n) return n * Fixed:New(31416,1800000) end
