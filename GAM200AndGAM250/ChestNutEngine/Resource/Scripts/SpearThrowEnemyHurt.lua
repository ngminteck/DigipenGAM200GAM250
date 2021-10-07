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

	-- Player ref
	playerTrans = player:GetPrefab():GetTransformComponent()
	
	states:SetCurrDirection(2)
	cornered = false
	dead = false
	
	animDuration = 0.44
	animTimer = 0
	
	slashFX = sceneactor:GetChild("EnemySlashFX")
	sparksFX = sceneactor:GetChild("EnemyHitSparksFX")
	
	FXDuration = (0.064 * 3) - 0.01
	FXTimer = 0
	
end

-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, 0))
	
	for i = 1, #mySpears do
	
		mySpears[i]:SetActive(false)
	
	end
	
	if animTimer < animDuration then
	
		sprite:SetAnimation("HURT",states)
		animTimer = animTimer + _G.DT
		
	else
	
		--myStats:SetCooldown("WAIT_BETWEEN_VOLLEYS", 0)
		animTimer = 0
		--print("VALK WINDING UP (HURT)")
		slashFX:SetActive(false)
		sparksFX:SetActive(false)
		FXTimer = 0
		states:SetCurrLogicState("IDLE")
		return
	
	end
	
	if FXTimer < FXDuration then
	
		slashFX:SetActive(true)
		sparksFX:SetActive(true)
		FXTimer = FXTimer + _G.DT
		
	else
	
		slashFX:SetActive(false)
		sparksFX:SetActive(false)
	
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
			
				animTimer = 0
				states:SetCurrLogicState("HURT")
			
			end
			
			return
			
		end
		
	end
	
end