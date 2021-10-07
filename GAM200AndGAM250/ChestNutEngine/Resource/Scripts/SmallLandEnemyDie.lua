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
	
	states:SetCurrDirection(2)
	
	animDuration = (0.05 * 8) - 0.02
	animTimer = 0
	
	slashFX = sceneactor:GetChild("EnemySlashFX")
	sparksFX = sceneactor:GetChild("EnemyHitSparksFX")
	deathFX = sceneactor:GetChild("EnemyDeathFX")
	deathFXDuration = (0.05 * 13) - 0.01
	deathFXTimer = 0
	
end

-- Called every frame.
function Update()
	
	rb:SetVel(vec2.new(0, 0))
	rb:SetGravity(0)
	rb:SetHasGravity(false)
	rb:SetObjType("TRIGGERBOX")
	
	if deathFXTimer < deathFXDuration then
		
		slashFX:SetActive(false)
		sparksFX:SetActive(false)
		deathFX:SetActive(true)
		deathFXTimer = deathFXTimer + _G.DT
		
		if deathFXTimer >= deathFXDuration then
		
			deathFX:SetActive(false)
		
		end
	
	end
	
	if animTimer < animDuration then
	
		sprite:SetAnimation("DIEFADE",states)
		animTimer = animTimer + _G.DT
		
	else
	
		thisActor:SetActive(false)
	
	end

end