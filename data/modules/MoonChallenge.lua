local isMoonChallenge = false

Event.Register("onGameStart", function ()
	isMoonChallenge = Game.player.label == "Eagle"

	if isMoonChallenge then
		Timer:CallAt(Game.time+5, function ()
			Comms.Message("Eagle, Houston. If you read, you're GO for powered descent.", "Charlie Duke (CAPCOM)")
		end)
		Timer:CallAt(Game.time+10, function ()
			Comms.Message("Eagle, this is Columbia. They just gave you a GO for powered descent.", "Michael Collins (CMP)")
		end)
	end
end)
