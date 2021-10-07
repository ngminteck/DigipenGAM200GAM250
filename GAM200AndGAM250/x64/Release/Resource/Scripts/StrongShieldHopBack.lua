--[[

This script controls this enemy hopping back.

Possible states this script can transition to:
- Walk [untested]

]]--


-- Called at first frame.
function Start(sceneactor,player)

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
	
	-- Variables for player direction.
	playerAtRight = false
	alreadyFaced = false

	-- Variables for managing jump
	jumpTimeLimit = 0.25
	timePassed = 0

	-- Variables for controlling jump
	jumpHeight = 5
	
	-- Variables for controlling walk
	speed = 8
	
	-- Variables to check for state change
	hasJumped = false

end


-- Called every frame.
function Update()

	if alreadyFaced == false then
		GetPlayerDirection()
		alreadyFaced = true
	end
	
	Jump()
	MoveBack()
	CheckEndState()

end


-- Get the player's direction with respect to the attached object.
function GetPlayerDirection()

	if myPos < playerPos then
		playerAtRight = true
	end

	if myPos > playerPos then
		playerAtRight = false
	end

end


-- The jump aspect of hopping back.
function Jump()

	timePassed = timePassed + _G.DT

	if timePassed <= jumpTimeLimit then
		rb:SetVel(vec2.new(0, jumpHeight))
		hasJumped = true
	end

end


-- The moving back aspect of hopping back.
function MoveBack()

	if playerAtRight == true then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
	else
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
	end

end


-- Check whether it is time to end the state.
function CheckEndState()

	if myCollision:GetColBottom() == true and hasJumped == true then
		
		hasJumped = false
		timePassed = 0
		alreadyFaced = false
		
		states:SetCurrLogicState("WALK")
		
	end

end
