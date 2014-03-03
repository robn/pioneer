-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Event = import("Event")
local Engine = import("Engine")

--
-- Interface: Comms
--
-- Player communication functions.
--

local Comms = {}

local history = {}
local last = 0

local function addMessage (m)
	local key = util.hash_random(string.format("%s:%s:%s:%s", m.priority, m.message, m.from or "", m.target or "")) -- XXX use a crypto hash?
	local now = Engine.ticks
	if (last ~= key and (history[key] or 0) + 5000 < now) then -- 5 seconds
		history[key] = now
		last = key
		Event.Queue("onCommsMessage", m)
	end
end

--
-- Function: Message
--
-- Post a message to the player's comms panel
--
-- > Comms.Message(message, from)
--
-- Parameters:
--
--   message - the message text to post
--
--   from - optional; who the message is from (person, ship, etc)
--
-- Example:
--
-- > Comms.Message("Please repair my ship.", "Gary Jones")
--
-- Availability:
--
--   alpha 10
--
-- Status:
--
--   experimental
--

function Comms.Message (message, from)
	if type(from) == "userdata" then
		return addMessage({ priority = "normal", message = message, from = from.label, target = from })
	end
	return addMessage({ priority = "normal", message = message, from = from })
end

--
-- Function: ImportantMessage
--
-- Post an important message to the player's comms panel
--
-- > Comms.ImportantMessage(message, from)
--
-- The main difference between this and <Message> is that this will do more to
-- draw the player's attention to it with an audible alert and disable time
-- acceleration.
--
--
-- Parameters:
--
--   message - the message text to post
--
--   from - optional; who the message is from (person, ship, etc)
--
-- Example:
--
-- > Comms.ImportantMessage("Prepare to die!", "AB-1234")
--
-- Availability:
--
--   alpha 10
--
-- Status:
--
--   experimental
--

function Comms.ImportantMessage (message, from)
	if type(from) == "userdata" then
		return addMessage({ priority = "important", message = message, from = from.label, target = from })
	end
	return addMessage({ priority = "important", message = message, from = from })
end

Event.Register("onCoreCommsMessage", addMessage)
Event.Register("onGameStart", function ()
	history = {}
	last = 0
end)

-- XXX serializer

return Comms
