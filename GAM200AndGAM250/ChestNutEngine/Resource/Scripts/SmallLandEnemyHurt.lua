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
	
	slashFX = sceneactor:GetChild("EnemySlashFX")
	sparksFX = sceneactor:GetChild("EnemyHitSparksFX")
	hitPFX = sceneactor:GetChild("EnemyHitParticlesFX")
	slashFXTransform = slashFX:GetPrefab():GetTransformComponent()
	hitPFXTransform = hitPFX:GetPrefab():GetTransformComponent()
	
	states:SetCurrDirection(2)
	playerHitboxType = "PLAYER_HIT"
	knockbackX = 2
	knockbackY = 5
	
	FXDuration = (0.064 * 3) - 0.01
	FXTimer = 0
	
	hitPFXDuration = (0.075 * 4) - 0.01
	hitPFXTimer = 0
	
	animDuration = (0.1 * 3) - 0.01
	animTimer = 0
	
end

-- Called every frame.
function Update()
	
	if animTimer == 0 then
	
		if states:GetCurrDirection() == 1 then
			rb:SetVel(vec2.new(knockbackX, knockbackY))
			slashFXTransform:SetScale(vec3.new(2, 2, 1))
			hitPFXTransform:SetScale(vec3.new(-3, 3, 1))
		else
			rb:SetVel(vec2.new(-knockbackX, knockbackY))
			slashFXTransform:SetScale(vec3.new(-2, 2, 1))
			hitPFXTransform:SetScale(vec3.new(3, 3, 1))
		end
	
	end
	
	--rb:SetVel(vec2.new(0, 0))
	CheckCollision()
	
	if animTimer < animDuration then
	
		sprite:SetAnimation("HURT",states)
		animTimer = animTimer + _G.DT
		
	else
	
		animTimer = 0
		FXTimer = 0
		hitPFXTimer = 0
		slashFX:SetActive(false)
		sparksFX:SetActive(false)
		hitPFX:SetActive(false)
		states:SetCurrLogicState("IDLE")
		return
		
	end
	
	if hitPFXTimer < hitPFXDuration then
	
		hitPFX:SetActive(true)
		
		hitPFXTimer = hitPFXTimer + _G.DT
		
	else
	
		hitPFX:SetActive(false)
	
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
				FXTimer = 0
				hitPFXTimer = 0
				slashFX:SetActive(false)
				sparksFX:SetActive(false)
				hitPFX:SetActive(false)
				states:SetCurrLogicState("HURT")
			
			end
			
			return
			
		end
		
	end
	
end