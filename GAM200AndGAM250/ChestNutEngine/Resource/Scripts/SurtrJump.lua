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
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
  	sprite = AllComponents:GetSpriteComponent()
  	-- Set gravity to 30
  	rb:SetGravity(50)

	-- Jump variables
	jumpHeight = 9.5 -- surtr jump height
	jumpInputDuration = 0.25 -- duration of jump input
	minJumpDuration = 0.05 -- the minimum duration jump is active
	jumpInputTimer = 0 -- Tracks how long the jump button has been pressed.
	hasJumped = false
	isDoubleJump = false

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
	damagingNames = { spearID, landEnemyID, sawProjID }
	
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

-- currently NOT using _G.canJumpMidair --

-- Called every frame.
function Update()

	timePassed = timePassed + _G.DT

	-- Jump code here
	MinimumJump()
	
	--RespawnChecker()
	
	--CheckCeiling()
	MoveOnAir()
	JumpAgain()
	DynamicCameraChecker()
	LimitViewLocationCall()
	CheckGetHurt()
	CheckHurt()
	CheckAirAttack()
	CheckDash()
	EndJump()
	
	if myCollision:GetColCheckpoint() == true then
			timePassed = 0
		states:SetCurrLogicState("END")
    sprite:SetAnimation("END",states)
	end
	
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


-- Handle Surtr light jump or normal jump.
function MinimumJump()
	if(timePassed < minJumpDuration) then
	
		rb:SetVel(vec2.new(0, jumpHeight))
		
	else
	
		-- Check if jump or double jump.
		if(isDoubleJump) then
			states:SetCurrLogicState("DOUBLEJUMP")
			-- ExecuteDoubleJump()
		else
			ExecuteJump()
		end
		
	end
end

-- Executes the jump function
function ExecuteJump()

	currentV = rb:GetVel()

	if(currentV.y > jumpHeight) then
		currentV.y = jumpHeight
	end

	if (_G.Input_Jump())
	and (hasJumped == false)
	and	(timePassed <= jumpInputDuration) then

		rb:SetVel(vec2.new(0, jumpHeight))

	-- If the jump key is released, end jump prematurely
	elseif (_G.i:KeyUp(KEY_JUMP) and _G.i:KeyUp(KEY_ALTJUMP) and _G.xbox:KeyUp(BUTTON_A)) then

		--rb:SetVel(vec2.new(0, currentV.y))
		-- Next jump input will be double jump unless touching ground
		hasJumped = true
	end
end

-- Executes Surtr double jump
function ExecuteDoubleJump()

	if (timePassed <= jumpInputDuration) then

		--rb:SetVel(vec2.new(0, jumpHeight/1.2))
		rb:SetVel(vec2.new(0, jumpHeight))

		-- Spawns Surtrs double jump vfx.	
		EffectActivation()

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

-- Handles collision interactions.
function RespawnChecker()
	
	for i = 1, #colObj do
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "RESPAWNBOX" then
			_G.respawnPoint = colObj[i]:GetPrefab():GetTransformComponent():GetPosition()
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
			timePassed = 0
			states:SetCurrLogicState("RESPAWN")
			sprite:SetAnimation("HURT",states)
			--transComp:SetPosition(_G.respawnPoint)
			break
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
		and myStats:GetCooldown("INVULN") <= 0.0 then
		
			timePassed = 0
			states:SetCurrLogicState("HURT")
			sprite:SetAnimation("HURT",states)
			break
		end
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
--	-- Get current velocity
--	local currentV = rb:GetVel()
--	
--	if _G.i:KeyDown(CVK_LEFT) then
--
--		-- assign velocity for left movement
--		currentV.x = -speed
--
--		states:SetCurrDirection(1)
--
--	elseif _G.i:KeyDown(CVK_RIGHT) then
--
--		-- assign velocity for right movement
--		currentV.x = speed
--		states:SetCurrDirection(2)
--    
--	elseif not _G.i:KeyDown(CVK_RIGHT) and not _G.i:KeyDown(CVK_LEFT) then
--    currentV.x = 0
--  
--  elseif _G.i:KeyDown(CVK_RIGHT) and _G.i:KeyDown(CVK_LEFT) then
--    currentV.x = 0
--    
--  end
--  
--	rb:SetVel(vec2.new(currentV.x, rb:GetVel().y))
--	sprite:SetAnimation("JUMP",states)
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

		states:SetCurrLogicState("JUMP")
		sprite:SetAnimation("JUMP",states)
	end
end

-- Check if the player got hurt mid-attack.
function CheckGetHurt()

	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
		
		print("Hurt")
		_G.canDoubleJump = true
		_G.canDash = true
		timePassed = 0
		effectTimer = effectDuration
		effectObject:SetActive(false)
		
		if myCollision:GetColPitfall() then
			states:SetCurrLogicState("RESPAWN")
		elseif myStats:GetCooldown("INVULN") <= 0.0 then
			states:SetCurrLogicState("HURT")
		end
		
		sprite:SetAnimation("HURT",states)
	end
end

function CheckHurt()

	if myStats:GetCooldown("INVULN") <= 0.0 then
		
		validHit = false
		invulnEffect:SetActive(false)
	
		for i = 1, #colObj do
		
			for j = 1, #damagingNames do
			
				--print(hitboxColObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"))
				if string.sub(colObj[i]:GetSceneActorTaggingValue("scene_actor_name_key"), 1, string.len(damagingNames[j])) == damagingNames[j] then
					
					states:SetCurrLogicState("HURT")
					sprite:SetAnimation("HURT",states)
					validHit = true
					break
					
				end
				
			end
			
			if validHit then break end
		
		end
	
	end

end

-- Check for attack input mid-air
function CheckAirAttack()

	if myCollision:GetColBottom() == false and _G.Input_Attack() then
	
		timePassed = 0
		
		--[[if _G.i:KeyDown(KEY_DOWN) and groundPoundUnlocked then
			
			effectTimer = effectDuration
			effectObject:SetActive(false)
			states:SetCurrLogicState("GROUNDPOUNDFALL")
			sprite:SetAnimation("GROUNDPOUNDFALL",states)
			--states:SetCurrLogicState("JUMPATTACK")
			
		else
		
			effectTimer = effectDuration
			effectObject:SetActive(false)
			states:SetCurrLogicState("JUMPATTACK")
			sprite:SetAnimation("JUMPATTACK",states)
		end--]]
		
			timePassed = 0
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
		effectTimer = effectDuration
		effectObject:SetActive(false)
		states:SetCurrLogicState("WALK")
		sprite:SetAnimation("WALK",states)

	--[[elseif _G.i:KeyDown(CVK_X) then
		effectTimer = effectDuration
		effectObject:SetActive(false)
		states:SetCurrLogicState("ATTACK1")
		sprite:SetAnimation("ATTACK1",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
    sprite:SetAnimation("BIGPUNCH",states)]]--

    -- If no input, go to Idle State.
	elseif rb:GetVel().y <= 0 then
		effectTimer = effectDuration
		effectObject:SetActive(false)
		--print("Idle transition")
			timePassed = 0
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
	end
end

--[[
-- React if the player hits the ceiling while jumping
function CheckCeiling()

	if myCollision:GetColTop() == true then
		hitCeiling = true
		rb:SetVel(vec2.new(0, -ceilingRebound))
	end

end]]--
