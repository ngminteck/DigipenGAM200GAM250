
-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	thisActor = sceneactor
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	sprite = AllComponents:GetSpriteComponent()
	transComp = AllComponents:GetTransformComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()

	playerTrans = player:GetPrefab():GetTransformComponent()

	idleSprite = "SAW_SPAWNER_FACEDOWN"
	firingSprite = "SAW_SPAWNER_FIRE_FACEDOWN"
	shootAnimDuration = (0.016 * 6) - 0.01
	animToPlay = idleSprite
	animationTimer = 0
	animAtEnd = idleSprite
    
	shootAudioPath = "Resource/Audio/SawSpawnerShoot.wav"
	windupAudioPath = "Resource/Audio/SawSpinning.wav"
	deathAudioPath = "Resource/Audio/BossBurst.wav"
	selfToPlayer = vec2.new(0, 0)
	distanceFromPlayer = 0
	playSoundDistance = 15.5
	windupAudioPlayed = false
	deathAudioPlayed = false
	
	--timeBetweenVolleys = rb:GetStartTime()
	timeBetweenVolleys = 2
	volleyTimer = 0
	
	timeBetweenSaws = 0.25
	betweenSawTimer = 0
	
	windupFX = sceneactor:GetChild("EnemyWindupFX")
	deathFX = sceneactor:GetChild("Explosion")
	FXTimeBeforeFiring = 0.5
	deathFXDuration = (0.06 * 9) - 0.01
	deathFXTimer = 0
	
	--volleySize = math.floor(rb:GetModA())
	volleySize = 3
	if rb:GetModA() > 0 then
	
		volleySize = math.floor(rb:GetModA())
	
	end
	volleyProgress = 0
	
    sawSpeed = rb:GetModB()
	
	if sawSpeed == 0 then
	
		sawSpeed = 0.25
		
	end
	
	--sawLifetime = rb:GetDuration()
	sawLifetime = 2.5
	sawDirection = rb:GetDir() / rb:GetDir():Length()
	sawStartOffset = 1.1 * sawDirection
	
	mySaws = 		{ sceneactor:GetChild("Saw 1"), sceneactor:GetChild("Saw 2"),
					  sceneactor:GetChild("Saw 3"), sceneactor:GetChild("Saw 4"),
					  sceneactor:GetChild("Saw 5") }
					  
    sawVectors = { sawDirection, sawDirection, sawDirection, sawDirection, sawDirection }
	sawTimers = { sawLifetime, sawLifetime, sawLifetime, sawLifetime, sawLifetime }
	activeSaws = { false, false, false, false, false }
	deflectedSaws = { false, false, false, false, false }
	
	dead = false
  
    for i = 1, #mySaws do
	
		mySaws[i]:SetActive(false)
		mySaws[i]:GetPrefab():GetAABBComponent():SetOffset(sawStartOffset)
	
	end
  
end


function Update()
	
	sprite:SetAnimation("SAW_SPAWNER_FACEDOWN", states)
	selfToPlayer = playerTrans:GetPosition() - transComp:GetPosition()
	distanceFromPlayer = selfToPlayer:Length()
	
	if not dead then
		AnimationHandling()
		FireSaws()
	end
	
	SawHandling()

	if deathFXTimer > 0 then
	
		deathFXTimer = deathFXTimer - _G.DT
		deathFX:SetActive(true)
		
		if not deathAudioPlayed then
		
			_G.audio:Play(deathAudioPath, vec3.new(0,0,0), 0.0, false)
			deathAudioPlayed = true
		
		end
	
	else
	
		deathFX:SetActive(false)
	
	end
  
end


function FireSaws()

	if volleyTimer <= 0 then
	
		if betweenSawTimer <= 0 then
		
			windupFX:SetActive(false)
			windupAudioPlayed = false
			
			for i = 1, #activeSaws do
			
				if not activeSaws[i] then
					
					if distanceFromPlayer < playSoundDistance then
					
						_G.audio:Play(shootAudioPath, vec3.new(0,0,0), 0.0, false)
					
					end
					
					InternalAnimSet(firingSprite, shootAnimDuration, idleSprite)
					activeSaws[i] = true
					betweenSawTimer = timeBetweenSaws
					volleyProgress = volleyProgress + 1
					break
					
				end
			
			end
			
			if volleyProgress >= volleySize then
		
				volleyTimer = timeBetweenVolleys
				volleyProgress = 0
			
			end
		
		else

			betweenSawTimer = betweenSawTimer - _G.DT
		
		end
	
	else
	
		volleyTimer = volleyTimer - _G.DT
		
		if volleyTimer <= FXTimeBeforeFiring then
			
			if not windupAudioPlayed and distanceFromPlayer < playSoundDistance then
			
				_G.audio:Play(windupAudioPath, vec3.new(0,0,0), 0.0, false)
				windupAudioPlayed = true
			
			end
			
			windupFX:SetActive(true)
			InternalAnimSet(firingSprite, FXTimeBeforeFiring, idleSprite)
			
		end
		
	end

end


function SawHandling()
	
	for i = 1, #activeSaws do
			
		if activeSaws[i] then
			
			mySaws[i]:SetActive(true)
			sawTimers[i] = sawTimers[i] - _G.DT
			mySaws[i]:GetPrefab():GetAABBComponent():SetOffset(mySaws[i]:GetPrefab():GetAABBComponent():GetOffset() + sawVectors[i] * sawSpeed)
			
			colObj = mySaws[i]:GetPrefab():GetCollisionComponent():GetCollidedObject()
			
			for j = 1, #colObj do
			
				collisionType = colObj[j]:GetPrefab():GetRigidBodyComponent():GetObjType()
				
				if collisionType == "COLLIDERBOX" then
				
					sawTimers[i] = 0
					if colObj[j] == thisActor then
						dead = true
						deathFXTimer = deathFXDuration
						thisActor:SetActive(false)
					end
				
				elseif collisionType == "PLAYER_HIT" then
				
					collisionType = colObj[j]:SetActive(false)
					sawVectors[i] = -sawVectors[i]
					deflectedSaws[i] = true
				
				end
			
			end
			
			if sawTimers[i] <= 0 then
			
				mySaws[i]:GetPrefab():GetAABBComponent():SetOffset(sawStartOffset)
				sawVectors[i] = sawDirection
				deflectedSaws[i] = false
				mySaws[i]:SetActive(false)
				activeSaws[i] = false
				sawTimers[i] = sawLifetime
			
			end
		
		end
			
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

end

function CheckCollision()

	for i = 1, #colObj do
	
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
		
			for j = 1, #deflectedSaws do
			
				if deflectedSaws[j] then
					

					if colObj[i] == mySaws[j] then
					
						--print("DESTROYED")
						
					end
				
				end
			
			end
		
		end
	
	end

end