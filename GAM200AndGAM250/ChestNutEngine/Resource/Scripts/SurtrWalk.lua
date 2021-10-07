--[[

This script controls the walking of the character.

Possible states this script can transition to:
- Idle [DONE]
- AttackDecide [DONE]
- Jump [DONE]
- BigPunch [DONE]
- Dash [DONE]
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
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	myCamera = AllComponents:GetCameraComponent()
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
  
	-- Variables for controlling walk
	speed = 6
	
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
	
	
	_G.isMiracleJumping = false
	
	DynamicCameraChecker()
	LimitViewLocationCall()
	
	MoveOnInput()
	LeaveState()
	
	CheckCollision()

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
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
			
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end

-- Receive input and move the character accordingly.
function MoveOnInput()

	rb:SetVel(vec2.new(0, rb:GetVel().y))
  if not _G.Input_MoveRight() and not _G.Input_MoveLeft() then
    rb:SetVel(vec2.new(0, rb:GetVel().y))
    sprite:SetAnimation("IDLE",states)
    states:SetCurrLogicState("IDLE")
    return
  end 
  
  if _G.Input_MoveRight() and _G.Input_MoveLeft() then
    rb:SetVel(vec2.new(0, rb:GetVel().y))
    sprite:SetAnimation("IDLE",states)
    states:SetCurrLogicState("IDLE")
    return
  end
	if _G.Input_MoveLeft() then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
		states:SetCurrDirection(1)
    sprite:SetAnimation("WALK",states)
	end

	if _G.Input_MoveRight() then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
		states:SetCurrDirection(2)
    sprite:SetAnimation("WALK",states)
	end

end


-- Conditions to leave the current state.
function LeaveState()

	if myCollision:GetColEndGameBox() == true then
		states:SetCurrLogicState("WIN")
    sprite:SetAnimation("WALK",states)
	end

	if _G.i:KeyUp(KEY_LEFT) and _G.i:KeyUp(KEY_RIGHT) and _G.xbox:KeyUp(LSTICK_LEFT) and _G.xbox:KeyUp(LSTICK_RIGHT) then
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	return
	end

	if myCollision:GetColBottom() == false then
	--print("Fall transition")
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	return
	end

	if (_G.Input_Jump()) and myCollision:GetColBottom() == true then
	--print("Jump transition")
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)
	return
	
	end

	if _G.Input_Attack() then
	states:SetCurrLogicState("ATTACK1")
    sprite:SetAnimation("ATTACK1",states)
	end

	if _G.Input_Dash() and myCollision:GetColBottom() == true and
	   _G.canDash and myStats:GetCooldown("DASHCOOLDOWN") == 0 then
		myStats:ResetCooldown("DASHCOOLDOWN")
		myStats:ActivateCooldown("DASHCOOLDOWN", true)
		states:SetCurrLogicState("DASH")
    sprite:SetAnimation("DASH",states)
	return
	end
	
	if myCollision:GetColCheckpoint() == true then
		states:SetCurrLogicState("END")
    sprite:SetAnimation("END",states)
	return
	end
	
end
