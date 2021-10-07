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
	
	wallSensor = sceneactor:GetChild("WallSensor")
	wallSensorColObj = wallSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	ledgeSensor = sceneactor:GetChild("LedgeSensor")
	ledgeSensorColObj = ledgeSensor:GetPrefab():GetCollisionComponent():GetCollidedObject()
	mySpears = 		{ sceneactor:GetChild("Spear 1"), sceneactor:GetChild("Spear 2"),
					  sceneactor:GetChild("Spear 3"), sceneactor:GetChild("Spear 4"),
					  sceneactor:GetChild("Spear 5") }
	
	-- Variables for movement and detection
	alertDistance = 20
	attackDistance = 15
	keepAwayDistance = 5
	--corneredDistance = 5
	moveSpeed = 2
	movementDir = 1
	selfToPlayer = vec2.new(0, 0)
	platformType = "COLLIDERBOX"
	playerHitboxType = "PLAYER_HIT"

	-- Variables for spear throwing
	spearLifeTime = 2.5
	spearSpeed = 6
	throwDelay = 2
	throwDelayTimer = 0
	throwWait = 5
	throwWaitTimer = 0
	throwWindupDuration = 0.5
	throwWindupTimer = 0
	nSpearsPerVolley = 3
	nSpearsThrown = 0
	spearVectors = { vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0) }
	spearTimers = { spearLifeTime, spearLifeTime, spearLifeTime, spearLifeTime, spearLifeTime }
	activeSpears = { false, false, false, false, false }
	spearsInMotion = 0

	startPosition = transComp:GetPosition()
	patrolRange = rb:GetModA()

	animationTimer = 0
	animToPlay = "IDLE"
	animAtEnd = "IDLE"
	animSet = false

	-- Player ref
	playerTrans = player:GetPrefab():GetTransformComponent()
	
	states:SetCurrDirection(2)
    sprite:SetAnimation("IDLE",states)
	cornered = false
	dead = false
	
end

