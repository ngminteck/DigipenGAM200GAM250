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
	playerType = "PLAYER"

	-- Variables for spear throwing
	spearLifeTime = 2.5
	spearSpeed = 6
	throwElevation = 0.5
	
	throwDelay = 2.55
	throwDelayTimer = 0
	throwWait = 5
	throwWaitTimer = 0
	nSpearsPerVolley = 3
	nSpearsThrown = 0
	
	spearVectors = { vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0), vec2.new(0, 0) }
	spearTimers = { spearLifeTime, spearLifeTime, spearLifeTime, spearLifeTime, spearLifeTime }
	activeSpears = { false, false, false, false, false }
	spearsInMotion = 0
	myStats:RegisterCooldown("WAIT_BETWEEN_VOLLEYS", throwWait, throwWait, true)

	startPosition = transComp:GetPosition()
	patrolRange = rb:GetModA()

	animationTimer = 0
	animToPlay = "IDLE"
	animAtEnd = "IDLE"
	animSet = false
	throwAnimLength = 0.34
	spearBreakDuration = 0.15

	-- Player ref
	playerTrans = player:GetPrefab():GetTransformComponent()
	
	states:SetCurrDirection(2)
    sprite:SetAnimation("IDLE",states)
	cornered = false
	dead = false
	
	for i = 1, #mySpears do
	
		mySpears[i]:SetActive(false)
		mySpears[i]:GetPrefab():GetAABBComponent():SetOffset(vec2.new(0, throwElevation))
		
	end
	
	throwFX = sceneactor:GetChild("EnemyThrowFX")
	throwFXDuration = (0.05 * 7) - 0.01
	throwFXTimer = 0
	
end

-- Called every frame.
function Update()
	
	CheckCollision()
	--Vector to player
	selfToPlayer = playerTrans:GetPosition() - transComp:GetPosition()
	distanceFromPlayer = selfToPlayer:Length()
	
	AnimationHandling()
	
	AttackBehaviour()
	SpearHandling()

			
end

--Spear-throwing behaviour
function AttackBehaviour()

	if spearsInMotion == 0 then FacePlayer() end
	
	rb:SetVel(vec2.new(0, 0))
	--If ready to throw spear, find a spear not in motion, set vector and angle, and throw
	if throwDelayTimer <= 0 and myStats:GetCooldown("WAIT_BETWEEN_VOLLEYS") <= 0.0 then
	
		for i = 1, #activeSpears do
		
			if activeSpears[i] == false then
				
				--print("THROW ANIM")
				
				InternalAnimSet("THROW", throwAnimLength, "IDLE")
				spearsInMotion = spearsInMotion + 1
				
				spearDirection = selfToPlayer - vec2.new(0, throwElevation)
				
				if states:GetCurrDirection() == 2 then
					spearVectors[i] = vec2.new(-spearDirection.x, spearDirection.y) / spearDirection:Length()
				else
					spearVectors[i] = vec2.new(spearDirection.x, spearDirection.y) / spearDirection:Length()
				end
				mySpears[i]:GetPrefab():GetTransformComponent():SetRotate(RotationConverter(spearDirection))
				mySpears[i]:SetActive(true)
				activeSpears[i] = true
				
				throwFX:GetPrefab():GetTransformComponent():SetRotate(RotationConverter(spearDirection) - 90)
				throwFXTimer = throwFXDuration
				throwDelayTimer = throwDelay
				nSpearsThrown = nSpearsThrown + 1
				
				if nSpearsThrown == nSpearsPerVolley then
				
					myStats:ResetCooldown("WAIT_BETWEEN_VOLLEYS")
					myStats:ActivateCooldown("WAIT_BETWEEN_VOLLEYS", true)
					nSpearsThrown = 0
					
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
				
				--Moves spears and checks for lifetime
				spearTimers[i] = spearTimers[i] - _G.DT
				
				--spear collision
				spearColObj = mySpears[i]:GetPrefab():GetCollisionComponent():GetCollidedObject()
				for j = 1, #spearColObj do
					if spearTimers[i] > 0 and
					   (spearColObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType or
					   spearColObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType() == platformType or
					   spearColObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerType) then
						spearTimers[i] = 0
					end
				end
				
				--At end of spear life, deactivate spear
				if spearTimers[i] <= 0 then
				
					mySpears[i]:GetPrefab():GetSpriteComponent():SetAnimation("SPEAR_BREAK",
					mySpears[i]:GetPrefab():GetMovementComponent())
				
					if spearTimers[i] <= -spearBreakDuration then
						
						activeSpears[i] = false
						mySpears[i]:GetPrefab():GetSpriteComponent():SetAnimation("SPEAR",
						mySpears[i]:GetPrefab():GetMovementComponent())
						mySpears[i]:GetPrefab():GetAABBComponent():SetOffset(vec2.new(0, throwElevation))
						spearTimers[i] = spearLifeTime
						mySpears[i]:SetActive(false)
						spearsInMotion = spearsInMotion - 1
					
					end
				
				else
				
					mySpears[i]:GetPrefab():GetAABBComponent():SetOffset((spearVectors[i] * spearSpeed * (spearLifeTime - spearTimers[i])) + vec2.new(0, throwElevation))
				
				end
			
			end
		
		end
	
	end
	
	if throwDelayTimer > 0 then
	
		throwDelayTimer = throwDelayTimer - _G.DT
	
		if throwDelayTimer <= 0 and myStats:GetCooldown("WAIT_BETWEEN_VOLLEYS") <= 0.0 then
	
			--print("VALK WINDING UP (THROW)")
			throwFXTimer = 0
			throwFX:SetActive(false)
			states:SetCurrLogicState("WINDUP")
		
		end
		
	end
	
	if myStats:GetCooldown("WAIT_BETWEEN_VOLLEYS") > 0.0 and spearsInMotion == 0 then
	
		throwDelayTimer = 0
		throwFXTimer = 0
		throwFX:SetActive(false)
		states:SetCurrLogicState("IDLE")
	
	end

end

function InternalAnimSet(playAnim, timer, endAnim)

	animToPlay = playAnim
	animationTimer = timer
	animAtEnd = endAnim

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
	
	if throwFXTimer > 0 then
	
		throwFX:SetActive(true)
		throwFXTimer = throwFXTimer - _G.DT
		
		if throwFXTimer <= 0 then
		
			throwFX:SetActive(false)
		
		end
	
	end

end

function CheckCollision()

	for i = 1, #myColObj do
	
		if myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
		
			myColObj[i]:SetActive(false)
			
			CancelSpears()
			stats:SetHP(stats:GetHP()-1)
			
			if stats:GetHP() == 0 then 
				
				throwFXTimer = 0
				throwFX:SetActive(false)
				states:SetCurrLogicState("DIE")
			 
			else
				
				throwFXTimer = 0
				throwFX:SetActive(false)
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

function CancelSpears()

	for i = 1, #mySpears do
	
		activeSpears[i] = false
		mySpears[i]:GetPrefab():GetSpriteComponent():SetAnimation("SPEAR",
		mySpears[i]:GetPrefab():GetMovementComponent())
		mySpears[i]:GetPrefab():GetAABBComponent():SetOffset(vec2.new(0, throwElevation))
		spearTimers[i] = spearLifeTime
		mySpears[i]:SetActive(false)
		spearsInMotion = 0
	
	end
	
	throwDelayTimer = 0
	myStats:SetCooldown("WAIT_BETWEEN_VOLLEYS", 0)
	nSpearsThrown = 0

end