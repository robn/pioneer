-- Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
-- Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

local Event = import("Event")

--
-- Interface: Comms
--
-- Player communication functions.
--

local Comms = {}

local messages = {}

local function addMessage (priority, message, from)
	local m = {
		priority = priority,
		message  = message,
		from     = from,
	}
	table.insert(messages, m)
	Event.Queue("onCommsMessage", m)
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
	return addMessage("normal", message, from)
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
	return addMessage("important", message, from)
end

Event.Register("onGameStart", function ()
    messages = {}
end)

-- XXX serializer

return Comms
