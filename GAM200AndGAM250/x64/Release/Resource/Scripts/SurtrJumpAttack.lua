--[[

This script controls the aerial attack of the character.

Possible states this script can transition to:
- Fall [DONE]
- Hurt [untested]
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
	attackHitbox = sceneactor:GetChild("PlayerHitbox")

	-- Variables for managing attack
	timePassed = 0
	attackDuration = 1
	
	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.2

	-- Variables for controlling movement
	speed = 5 -- same as SurtrWalk

end


-- Called every frame.
function Update()

	ExecuteJumpAttack()
	CheckHitboxCollision()
	MoveOnAir()
	CheckGetHurt()
	EndJumpAttack()

end


-- Executes the jump attack.
function ExecuteJumpAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay then

		attackHitbox:SetActive(true)
		timePassedForHitbox = 0

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	if attackHitbox:GetPrefab():GetCollisionComponent():GetColEnemy() == true then
		print("i is colliding")
		attackHitbox:SetActive(false)
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


-- End jump attack state
function EndJumpAttack()

	timePassed = timePassed + _G.DT

	if myCollision:GetColBottom() == true then
	
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("LAND")
		
	elseif timePassed >= attackDuration then
	
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("FALL")
		
	end

end
