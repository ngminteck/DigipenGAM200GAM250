--[[

This script controls the jumping of the character.

Possible states this script can transition to:
- Jump-attack [DONE]
- Hurt [DONE]
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

	-- Variables for managing jump
	jumpTimeLimit = 0.25
	timePassed = 0
	hasJumped = false
	hitCeiling = false

	-- Variables for controlling jump
	jumpHeight = 10
	ceilingRebound = 5

	-- Variables for controlling air movement
	speed = 5 -- make sure this value is the same as SurtrWalk's

end


-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT
	ExecuteJump() -- remember to give gravity a really huge number
	CheckCeiling()
	MoveOnAir()
	CheckGetHurt()
	CheckAirAttack()
	EndJump()

end


-- Executes the jump function
function ExecuteJump()

	if timePassed <= jumpTimeLimit and hitCeiling == false then
		rb:SetVel(vec2.new(0, jumpHeight))
		hasJumped = true
	end

end


-- React if the player hits the ceiling while jumping
function CheckCeiling()

	if myCollision:GetColTop() == true then
		hitCeiling = true
		rb:SetVel(vec2.new(0, -ceilingRebound))
	end

end


-- Receives input and allows movement on air
function MoveOnAir()

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


-- Check for attack input mid-air
function CheckAirAttack()

	if myCollision:GetColBottom() == false and _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("JUMPATTACK")
	end

end


-- End jump state (aka I can jump again)
function EndJump()

	if timePassed > jumpTimeLimit and myCollision:GetColBottom() == true and hasJumped == true then
		timePassed = 0
		hasJumped = false
		hitCeiling = false
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
