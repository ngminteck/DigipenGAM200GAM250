--[[

This script controls the idle of the character.

Possible states this script can transition to:
- Walk [untested]
- Hurt

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myTransform = AllComponents:GetTransformComponent()

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()

	-- Variables for managing patrol
	walkTimePassed = 0
	walkDuration = 0.2
	pauseTimePassed = 0
	pauseDuration = 0.5
	facingRight = false

	-- Variables for controlling walk
	speed = 4

end


-- Called every frame.
function Update()

	Walk()
	CheckPlayer()

end


-- The enemy walks as part of patrolling.
function Walk()

	walkTimePassed = walkTimePassed + _G.DT

	if walkTimePassed <= walkDuration then

		if facingRight == true then
			rb:SetVel(vec2.new(speed, rb:GetVel().y))
			states:SetCurrDirection(Direction.Right)
		else
			rb:SetVel(vec2.new(-speed, rb:GetVel().y))
			states:SetCurrDirection(Direction.Left)
		end

	else

		Wait()

	end

end


-- The enemy waits as part of patrolling.
function Wait()

	pauseTimePassed = pauseTimePassed +_G.DT

	if pauseTimePassed <= pauseDuration then

		rb:SetVel(vec2.new(0, rb:GetVel().y))

	else

		-- Change facing direction.
		if facingRight == true then
			facingRight = false
		else
			facingRight = true
		end

		-- Reset all timing
		walkTimePassed = 0
		pauseTimePassed = 0

	end

end


-- Check if the player is nearby.
function CheckPlayer()

	if myCollision:DetectInRadius(50, playerTransform, myTransform) == true then
	
		if facingRight == true then
			states:SetCurrDirection(Direction.Right)
		else
			states:SetCurrDirection(Direction.Left)
		end
		
		states:SetCurrLogicState("WALK")
		
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
