--[[

This script controls the dashing of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Attack1 [DONE]
- Jump [DONE]
- Big Punch [DONE]
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
	attackHitbox = sceneactor:GetChild("PlayerDashHitbox")
	myStats = AllComponents:GetStatsComponent()

	-- Variables for managing dash
	dashRecoverTime = 0.15
	timePassed = 0

	-- Variables for controlling dash
	dashDirection = "n"
	speed = 20

end


-- Called every frame.
function Update()

	DecideWhetherToDash()
	CheckGetHurt()

end


-- Decide whether to dash
function DecideWhetherToDash()

	timePassed = timePassed + _G.DT
	if timePassed <= dashRecoverTime then
		DetermineDashDirection()
		DashTowardsDirection()
	else
		LeaveState()
	end

end


-- Determine direction of dash
function DetermineDashDirection()

	if _G.i:KeyDown(CVK_LEFT) and dashDirection ~= "r" then
		dashDirection = "l"
	end

	if _G.i:KeyDown(CVK_RIGHT) and dashDirection ~= "l" then
		dashDirection = "r"
	end

end


-- Dash if player can still dash
function DashTowardsDirection()

	-- Start attack
	attackHitbox:SetActive(true)

	rb:SetVel(vec2.new(0, 0))

	if dashDirection == "l" then
		rb:SetVel(vec2.new(-speed, 0))
		states:SetCurrDirection(Direction.Left)
		dashDirection = "l"
	end

	if dashDirection == "r" then
		rb:SetVel(vec2.new(speed, 0))
		states:SetCurrDirection(Direction.Right)
		dashDirection = "r"
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


-- End dash
function LeaveState()

	-- Stop attack
	attackHitbox:SetActive(false)

	-- Reset dash conditions
	timePassed = 0
	dashDirection = "n"

	-- Decide next state
	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	elseif _G.i:KeyDown(CVK_SPACE) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("JUMP")
	elseif myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
	else
		states:SetCurrLogicState("IDLE")
	end

end
