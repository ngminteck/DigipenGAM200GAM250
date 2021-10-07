--[[

This script controls this enemy getting hurt.

Possible states this script can transition to:
- Walk

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	stat = AllComponents:GetStatsComponent()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	
	-- Manage the duration of this state
	timePassed = 0
	flinchDuration = 0.7

end


-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	timePassed = timePassed + _G.DT
	
	if timePassed >= flinchDuration then
		states:SetCurrLogicState("WALK")
		timePassed = 0
	end
	if stat:GetHP()<=0 then
	states:SetCurrLogicState("DIE") 
	end

end