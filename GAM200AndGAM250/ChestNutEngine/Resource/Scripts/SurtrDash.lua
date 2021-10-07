--[[

This script controls the dashing of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- AttackDecide [DONE]
- Jump [DONE]
- Big Punch [DONE]
- Hurt [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	transComp = AllComponents:GetTransformComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	attackHitbox = sceneactor:GetChild("PlayerDashHitbox")
	effectObject = sceneactor:GetChild("PlayerDashFX")
	dustObject = sceneactor:GetChild("PlayerDashDustFX")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	effectTransform = effectObject:GetPrefab():GetTransformComponent()
	dustTransform = dustObject:GetPrefab():GetTransformComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	-- Variables for managing dash
	dashRecoverTime = 0.15
	timePassed = 0

	-- Variables for controlling dash
	dashDirection = "n"
	speed = 18.6 -- speed of dash // change the value in SurtrJump to match this value.
	
	-- Variables for locking/unlocking offensiveness
	offensive = false
	
	spearID = "Spear "
	spearEnemyID = "SpearThrowEnemy"
	landEnemyID = "SmallLandEnemy"
	sawProjID = "Saw_Projectile"
	bossFire1 = "Spike1"
	bossFire2 = "Spike2"
	bossFire3 = "Spike3"
	bossFire4 = "Spike4"
	bossFire5 = "Spike5"
	bossFire6 = "Spike6"
	bossFire7 = "Spike7"
	bossFire8 = "Spike8"
	bossFire9 = "Spike9"
	bossFire10 = "Spike10"
	bossFire11 = "Spike11"
	bossFire12 = "Spike12"
	bossFire13 = "Spike13"
	bossFire14 = "Spike14"
	bossFire15 = "Spike15"
	bossFire16 = "Spike16"
	bossFire17 = "Spike17"
	bossFire18 = "Spike18"
	bossFire19 = "Spike19"
	bossFire20 = "Spike20"
	damagingNames = { spearID, landEnemyID, sawProjID,
	bossFire1 ,
	bossFire2 ,
	bossFire3 ,
	bossFire4 ,
	bossFire5 ,
	bossFire6 ,
	bossFire7 ,
	bossFire8 ,
	bossFire9 ,
	bossFire10,
	bossFire11,
	bossFire12,
	bossFire13,
	bossFire14,
	bossFire15,
	bossFire16,
	bossFire17,
	bossFire18,
	bossFire19,
	bossFire20
	}
	-- Variable(s) for dynamic camera
	myCamera = AllComponents:GetCameraComponent():GetCamera()
	xCamPos = 0
	yCamPos = 0
	zoom = 0
	zoomSpeed = 0
	camMoveSpeed = 0.4
	dynamicCamMode = false
	defaultZoom = myCamera:GetZoom()
	myCamera:SaveCameraOriginPos()

end

-- Called every frame.
function Update()
	
	_G.canMiracleJump = false
	CheckCollision()
	DecideWhetherToDash()
	DynamicCameraChecker()
	LimitViewLocationCall()

	if myCollision:GetColEndGameBox() == true then
		states:SetCurrLogicState("WIN")
    sprite:SetAnimation("WALK",states)
	end
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
			--print(states:GetCurrLogicState())
			myCamera:ViewBackItself(0.0, 0.0)
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

-- Decide whether to dash
function DecideWhetherToDash()
	timePassed = timePassed + _G.DT
	if timePassed <= dashRecoverTime then
	
		if myCollision:GetColBottom() then
			dustObject:SetActive(true)
		end
	
		effectObject:SetActive(true)
		DetermineDashDirection()
		DashTowardsDirection()
	else
		dustObject:SetActive(false)
		effectObject:SetActive(false)
		if not myCollision:GetColBottom() then _G.canDash = false end
		LeaveState()
	end
	
	-- insert jump exception
--	if(_G.i:KeyDown(CVK_SPACE)) then
--		-- Stop attack
--		attackHitbox:SetActive(false)
--
--		-- Reset dash conditions
--		timePassed = 0
--		dashDirection = "n"
--		dustObject:SetActive(false)
--		effectObject:SetActive(false)
--		states:SetCurrLogicState("JUMP")
--		sprite:SetAnimation("JUMP",states)--]]
--	end
end

-- Determine direction of dash
function DetermineDashDirection()

	if states:GetCurrDirection() == 1 then
		dashDirection = "l"
		effectTransform:SetScale(vec3.new(-1, 1, 1))
		dustTransform:SetScale(vec3.new(-1, 1, 1))
	end

	if states:GetCurrDirection() == 2 then
		dashDirection = "r"
		effectTransform:SetScale(vec3.new(1, 1, 1))
		dustTransform:SetScale(vec3.new(1, 1, 1))
	end

end

-- Dash if player can still dash
function DashTowardsDirection()

	-- Start attack
	if offensive == true then
		attackHitbox:SetActive(true)
	end

	rb:SetVel(vec2.new(0, 0))

	if dashDirection == "l" then
		rb:SetVel(vec2.new(-speed, 0))
		states:SetCurrDirection(Direction.Left)
    sprite:SetAnimation("DASH",states)
		dashDirection = "l"
	end

	if dashDirection == "r" then
		rb:SetVel(vec2.new(speed, 0))
		states:SetCurrDirection(Direction.Right)
    sprite:SetAnimation("DASH",states)
		dashDirection = "r"
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
					dashDirection = "n"
					dustObject:SetActive(false)
					effectObject:SetActive(false)
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
				dashDirection = "n"
				dustObject:SetActive(false)
				effectObject:SetActive(false)
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
				timePassed = 0
				dashDirection = "n"
				dustObject:SetActive(false)
				effectObject:SetActive(false)
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end


-- End dash
function LeaveState()

	
	-- Stop attack
	attackHitbox:SetActive(false)

	-- Reset dash conditions
	timePassed = 0
	dashDirection = "n"

	-- Decide next state
	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
	
		dustObject:SetActive(false)
		effectObject:SetActive(false)
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACKDECIDE")
    sprite:SetAnimation("ATTACKDECIDE",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	elseif _G.i:KeyDown(CVK_SPACE) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)--]]
	elseif myCollision:GetColBottom() == false then
		dustObject:SetActive(false)
		effectObject:SetActive(false)
		_G.canMiracleJump = false
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	else
		_G.canMiracleJump = true
		dustObject:SetActive(false)
		effectObject:SetActive(false)
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
	end
	
	if myCollision:GetColCheckpoint() == true then
		states:SetCurrLogicState("END")
    sprite:SetAnimation("END",states)
	end

end
