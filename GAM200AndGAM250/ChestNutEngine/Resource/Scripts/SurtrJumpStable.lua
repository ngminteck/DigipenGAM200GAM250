--[[

This script controls the jumping of the character.

Possible states this script can transition to:
- Jump-attack [DONE]
- Hurt [DONE]
- Land [DONE]
- Double-jump 

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	transComp = AllComponents:GetTransformComponent()
	effectObject = sceneactor:GetChild("PlayerDoubleJumpFX")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	jumpHitbox = sceneactor:GetChild("PlayerJumpHitbox")
	jumpHitboxColObj = nil
	
	if jumpHitbox ~= nil then
	
		jumpHitboxColObj = jumpHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
		jumpHitbox:GetPrefab():GetRigidBodyComponent():SetObjType("TRIGGERBOX")
	
	end
	
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
  	sprite = AllComponents:GetSpriteComponent()


	-- Jump variables
	jumpHeight = 15 -- surtr jump height
	minJumpDuration = 0.05 -- the minimum duration jump is active
	jumpInputTimer = 0 -- Tracks how long the jump button has been pressed.
	hasJumped = false
	isDoubleJump = false
	hitBoost = 15

	--isJumping = true
	--hasJumped = false
	--hitCeiling = false
	--ceilingRebound = 2
	timePassed = 0
	
	-- Double jump variables
	allowDoubleJumpTime = 0.05 -- duration of djump input
	
	-- Air movement variables
	speed = 6 -- make sure this value is the same as SurtrWalk's
	dashSpeed = 24 -- make this the same as Surtr dash speed.
	speedClamp = 60 -- the max air speed Surtr can reach.
	
	-- Misc variables
	groundPoundUnlocked = true
	debugMode = true -- enable debug printing if true
	effectDuration = 0.08
	effectTimer = effectDuration
	
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
	camMoveSpeed = 1
	dynamicCamMode = false
	defaultZoom = myCamera:GetZoom()
	myCamera:SaveCameraOriginPos()
	
	--Adrian's variables
	alreadyJumped = false
	jumpKeyReleased = false
	
end

-- currently NOT using _G.canJumpMidair --

-- Called every frame.
function Update()
  	-- Set gravity to 35
  	rb:SetGravity(35)
	
	timePassed = timePassed + _G.DT
	
	MinimumJump()
	MoveOnAir()
	JumpAgain()
	CheckCollision()
	CheckAirAttack()
	CheckDash()
	EndJump()
	
	DynamicCameraChecker()
	LimitViewLocationCall()
	
	if myCollision:GetColCheckpoint() == true then
			timePassed = 0
		states:SetCurrLogicState("END")
    sprite:SetAnimation("END",states)
	end
	
	if myCollision:GetColEndGameBox() == true then
		states:SetCurrLogicState("WIN")
    sprite:SetAnimation("WALK",states)
	end
	
	jumpHitbox:SetActive(rb:GetVel().y < 0)
	CheckJumpHitboxCollision()
	
end


-- Checks if Surtr collides with a dynamic camera trigger.
function DynamicCameraChecker()
			
	for i = 1, #colObj do
		
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "DYNAMICCAMERAIN" and _G.cameraInFlag == false then
			
			xCamPos = colObj[i]:GetPrefab():GetRigidBodyComponent():GetModA()
			yCamPos = colObj[i]:GetPrefab():GetRigidBodyComponent():GetModB()
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
					timePassed = 0
					alreadyJumped = false
					jumpKeyReleased = false
					effectTimer = effectDuration
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
				alreadyJumped = false
				jumpKeyReleased = false
				effectTimer = effectDuration
				effectObject:SetActive(false)
				states:SetCurrLogicState("RESPAWN")
				sprite:SetAnimation("HURT",states)
				--transComp:SetPosition(_G.respawnPoint)
				break
			elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
			and myStats:GetCooldown("INVULN") <= 0.0 then
				
				timePassed = 0
				alreadyJumped = false
				jumpKeyReleased = false
				effectTimer = effectDuration
				effectObject:SetActive(false)
				states:SetCurrLogicState("HURT")
				sprite:SetAnimation("HURT",states)
				break
			
			end
		
		end
	
	end

end


-- Handle Surtr light jump or normal jump.
function MinimumJump()

				--print(_G.xbox:KeyUp(BUTTON_A))
	if not alreadyJumped then
	
		rb:SetVel(vec2.new(0, jumpHeight))
		alreadyJumped = true
	
	else
	
		if not jumpKeyReleased then
		
			if (_G.i:KeyUp(KEY_JUMP) and _G.i:KeyUp(KEY_ALTJUMP) and not _G.xbox:KeyDown(BUTTON_A)) then
			
				jumpKeyReleased = true
			
			end
		
		else
		
			if (_G.Input_Jump()) and _G.canDoubleJump then
				
				_G.canDoubleJump = false
				alreadyJumped = false
				jumpKeyReleased = false
				effectObject:SetActive(true)
				states:SetCurrLogicState("DOUBLEJUMP")
			
			end
		
		end
	
	end
	
end

-- Handles the disabling of the jump vfx.
function EffectActivation()
	
	if (effectTimer > 0) then
		
		effectTimer = effectTimer - _G.DT
		
	else
	
		effectObject:SetActive(false)
	
	end	
end

-- Prints text if in debug mode.
function DebugPrint(text)
	if(debugMode) then
	print(text)
	end
end

-- Receives input and allows movement on air
function MoveOnAir()

  if not _G.Input_MoveRight() and not _G.Input_MoveLeft() then
    rb:SetVel(vec2.new(0, rb:GetVel().y))
    sprite:SetAnimation("JUMP",states)
    return
  end 
  
  if _G.Input_MoveRight() and _G.Input_MoveLeft() then
    rb:SetVel(vec2.new(0, rb:GetVel().y))
    sprite:SetAnimation("JUMP",states)
    return
  end
  
	if _G.Input_MoveLeft() then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
		states:SetCurrDirection(1)
    sprite:SetAnimation("JUMP",states)
	end

	if _G.Input_MoveRight() then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
		states:SetCurrDirection(2)
    sprite:SetAnimation("JUMP",states)
	end
	
end

-- Receives input and allows for second jump on air
function JumpAgain()
	if (hasJumped) 
	and (_G.Input_Jump())
	and (_G.canDoubleJump)then
		--Reset state timer.
		timePassed = 0
		hasJumped = false
		isDoubleJump = true
		_G.canDoubleJump = false

		-- Enable double jump vfx.
		effectObject:SetActive(true)
			timePassed = 0

		--print("Jumping again")
		--states:SetCurrLogicState("JUMP")
		sprite:SetAnimation("JUMP",states)
	end
end

-- Check for attack input mid-air
function CheckAirAttack()

	if myCollision:GetColBottom() == false and _G.Input_Attack() then
		
		timePassed = 0
		alreadyJumped = false
		jumpKeyReleased = false
		effectTimer = effectDuration
		effectObject:SetActive(false)
		states:SetCurrLogicState("JUMPATTACK")
		sprite:SetAnimation("JUMPATTACK",states)
		
	end
end

-- Checks if dash is possible
function CheckDash()

	if (_G.canDash) and (_G.Input_Dash()) then
		
		timePassed = 0
		effectTimer = effectDuration
				alreadyJumped = false
				jumpKeyReleased = false
		effectObject:SetActive(false)
		states:SetCurrLogicState("DASH")
    	sprite:SetAnimation("DASH",states)
		_G.canDash = false
		_G.canJump = false
		
	end
end

-- End jump state (aka I can jump again)
function EndJump()

	if (myCollision:GetColBottom() == true) then
		-- Reset state timer.
		timePassed = 0
		_G.canDash = true
		_G.canJump = true
		_G.canDoubleJump = true
		_G.canMiracleJump = true
		hasJumped = false
		isDoubleJump = false
		--hitCeiling = false
		--isJumping = true

		--Transistion to next state.
		ChooseNextAction()
	end
end

--Chooses the next action.
function ChooseNextAction()

	-- If movement keys pressed, go to Walk State.
	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
			timePassed = 0
				alreadyJumped = false
				jumpKeyReleased = false
		effectTimer = effectDuration
		effectObject:SetActive(false)
		--print("FROM JUMP TO WALK")
		states:SetCurrLogicState("WALK")
		sprite:SetAnimation("WALK",states)

    -- If no input, go to Idle State.
	elseif rb:GetVel().y <= 0 then
		effectTimer = effectDuration
		effectObject:SetActive(false)
		--print("FROM JUMP TO IDLE")
			timePassed = 0
				alreadyJumped = false
				jumpKeyReleased = false
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
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
