--[[

This script controls the walking of the character.

Possible states this script can transition to:
- Idle [DONE]
- Attack1 [DONE]
- Jump [DONE]
- BigPunch [DONE]
- Dash [DONE]
- Hurt [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()

	-- Variables for controlling walk
	speed = 5

end


-- Called every frame.
function Update()

	--print("walking")

	MoveOnInput()
	LeaveState()

end


-- Receive input and move the character accordingly.
function MoveOnInput()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if _G.i:KeyDown(CVK_LEFT) then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
		states:SetCurrDirection(Direction.Left)
	end

	if _G.i:KeyDown(CVK_RIGHT) then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
		states:SetCurrDirection(Direction.Right)
	end

end


-- Conditions to leave the current state.
function LeaveState()

	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
		
		states:SetCurrLogicState("HURT")
		
	end

	if (_G.i:KeyUp(CVK_LEFT) and _G.i:KeyUp(CVK_RIGHT)) then
		states:SetCurrLogicState("IDLE")
	end

	if _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	end

	if _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	end

	if _G.i:KeyDown(CVK_SPACE) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("JUMP")
		--rb:SetForce(Force.new(vec2.new(0, 150), 0.000001, 0, true))
	end

	if _G.i:KeyDown(CVK_Z) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("DASH")
	end

	if myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
	end
	
	if myCollision:GetColCheckpoint() == true then
		states:SetCurrLogicState("END")
	end
	
end
