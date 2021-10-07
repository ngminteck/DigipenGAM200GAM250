--[[

This script controls the death of the character.

]]--


-- Called at first frame.
function Start(sceneactor)

	-- For debug purposes
	scene = sceneactor
	
	timePassed = 0
	deathAnimDuration = 1.5

end


-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT
	
	if timePassed >= deathAnimDuration then
		scene:SetDead(true)
	end

end
