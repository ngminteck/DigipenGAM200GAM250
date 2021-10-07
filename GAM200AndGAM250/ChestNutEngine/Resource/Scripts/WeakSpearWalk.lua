--[[

This script controls the enemy's walk.

Possible states this script can transition to:
- Attack [DONE]
- Hurt [DONE]

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
	mySceneActor = sceneactor

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	playerPos = playerTransform:GetPosition().x

	-- Variables for controlling walk
	speed = 4
	facingRight = true

end


-- Called every frame.
function Update()

	UpdatePosition()
	FacePlayerDirection()
	WalkTowardsPlayer()
	CheckWhetherToAttack()
	CheckGetHurt()

end


-- Update the player's own position
function UpdatePosition()

	myPos = myTransform:GetPosition().x
	playerPos = playerTransform:GetPosition().x

end


-- Face the direction where player is.
function FacePlayerDirection()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if myPos < (playerPos - 5) then
		facingRight = true
		states:SetCurrDirection(Direction.Right)
	end

	if myPos > (playerPos + 5) then
		facingRight = false
		states:SetCurrDirection(Direction.Left)
	end

end


-- Walk towards the player.
function WalkTowardsPlayer()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if facingRight == true then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
	else
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
	end

end


-- Check whether to attack the player based on distance and facing direction.
function CheckWhetherToAttack()

	local distance = myPos - playerPos

	if (facingRight == true and distance > -1.5 and distance < -0.5)
	or (facingRight == false and distance < 1.5 and distance > 0.5) then
		states:SetCurrLogicState("ATTACK")
	end

end


-- Check whether this enemy got hurt.
function CheckGetHurt()

	if myCollision:GetColPlayerHitbox() == true
	or myCollision:GetColPlayerDashHitbox() == true
	or myCollision:GetColBigPunchHitbox() == true then
 
		-- Invulnerability
		if mySceneActor:GetInvul() == false then
			mySceneActor:SetInvul(true)
			mySceneActor:SetTimer(0.0)
		else
			mySceneActor:IncTimer()
			if mySceneActor:GetTimer() >= 2 then
				mySceneActor:SetInvul(false)
			end
		end
		
		_G.TotalHits = _G.TotalHits + 1
		states:SetCurrLogicState("HURT")
		
	end
	
end
