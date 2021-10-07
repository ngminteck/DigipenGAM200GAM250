--[[

This script controls the character getting hurt.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Attack1 [DONE]
- Jump [DONE]
- Big Punch [DONE]
- Fall [DONE]
- Die

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()

	-- Manage the duration of this state
	timePassed = 0
	flinchDuration = 0.2

end


-- Called every frame.
function Update()

	-- Check whether the player is dead
	if myStats:GetHP() <= 0 then
		states:SetCurrLogicState("DIE")
	end
	
	-- Stop moving
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	-- Do nothing while waiting for the state to be over
	timePassed = timePassed + _G.DT
	if timePassed >= flinchDuration then
		ChooseNextState()
		timePassed = 0
	end

end


-- Select the natural next state based on conditions.
function ChooseNextState()

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	elseif _G.i:KeyDown(CVK_SPACE) then
		states:SetCurrLogicState("JUMP")
	elseif myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
	else
		states:SetCurrLogicState("IDLE")
	end

end