-- Called every frame.
function Update()

	if not dead then
		--Vector to player
		selfToPlayer = playerTrans:GetPosition() - transComp:GetPosition()
		distanceFromPlayer = selfToPlayer:Length()
		
		SpearHandling()
		
		if animationTimer > 0 then
		
			AnimationHandling()
		
		end
		
		CheckCollision()
		
		--If player seen, get into attack range
		if (distanceFromPlayer < alertDistance) then
		
			--If in attack range, attack
			if (distanceFromPlayer < attackDistance) then
			
				--If too close for comfort, back away
				if (distanceFromPlayer < keepAwayDistance) then
				
					if cornered then
					
						FacePlayer()
						AttackBehaviour()
					
					else
					
						BackawayMovement()
					
					end
				
				else
			
					cornered = false
					if throwWaitTimer <= 0 then
					
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
		
		--spearColObj = mySpears[1]:GetPrefab():GetCollisionComponent():GetCollidedObject()
		--print(#spearColObj)
			--[[
		for i = 1, #spearColObj do
			if spearColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
				print("Hit a player hitbox")
			end
			print(i.." "..spearColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType())
		end
			--]]
		--[[
		--]]
		--print("Update ended")
		--[[print("==================================")
		for i = 1, #activeSpears do
		
			print(activeSpears[i])
		
		end--]]
		
	end

end

--Patrolling behaviour
function PatrolMovement()

	if spearsInMotion == 0 then
	
		sprite:SetAnimation("WALK",states)
		rb:SetVel(vec2.new(moveSpeed * movementDir, 0))
		
		if patrolRange > 0 then
		
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
	
	end
	
end

--Chasing behaviour
function ChaseMovement()

	if spearsInMotion == 0 then
	
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

end

function BackawayMovement()

	if spearsInMotion == 0 then
		
		sprite:SetAnimation("WALK",states)
		
		for i = 1, #wallSensorColObj do
			if wallSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
				print("Backing off wall encountered")
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
		sprite:SetAnimation("IDLE",states)
		cornered = false
			
			--If ledge encountered, stop
		for i = 1, #ledgeSensorColObj do
			if ledgeSensorColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
				return
			end
		end
		print("Backing off ledge encountered")
		cornered = true
	
	end

end

--Spear-throwing behaviour
function AttackBehaviour()

	if spearsInMotion == 0 then FacePlayer() end
	
	--sprite:SetAnimation("WINDUPPAUSE",states)
	rb:SetVel(vec2.new(0, 0))
	--If ready to throw spear, find a spear not in motion, set vector and angle, and throw
	if throwDelayTimer <= 0 then
		
		for i = 1, #activeSpears do
		
			if activeSpears[i] == false then
				
				if not animSet then
					SetAnimationTimer("WINDUP", 0.122, "WINDUPPAUSE")
					animSet = true
					return
				end
				
				if animationTimer > 0 then return end
				
				SetAnimationTimer("THROW", 0.17, "IDLE")
				
				animSet = false
				
				spearsInMotion = spearsInMotion + 1
				
				if states:GetCurrDirection() == 2 then
					spearVectors[i] = vec2.new(-selfToPlayer.x, selfToPlayer.y) / selfToPlayer:Length()
				else
					spearVectors[i] = vec2.new(selfToPlayer.x, selfToPlayer.y) / selfToPlayer:Length()
				end
				
				mySpears[i]:GetPrefab():GetTransformComponent():SetRotate(RotationConverter(selfToPlayer))
				mySpears[i]:SetActive(true)
				activeSpears[i] = true
				throwDelayTimer = throwDelay
				nSpearsThrown = nSpearsThrown + 1
				if nSpearsThrown == nSpearsPerVolley then
				
					nSpearsThrown = 0
					throwWaitTimer = throwWait
					
				end
				
				break
			
			end
			
		end
	
	end

end

--Runs every frame, handles spear activity
function SpearHandling()

	if (spearsInMotion > 0) then
	
		for i = 1, #activeSpears do
		
			if activeSpears[i] == true then
			
				--spear collision
				spearColObj = mySpears[i]:GetPrefab():GetCollisionComponent():GetCollidedObject()
				for j = 1, #spearColObj do
					if spearColObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType or
					   spearColObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType then
						--print(i.." Hit a player hitbox")
						spearTimers[i] = 0
					end
				end
				
				--Moves spears and checks for lifetime
				spearTimers[i] = spearTimers[i] - _G.DT
				mySpears[i]:GetPrefab():GetAABBComponent():SetOffset(spearVectors[i] * spearSpeed * (spearLifeTime - spearTimers[i]))
				
				--At end of spear life, deactivate spear
				if spearTimers[i] <= 0 then
				
					activeSpears[i] = false
					mySpears[i]:GetPrefab():GetAABBComponent():SetOffset(vec2.new(0, 0))
					spearTimers[i] = spearLifeTime
					mySpears[i]:SetActive(false)
					spearsInMotion = spearsInMotion - 1
				
				end
			
			end
		
		end
	
	end
	
	if throwDelayTimer > 0 then
	
		throwDelayTimer = throwDelayTimer - _G.DT
		
	end
	
	if throwWaitTimer > 0 then
	
		throwWaitTimer = throwWaitTimer - _G.DT
		
	end

end

function AnimationHandling()

	if animationTimer > 0 then
	
		sprite:SetAnimation(animToPlay, states)
		animationTimer = animationTimer - _G.DT
	
		if animationTimer <= 0 then
		
			sprite:SetAnimation(animAtEnd, states)
		
		end
	
	else
	
		sprite:SetAnimation(animAtEnd, states)
	
	end

end

function SetAnimationTimer(playAnim, duration, endAnim)

	animToPlay = playAnim
	animationTimer = duration
	animAtEnd = endAnim

end

function CheckCollision()

	for i = 1, #myColObj do
	
		if myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
			
      if stats:GetHP() == 0 then 
			  thisActor:SetActive(false)
			  dead = true
      else
        stats:SetHP(stats:GetHP()-1)
			end
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
    --sprite:SetAnimation("THROW",states)

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
    --sprite:SetAnimation("THROW",states)
	
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