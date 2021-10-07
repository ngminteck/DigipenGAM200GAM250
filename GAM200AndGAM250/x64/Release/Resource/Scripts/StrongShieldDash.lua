--[[

This script controls this enemy's dash.

Possible states this script can transition to:
- Walk [DONE]
- Attack [DONE]

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components and children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myTransform = AllComponents:GetTransformComponent()
	myPos = myTransform:GetPosition().x

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	playerPos = playerTransform:GetPosition().x

	-- Variables for dash
	timePassedForDashing = 0
	dashMaxDuration = 0.5
	speed = 20
	facingRight = true
	
	-- Manage direction-change to disable direction-change mid-dash
	alreadyDirectioned = false

end


-- Called every frame.
function Update()
	
	UpdatePosition()
	
	if alreadyDirectioned == false then
		FacePlayerDirection()
	end
		
	DashTowardsPlayer()
	EndDash()

end


-- Update the player's own position
function UpdatePosition()

	myPos = myTransform:GetPosition().x
	playerPos = playerTransform:GetPosition().x

end


-- Face the direction where player is.
function FacePlayerDirection()

	if myPos < (playerPos - 5) then
		facingRight = true
		states:SetCurrDirection(Direction.Right)
	end

	if myPos > (playerPos + 5) then
		facingRight = false
		states:SetCurrDirection(Direction.Left)
	end
	
	alreadyDirectioned = true

end


-- Dash towards the player.
function DashTowardsPlayer()

	if facingRight == true then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
	else
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
	end
	
end


-- Check for the conditions to end the dash.
function EndDash()

	-- After 1 second of unproductive dashing, revert to walking.
	timePassedForDashing = timePassedForDashing + _G.DT
	if timePassedForDashing >= dashMaxDuration then

		timePassedForDashing = 0
		alreadyDirectioned = false
		states:SetCurrLogicState("WALK")

	end

	-- If the enemy gets close to the player, stop dashing and proceed to bashing.
	local distance = myPos - playerPos
	if distance > - 1.5 and distance < 1.5 then

		alreadyDirectioned = false
		states:SetCurrLogicState("BASH")

	end

end
