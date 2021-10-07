--[[

This script controls the aerial attack of the character.

Possible states this script can transition to:
- Fall [DONE]
- Hurt [untested]
- Land [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("PlayerHitbox")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	jumpHitbox = sceneactor:GetChild("PlayerJumpHitbox")
	jumpHitboxColObj = nil
	
	if jumpHitbox ~= nil then
	
		jumpHitboxColObj = jumpHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
		jumpHitbox:GetPrefab():GetRigidBodyComponent():SetObjType("TRIGGERBOX")
	
	end
	
	sprite = AllComponents:GetSpriteComponent()
	hitboxColObj = attackHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
    
	-- Variables for managing attack
	timePassed = 0
	attackDuration = 0.45
	
	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.2

	-- Variables for controlling movement
	speed = 6 -- same as SurtrWalk
	
	spearID = "Spear "
	spearEnemyID = "SpearThrowEnemy"
	landEnemyID = "SmallLandEnemy"
	sawProjID = "Saw_Projectile"
	bossFire = "Spike19"
	bossFire1 = "Spike20"
	hittableNames = { spearID, spearEnemyID, landEnemyID, sawProjID }
	damagingNames = { spearID, sawProjID, bossFire, bossFire1 }
	hitBoost = 15
	
	-- Variable(s) for dynamic camera
	myCamera = AllComponents:GetCameraComponent():GetCamera()
	xCamPos = 0
	yCamPos = 0
	zoom = 0
	zoomSpeed = 0
	camMoveSpeed = 0.4
	dynamicCamMode = false
	defaultZoom = myCamera:GetZoom()
	defaultCamPos = myCamera:SaveCameraOriginPos()

end


-- Called every frame.
function Update()

	_G.canMiracleJump = false
	ExecuteJumpAttack()
	CheckHitboxCollision()
	MoveOnAir()
	DynamicCameraChecker()
	LimitViewLocationCall()
	CheckCollision()
	EndJumpAttack()
	
	jumpHitbox:SetActive(rb:GetVel().y < 0)
	CheckJumpHitboxCollision()

end


-- Checks if Surtr collides with a dynamic camera trigger.
function DynamicCameraChecker()
			
	for i = 1, #colObj do
		
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "DYNAMICCAMERAIN" and _G.cameraInFlag == false then
			
			xCamPos = colObj[i]:GetPrefab():GetRigidBodyComponent():GetModA()
			yCamPos = colObj[i]:GetPrefab():GetRigidBodyComponent():GetModB()
			camMoveSpeed = colObj[i]:GetPrefab():GetRigidBodyComponent():GetModC()
			zoom = colObj[i]:GetPrefab():GetRigidBodyComponent():GetDuration()
			zoomSpeed = colObj[i]:GetPrefab():GetRigidBodyComponent():GetStartTime()
			myCamera:SetViewLocation(xCamPos, yCamPos, zoom, false, 0, camMoveSpeed, zoomSpeed)
			
			_G.cameraInFlag = true
			
			--[[
			if viewAlreadySet == false then
				myCamera:SetViewLocation(xCamPos, yCamPos, zoom, false, 0, camMoveSpeed, zoomSpeed)
				viewAlreadySet = true
			else
				timePassedSinceSetView = timePassedSinceSetView + _G.DT
				if timePassedSinceSetView >= setViewCooldown then
					timePassedSinceSetView = 0
					viewAlreadySet = false
				end
			end]]--
		
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "DYNAMICCAMERAOUT" and _G.cameraOutFlag == false then
			
			zoom = colObj[i]:GetPrefab():GetRigidBodyComponent():GetDuration()
			zoomSpeed = colObj[i]:GetPrefab():GetRigidBodyComponent():GetStartTime()
			myCamera:SetOriginZoom(zoom)
			myCamera:ViewBackItself(camMoveSpeed,zoomSpeed)
			--myCamera:SetSelfView(xCamPos ,yCamPos ,15,camMoveSpeed, zoomSpeed)
			
			_G.cameraOutFlag = true
			
		end
	
	end

end


-- This function limits collision with dynamic camera triggers
function LimitViewLocationCall()

	if myCollision:GetColDIn() == false then
		_G.cameraInFlag = false
	end

	if myCollision:GetColDOut() == true then
		_G.cameraOutFlag = false
	end

end


-- Executes the jump attack.
function ExecuteJumpAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay then

		attackHitbox:SetActive(true)
		timePassedForHitbox = 0

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	validHit = false
	
	for i = 1, #hitboxColObj do
	
		for j = 1, #hittableNames do
		
			--print(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
			if string.sub(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"), 1, string.len(hittableNames[j])) == hittableNames[j] then
				
				rb:SetVel(vec2.new(rb:GetVel().x, hitBoost))
				attackHitbox:SetActive(false)
				_G.canDoubleJump = true
				validHit = true
				break
				
			end
			
		end
		
		if validHit then break end
	
	end
	
	validHit = false
	
end


-- Receives input and allows movement on air
function MoveOnAir()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if _G.Input_MoveLeft() then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
		states:SetCurrDirection(Direction.Left)
    sprite:SetAnimation("JUMPATTACK",states)
	end

	if _G.Input_MoveRight() then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
		states:SetCurrDirection(Direction.Right)
    sprite:SetAnimation("JUMPATTACK",states)
	end

end


-- Checks if Surtr is hurt
function CheckCollision()

	if myStats:GetCooldown("INVULN") <= 0.0 then
		
		validHit = false
		invulnEffect:SetActive(false)
	
		for i = 1, #colObj do
		
			for j = 1, #damagingNames do
			
				--print(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
				if string.sub(colObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"), 1, string.len(damagingNames[j])) == damagingNames[j] then
					
					print(colObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
					timePassed = 0
					attackHitbox:SetActive(false)
					states:SetCurrLogicState("HURT")
					sprite:SetAnimation("HURT",states)
					validHit = true
					break
					
				end
				
			end
			
			if validHit then break end
			
			if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "RESPAWNBOX" then
				_G.respawnPoint = colObj[i]:GetPrefab():GetTransformComponent():GetPosition()
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
				timePassed = 0
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
				
				timePassed = 0
				attackHitbox:SetActive(false)
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end


-- End jump attack state
function EndJumpAttack()

	if myCollision:GetColEndGameBox() == true then
		states:SetCurrLogicState("WIN")
    sprite:SetAnimation("WALK",states)
	end
	
	timePassed = timePassed + _G.DT

	if myCollision:GetColBottom() == true then
	
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
	elseif timePassed >= attackDuration then
	
		timePassed = 0
		attackHitbox:SetActive(false)
		_G.canMiracleJump = false
		states:SetCurrLogicState("FALL")
		sprite:SetAnimation("FALL",states)
	end

end

function CheckJumpHitboxCollision()

	for i = 1, #jumpHitboxColObj do
	
		if jumpHitboxColObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY" then
		
			rb:SetVel(vec2.new(rb:GetVel().x, hitBoost))
			jumpHitbox:SetActive(false)
			return
		
		end
	
	end

end
