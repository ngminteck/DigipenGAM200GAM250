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
	cornered = false
	dead = false
	
	windupDuration = 0.23335
	windupTimer = 0
	pauseDuration = 1
	pauseTimer = 0
	
	windupFX = sceneactor:GetChild("EnemyWindupFX")
	windupFXDuration = (0.05 * 9 * 2) - 0.01
	windupFXTimer = 0
	
end

-- Called every frame.
function Update()
	
	for i = 1, #mySpears do
	
		mySpears[i]:SetActive(false)
	
	end
	if windupFXTimer < windupFXDuration then
	
		windupFX:SetActive(true)
		windupFXTimer = windupFXTimer + _G.DT
		
		if windupFXTimer >= windupFXDuration then
		
			windupFX:SetActive(false)
		
		end
	
	end
	
	if windupTimer < windupDuration then
	
		sprite:SetAnimation("WINDUP",states)
		windupTimer = windupTimer + _G.DT
		
	else
	
		if pauseTimer < pauseDuration then
		
			sprite:SetAnimation("WINDUPPAUSE",states)
			pauseTimer = pauseTimer + _G.DT
			
		else
		
			windupTimer = 0
			pauseTimer = 0
			windupFXTimer = 0
			windupFX:SetActive(false)
			--myStats:SetCooldown("WAIT_BETWEEN_VOLLEYS", 0)
			states:SetCurrLogicState("THROW")
		
		end
		
	end

	CheckCollision()

end

function CheckCollision()

	for i = 1, #myColObj do
	
		if myColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == playerHitboxType then
		
			myColObj[i]:SetActive(false)
			
			stats:SetHP(stats:GetHP()-1)
			
			if stats:GetHP() == 0 then 
				
				windupFX:SetActive(false)
				states:SetCurrLogicState("DIE")
			 
			else
			
				windupTimer = 0
				pauseTimer = 0
				windupFXTimer = 0
				windupFX:SetActive(false)
				myStats:SetCooldown("WAIT_BETWEEN_VOLLEYS", 0)
				states:SetCurrLogicState("HURT")
			
			end
			
			return
			
		end
		
	end
	
end