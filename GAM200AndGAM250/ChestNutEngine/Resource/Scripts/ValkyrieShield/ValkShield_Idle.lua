
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
	
	-- Walk Variables
	walkSpeed = 1
	
	-- Idle Variables
	idleMinDuration = 20	-- values are mult by 10 due to math.random only returning integers
	idleMaxDuration = 45	-- values are mult by 10 due to math.random only returning integers
	currentIdleDuration = 0
	idleTimer = 0

	-- Time till next idle
	nextIdle = 5
	nextIdleTimer = 0
	isIdle = false
	
	
	-- Patrol Variables
	patrolDuration = 3.5
	patrolTimer = 0
	patrolRight = true
	
	--Detection Variables
	detectRange = 6
	playerDetected = false
	isDetecting = false

end


-- Called every frame.
function Update()

	TogglePlayerDetection()
	CheckForPlayer()
	DecidePatrolIdle()
	
	if(isIdle) then 
		PatrolIdle()
	else
		Patrol()
	end
	
	ChooseNextState()
	
end

-- Check if the player is in range of enemy
function CheckForPlayer()

	if (isDetecting == false) then
		return
	end
	
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

-- Enable valkyrie to detect player character,
-- Currently used to debug this state.aw
function TogglePlayerDetection()

	if (_G.i:KeyPrev(CVK_0)) then
		isDetecting = not isDetecting
		print("valk is detecting: ", isDetecting) 
	end
end

function Patrol()

	-- Increment timer
	patrolTimer = patrolTimer + _G.DT
	
	-- Change direction once timer exceeds
	if(patrolTimer > patrolDuration) then
		patrolTimer = 0
		patrolRight = not patrolRight
	end
	
	if(patrolRight) then
		-- Move right
		rb:SetVel(vec2.new(walkSpeed, 0))
		states:SetCurrDirection(Direction.Right)
		--sprite:SetAnimation("DASH",states)

	else
		-- Move left
		rb:SetVel(vec2.new(-walkSpeed, 0))
		states:SetCurrDirection(Direction.Left)
		--sprite:SetAnimation("DASH",states)

	end
end

--Decide if enemy will enter idle
function DecidePatrolIdle()

	-- Increment timer if currently not idle
	if(isIdle == false)then
		nextIdleTimer = nextIdleTimer + _G.DT
	end
	
	-- Enable idle if timer exceeds duration
	if(nextIdleTimer > nextIdle) then
		velocity = rb:GetVel()
		-- Set velocity to zero
		rb:SetVel(vec2.new(0, velocity.y))
		nextIdleTimer =  0
		isIdle = true
		-- Generate idle duration
		currentIdleDuration = (math.random (idleMinDuration, idleMaxDuration)) / 10
	end
	
end

-- Enemy will pause and idle randomly during their patrol
function PatrolIdle()
	
	idleTimer = idleTimer + _G.DT
	
	if(idleTimer > currentIdleDuration) then
		idleTimer = 0
		currentIdleDuration = 0
		isIdle = false
	end

end


function ChooseNextState()
	
	
	if (playerDetected) then
		-- Reset velocity to zero
		rb:SetVel(vec2.new(0, 0))
		states:SetCurrLogicState("CHASE")
	end
	
end

