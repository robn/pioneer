-- additional string utilities

--
-- string.interp - expand string format with named arguments
--
-- string.interp("That will be ${cost} please, Mr. {name}.", {
--     cost = "25.0",
--     name = "Jameson",
-- })
--   -> "That will be $25.0 please, Mr. Jameson,"
--
string.interp = function (s, t)
	return (s:gsub('(%b{})', function(w) return t[w:sub(2,-2)] or w end))
end
