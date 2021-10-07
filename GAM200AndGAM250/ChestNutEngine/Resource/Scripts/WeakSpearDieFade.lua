--[[

This script allows only to let the death fade animation play.

]]--


-- Called at first frame.
function Start(sceneactor)

	-- For debug purposes
	move = sceneactor:GetPrefab():GetMovementComponent()
	scene =sceneactor
	timePassed = 0
	deathAnimDuration = 1.5

end


-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT
	
	if timePassed >= deathAnimDuration then
		scene:SetActive(false)
	end

end

