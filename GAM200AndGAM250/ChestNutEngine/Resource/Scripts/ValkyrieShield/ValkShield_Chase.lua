
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
	
	-- Chase Variables
	facingRight = false
	chaseSpeed = 4
	
	-- Detection Variables
	detectRange = 6
	playerDetected = false
	isDetecting = false
	
	
end


-- Called every frame.
function Update()
	
	CheckForPlayer()
	
	FindPlayer()
	ChooseNextState()

end

-- Check for current direction of enemy.
function CheckDirection()
	direction = rb:GetVel()
	
	if(direction.x > 0) then
		facingRight = true
	else
		facingRight = false
	end
end

-- Check if the player is in range of enemy
function CheckForPlayer()
	-- Reset velocity to zero
	rb:SetVel(vec2.new(0, 0))
	-- Get my position
	myPos = myTransform:GetPosition()
	-- Get player position
	playerPos = playerTransform:GetPosition()
	
	-- Check distance between you and player
	distance = (myPos - playerPos):Length()
	
	if(distance < detectRange) then
		playerDetected = true
	else
		playerDetected = false
	end
	
end



-- Determine direction of player to enemy
function FindPlayer()
	playerDirection = playerTransform:GetPosition()- myTransform:GetPosition()
	print("Chasing Player")
	ChasePlayer(playerDirection)
end

-- Makes enemy chase player
-- **Currently only chase player horizontally but but vertically
function ChasePlayer(_direction)

	if(_direction.x > 0) then
		rb:SetVel(vec2.new(chaseSpeed,0))
	else
		rb:SetVel(vec2.new(-chaseSpeed,0))	
	end
end

function ChooseNextState()
	
	if (playerDetected == false) then
		states:SetCurrLogicState("IDLE")
	else
		rb:SetVel(vec2.new(0,0))
		states:SetCurrLogicState("ATTACK")
	end
	
end