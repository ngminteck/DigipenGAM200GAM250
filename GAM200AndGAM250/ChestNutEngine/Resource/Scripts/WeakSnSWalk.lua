--[[

This script controls the enemy's walk.

Possible states this script can transition to:
- Attack [DONE]
- CounterStance [DONE]
- Hurt

]]--


-- Called at first frame.
function Start(sceneactor, player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myTransform = AllComponents:GetTransformComponent()
	myPos = myTransform:GetPosition().x

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	playerPos = playerTransform:GetPosition().x

	-- Variables for controlling walk
	speed = 3
	facingRight = true

	-- Variables for randomising
	shouldAttack = true
	stopRandomising = false

end


-- Called every frame.
function Update()

	if stopRandomising == false then
		PseudoRandom()
	end

	UpdatePosition()
	FacePlayerDirection()
	WalkTowardsPlayer()
	CheckGetHurt()
	CheckWhetherToAttack()

end


-- A temporary function to randomise the next action.
function PseudoRandom()

	if shouldAttack == true then
		shouldAttack = false
	else
		shouldAttack = true
	end

end


-- Update the player's own position
function UpdatePosition()

	myPos = myTransform:GetPosition().x
	playerPos = playerTransform:GetPosition().x

end


-- Face the direction where player is.
function FacePlayerDirection()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if myPos < (playerPos - 3) then
		facingRight = true
		states:SetCurrDirection(Direction.Right)
	end

	if myPos > (playerPos + 3) then
		facingRight = false
		states:SetCurrDirection(Direction.Left)
	end

end


-- Walk towards the player.
function WalkTowardsPlayer()

	--rb:SetVel(vec2.new(0, rb:GetVel().y))

	if facingRight == true then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
	else
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
	end

end


-- Check whether this enemy got hurt.
function CheckGetHurt()

	if myCollision:GetColPlayerHitbox() == true
	or myCollision:GetColPlayerDashHitbox() == true
	or myCollision:GetColBigPunchHitbox() == true then
		_G.TotalHits = _G.TotalHits + 1
		states:SetCurrLogicState("HURT")
	end

end


-- Check whether to attack the player based on distance and facing direction.
function CheckWhetherToAttack()

	local distance = myPos - playerPos

	if (facingRight == true and distance > -1.5 and distance < -0.5)
	or (facingRight == false and distance < 1.5 and distance > 0.5) then

		stopRandomising = true

		-- Decide whether to attack or get into counter stance
		if shouldAttack == false then
			states:SetCurrLogicState("COUNTERSTANCE")
		else
			states:SetCurrLogicState("ATTACK")
		end

	else

		stopRandomising = false

	end

end
