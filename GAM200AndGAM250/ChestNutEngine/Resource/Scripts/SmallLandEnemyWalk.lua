-- Called at first frame.
function Start(sceneactor, player)
	
	-- Get components/Children
	thisActor = sceneactor
	AllComponents = sceneactor:GetPrefab()
	transComp = AllComponents:GetTransformComponent()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	sprite = AllComponents:GetSpriteComponent()
    stats = AllComponents:GetStatsComponent()
	myColObj = myCollision:GetCollidedObject()
	
	wallSensor = sceneactor:GetChild("WallSensor")
	wallSensorColObj = wallSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	ledgeSensor = sceneactor:GetChild("LedgeSensor")
	ledgeSensorColObj = ledgeSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	patrolRange = rb:GetModA()
	--[[
	--]]
	
	-- Variables for movement and detection
	alertDistance = 6
	moveSpeed = 1
	movementDir = 1
	ignoreCountdown = 5
	ignoreTimer = 0
	ignoringPlayer = false
	selfToPlayer = vec2.new(0, 0)
	startPosition = transComp:GetPosition()
	platformType = "COLLIDERBOX"
	playerHitboxType = "PLAYER_HIT"

	-- Player ref
	playerTrans = player:GetPrefab():GetTransformComponent()
	dead = false
	
	states:SetCurrDirection(2)
	
end

-- Called every frame.
function Update()

	sprite:SetAnimation("WALK",states)

	selfToPlayer = playerTrans:GetPosition() - transComp:GetPosition()
	distanceFromPlayer = selfToPlayer:Length()

	CheckCollision()

	if distanceFromPlayer < alertDistance and not ignoringPlayer then
		
		ChaseMovement()
		
		ignoreTimer = ignoreTimer + _G.DT
		
		if ignoreTimer >= ignoreCountdown then
		
			ignoringPlayer = true
		
		end
		
	else
	
		PatrolMovement()
		
	end
	
	if ignoringPlayer then
	
		if ignoreTimer > 0 then
		
			ignoreTimer = ignoreTimer - _G.DT
		
		else
		
			ignoringPlayer = false
		
		end
		
	end
		
end

function PatrolMovement()

	rb:SetVel(vec2.new(moveSpeed * movementDir, 0))

	for i = 1, #wallSensorColObj do
		if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			movementDir = movementDir * -1
			ChangeDirection()
			break
		end
	end
	
	groundDetected = false
	
	for i = 1, #ledgeSensorColObj do
		if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			groundDetected = true
		end
	end
	
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

function ChaseMovement()

	FacePlayer()
	rb:SetVel(vec2.new(moveSpeed * movementDir, 0))
	
	for i = 1, #wallSensorColObj do
		if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			rb:SetVel(vec2.new(0, 0))
			--sprite:SetAnimation("STOP",states)
			break
		end
	end
	
	groundDetected = false
	
	for i = 1, #ledgeSensorColObj do
		if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
			groundDetected = true
		end
	end
	
	if not groundDetected then
		rb:SetVel(vec2.new(0, 0))
		--sprite:SetAnimation("STOP",states)
	end

end

function CheckCollision()

	for i = 1, #myColObj do
	
		if myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
		
			myColObj[i]:SetActive(false)
			
			stats:SetHP(stats:GetHP()-1)
			
			if stats:GetHP() == 0 then 
			
				states:SetCurrLogicState("DIE")
			
			else
			
				ignoringPlayer = false
				ignoreTimer = 0
				states:SetCurrLogicState("HURT")
			
			end
			
			return
			
		end
		
	end
	
end

function ChangeDirection()

	if states:GetCurrDirection() == 1 then
		states:SetCurrDirection(2)
	else
		states:SetCurrDirection(1)
	end

end

function FacePlayer()

	if (selfToPlayer.x > 0) then
		movementDir = 1
		states:SetCurrDirection(2)
	else
		movementDir = -1
		states:SetCurrDirection(1)
	end
	
end
