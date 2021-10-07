--[[

This script controls the checkpoint behaviour (torch version).

]]--


-- Called at first frame.
function Start(sceneactor,player)

	AllComponents = sceneactor:GetPrefab()
	sprite = AllComponents:GetSpriteComponent()
	states = AllComponents:GetMovementComponent()
	
	idlePFX = sceneactor:GetChild("CheckpointIdleFX")
	
	-- Variables to choose animation
	timePassed = 0
	ignitionDuration = 0.432

end


-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT
	if timePassed >= ignitionDuration then
		sprite:SetAnimation("Checkpoint_Lit", states)
		idlePFX:SetActive(false)
	else
		sprite:SetAnimation("Checkpoint_Ignite", states)
	end

end
