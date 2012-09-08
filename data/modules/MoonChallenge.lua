-- Apollo 11 radio transcripts taken from Spacelog
-- http://apollo11.spacelog.org

local ARMSTRONG = "Neil Armstrong (CDR)"
local COLLINS   = "Michael Collins (CMP)"
local DUKE      = "Charlie Duke (CAPCOM)"

local isMoonChallenge = false

Event.Register("onGameStart", function ()
	isMoonChallenge = Game.player.label == "Eagle"
	if not isMoonChallenge then return end

	local messages = {
		{ 5,  "Eagle, Houston. If you read, you're GO for powered descent.",           DUKE },
		{ 10, "Eagle, this is Columbia. They just gave you a GO for powered descent.", COLLINS },
	}

	for i = 1,#messages do
		local message = messages[i]
		Timer:CallAt(Game.time+message[1], function () Comms.Message(message[2], message[3]) end)
	end
end)

Event.Register("onShipLanded", function (ship, body)
	if not isMoonChallenge then return end
	if not Game.player == ship then return end
	if not body.label == "Moon" then return end

	local messages = {
		{ 5,  "Houston, Tranquility Base here.", ARMSTRONG },
		{ 10, "The Eagle has landed.",           ARMSTRONG },
		{ 15, "Roger, Tranquility. We copy you on the ground. You got a bunch of guys "..
		      "about to turn blue. We're breathing again. Thanks a lot.", DUKE },
		{ 20, "Thank you.",                      ARMSTRONG },
	}

	for i = 1,#messages do
		local message = messages[i]
		Timer:CallAt(Game.time+message[1], function () Comms.Message(message[2], message[3]) end)
	end
end)
