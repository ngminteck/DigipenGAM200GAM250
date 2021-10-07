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
	myAABB = AllComponents:GetAABBComponent()
	mySpears = 		{ sceneactor:GetChild("Spear 1"), sceneactor:GetChild("Spear 2"),
					  sceneactor:GetChild("Spear 3"), sceneactor:GetChild("Spear 4"),
					  sceneactor:GetChild("Spear 5") }
	
	dieDuration = 0.44
	dieTimer = 0
	fadeDuration = 0.34
	fadeTimer = 0
	
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
	
		deathFX:SetActive(true)
		deathFXTimer = deathFXTimer + _G.DT
		
		if deathFXTimer >= deathFXDuration then
		
			deathFX:SetActive(false)
		
		end
	
	end
	
	if dieTimer < dieDuration then
	
		sprite:SetAnimation("DIE",states)
		dieTimer = dieTimer + _G.DT
		
		for i = 1, #mySpears do
		
			mySpears[i]:SetActive(false)
		
		end
		
	else
	
		if fadeTimer < fadeDuration then
		
			sprite:SetAnimation("DIEFADE",states)
			fadeTimer = fadeTimer + _G.DT
			
		else
		
			thisActor:SetActive(false)
		
		end
		
	end

end