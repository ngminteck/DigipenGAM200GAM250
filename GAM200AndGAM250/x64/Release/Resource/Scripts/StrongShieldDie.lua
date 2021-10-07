--[[

This script controls this enemy's death.

]]--


-- Called at first frame.
function Start(sceneactor)

	-- For debug purposes
	move = sceneactor:GetPrefab():GetMovementComponent()
	
	timePassed = 0
	deathAnimDuration = 1.5

end


-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT
	
	if timePassed >= deathAnimDuration then
		move:SetCurrLogicState("DIEFADE")
	end

end
