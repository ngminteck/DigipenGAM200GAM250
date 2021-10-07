--[[

This script controls the falling of the character.

Possible states this script can transition to:
- Hurt [DONE]
- Jump-attack [DONE]
- Land [DONE]

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
	speed = 5 -- make it same as SurtrWalk

end


-- Called every frame.
function Update()

	-- print("falling")

	MoveOnInput()
	CheckGetHurt()
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


-- Check if the player got hurt mid-attack.
function CheckGetHurt()

	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
		
		states:SetCurrLogicState("HURT")
		
	end

end


-- Conditions to leave the current state.
function LeaveState()

	if _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("JUMPATTACK")
	end

	if myCollision:GetColBottom() == true then
		ChooseNextAction()
	end

end


-- Chooses the next action.
function ChooseNextAction()

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	elseif _G.i:KeyDown(CVK_SPACE) then
		states:SetCurrLogicState("JUMP")
	else
		states:SetCurrLogicState("IDLE")
	end

end
