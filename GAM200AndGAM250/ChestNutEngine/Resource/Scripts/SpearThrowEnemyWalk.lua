-- Called at first frame.
function Start(sceneactor, player)
	
	-- Get components/Children
	thisActor = sceneactor
	AllComponents = sceneactor:GetPrefab()
	transComp = AllComponents:GetTransformComponent()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
  stats = AllComponents:GetStatsComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myColObj = myCollision:GetCollidedObject()
	sprite = AllComponents:GetSpriteComponent()
	myStats = AllComponents:GetStatsComponent()
	
	wallSensor = sceneactor:GetChild("WallSensor")
	wallSensorColObj = wallSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	ledgeSensor = sceneactor:GetChild("LedgeSensor")
	ledgeSensorColObj = ledgeSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	
	-- Variables for movement and detection
	alertDistance = 20
	attackDistance = 15
	keepAwayDistance = 5
	
	moveSpeed = 2
	movementDir = 1
	selfToPlayer = vec2.new(0, 0)
	platformType = "COLLIDERBOX"
	playerHitboxType = "PLAYER_HIT"

	startPosition = transComp:GetPosition()
	patrolRange = rb:GetModA()

	-- Player ref
	playerTrans = player:GetPrefab():GetTransformComponent()
	
	states:SetCurrDirection(2)
	cornered = false
	dead = false
	
end

-- Called every frame.
function Update()

	FaceMoveDirection()
	
	--Vector to player
	selfToPlayer = playerTrans:GetPosition() - transComp:GetPosition()
	distanceFromPlayer = selfToPlayer:Length()
	CheckCollision()
	
	--If player seen, get into attack range
	if (distanceFromPlayer < alertDistance) then
	
		--If in attack range, attack
		if (distanceFromPlayer < attackDistance) then
		
			--If too close for comfort, back away
			if (distanceFromPlayer < keepAwayDistance) then
			
				if cornered then
				
					FacePlayer()
					myStats:SetCooldown("WAIT_BETWEEN_VOLLEYS", 0)
					AttackBehaviour()
				
				else
				
					BackawayMovement()
				
				end
			
			else
		
				cornered = false
				if myStats:GetCooldown("WAIT_BETWEEN_VOLLEYS") <= 0.0 then
				
					FacePlayer()
					AttackBehaviour()
					
				else
				
					PatrolMovement()
				
				end
			
			end
			
		--If not in attack range, chase
		else
		
			cornered = false
			ChaseMovement()
			
		end
	
	--If player not seen, patrol
	else
	
		cornered = false
		PatrolMovement()
		
	end

end

function AttackBehaviour()

	rb:SetVel(vec2.new(0, 0))
	print("VALK WINDING UP (IDLE)")
	states:SetCurrLogicState("WINDUP")

end

--Patrolling behaviour
function PatrolMovement()

	sprite:SetAnimation("WALK",states)
	rb:SetVel(vec2.new(moveSpeed * movementDir, 0))
	
	--If wall encountered, turn around
	for i = 1, #wallSensorColObj do
		if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			movementDir = movementDir * -1
			ChangeDirection()
			break
		end
	end
	
	groundDetected = false
	
	--If ledge encountered, turn around
	for i = 1, #ledgeSensorColObj do
		if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			groundDetected = true
		end
	end
	--print("I'm turning around!")
	if not groundDetected then
		movementDir = movementDir * -1
		ChangeDirection()
	end
	
	if patrolRange > 0 and (selfToPlayer:Length() > alertDistance) then
	
		if transComp:GetPosition().x > startPosition.x + patrolRange then
		
			if movementDir == 1 then
				movementDir = -1
				ChangeDirection()
			end
		
		elseif transComp:GetPosition().x < startPosition.x - patrolRange then
			
			if movementDir == -1 then
				movementDir = 1
				ChangeDirection()
			end
			
		end
	
	end
	
end

--Chasing behaviour
function ChaseMovement()

	--Always move towards player
	FacePlayer()
	sprite:SetAnimation("WALK",states)
	rb:SetVel(vec2.new(moveSpeed * movementDir, 0))
	
	--If wall encountered, stop
	for i = 1, #wallSensorColObj do
		if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			rb:SetVel(vec2.new(0, 0))
			break
		end
	end
	
	--If ledge encountered, stop
	for i = 1, #ledgeSensorColObj do
		if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			return
		end
	end
	rb:SetVel(vec2.new(0, 0))

end

function BackawayMovement()

	sprite:SetAnimation("WALK",states)
	
	for i = 1, #wallSensorColObj do
		if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			--print("Backing off wall encountered")
			cornered = true
			return
		end
	end
		
	rb:SetVel(vec2.new(moveSpeed * movementDir, 0))
	if (selfToPlayer.x > 0) then
		movementDir = -1
		states:SetCurrDirection(1)
	else
		movementDir = 1
		states:SetCurrDirection(2)
	end
	sprite:SetAnimation("WALK",states)
	cornered = false
		
		--If ledge encountered, stop
	for i = 1, #ledgeSensorColObj do
		if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			return
		end
	end
	--print("Backing off ledge encountered")
	cornered = true

end

function CheckCollision()

	for i = 1, #myColObj do
	
		if myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
		
			myColObj[i]:SetActive(false)
			
			stats:SetHP(stats:GetHP()-1)
			
			if stats:GetHP() == 0 then 
			
				states:SetCurrLogicState("DIE")
			 
			else
			
				states:SetCurrLogicState("HURT")
			
			end
			
			return
			
		end
		
	end
	
end

--Change facing direction
function ChangeDirection()

	if states:GetCurrDirection() == 1 then
		states:SetCurrDirection(2)
	else
		states:SetCurrDirection(1)
	end

end

--Changes facing direction depending on player location
function FacePlayer()

	if (selfToPlayer.x > 0) then
		movementDir = 1
		states:SetCurrDirection(2)
	else
		movementDir = -1
		states:SetCurrDirection(1)
	end
	
end

function FaceMoveDirection()

	if movementDir == 1 then
		states:SetCurrDirection(2)
	else
		states:SetCurrDirection(1)
	end

end

--Converts alpha angle to engine angle value
function RotationConverter(theVector)
	
	alphaAngle = math.atan(theVector.y / theVector.x) * 180 / math.pi
	convertedAngle = 0
	
	if (theVector.x >= 0) then
	
		convertedAngle = 270 + alphaAngle
		
	else
	
		convertedAngle = 90 + alphaAngle
		
	end
	
	return convertedAngle
	
end