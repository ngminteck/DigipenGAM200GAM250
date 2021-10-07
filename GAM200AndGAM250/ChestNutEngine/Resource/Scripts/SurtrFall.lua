--[[

This script controls the falling of the character.

Possible states this script can transition to:
- Hurt [DONE]
- Jump-attack [DONE]
- Land [DONE]
- Jump

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
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	jumpHitbox = sceneactor:GetChild("PlayerJumpHitbox")
	jumpHitboxColObj = nil
	
	if jumpHitbox ~= nil then
	
		jumpHitboxColObj = jumpHitbox:GetPrefab():GetCollisionComponent():GetCollidedObject()
		jumpHitbox:GetPrefab():GetRigidBodyComponent():SetObjType("TRIGGERBOX")
	
	end
	
	-- Variables for controlling walk
	speed = 6 -- make it same as SurtrWalk
	
	--Midair mobility variables
	jumpHeight = 5.6
	hitBoost = 15
	
	-- Variables for coyote time mechanic
	miracleDuration = 0.2
	timePassed = 0

	groundPoundUnlocked = true
	
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

	MoveOnInput()
	CheckGetHurt()
	--CheckHurt()
	
	if _G.canMiracleJump then
	
			MiracleJump()
			
	else
	
		_G.isMiracleJumping = false;
		
	end

	--print(_G.isMiracleJumping)
	
	if _G.isMiracleJumping == false then
		CheckMidairJump()
	end
	
	LeaveState()
	
	DynamicCameraChecker()
	LimitViewLocationCall()
	RespawnChecker()
	
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


function RespawnChecker()
	
	for i = 1, #colObj do
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "RESPAWNBOX" then
			_G.respawnPoint = colObj[i]:GetPrefab():GetTransformComponent():GetPosition()
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
		
			_G.canDash = true
			_G.canDoubleJump = true
			_G.isMiracleJumping = false
			states:SetCurrLogicState("RESPAWN")
			sprite:SetAnimation("HURT",states)
			--transComp:SetPosition(_G.respawnPoint)
			break
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "ENEMY"
		and myStats:GetCooldown("INVULN") <= 0.0 then
			
			_G.canDash = true
			_G.canDoubleJump = true
			_G.isMiracleJumping = false
			states:SetCurrLogicState("HURT")
			sprite:SetAnimation("HURT",states)
			break
		end
	end
	
	if(_G.i:KeyDown(CVK_CTRL)) then

		if(_G.i:KeyTrig(CVK_0)) then
			transComp:SetPosition(_G.respawnPoint)
		end
	end
end

-- Receive input and move the character accordingly.
function MoveOnInput()

	rb:SetVel(vec2.new(0, rb:GetVel().y))


	if _G.Input_MoveLeft() then
		rb:SetVel(vec2.new(-speed, rb:GetVel().y))
		states:SetCurrDirection(1)
    sprite:SetAnimation("FALL",states)
	end

	if _G.Input_MoveRight() then
		rb:SetVel(vec2.new(speed, rb:GetVel().y))
		states:SetCurrDirection(2)
    sprite:SetAnimation("FALL",states)
	end

end


-- Check if the player got hurt mid-attack.
function CheckGetHurt()

	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
		
		_G.canDash = true
		_G.isMiracleJumping = false
		
		if myCollision:GetColPitfall() then
			--states:SetCurrLogicState("RESPAWN")
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
					
					print("Hurt in fall")
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

-- Allow player to perform a miracle-jump within 0.10 seconds of this state
function MiracleJump()

	timePassed = timePassed + _G.DT
	
	if timePassed < miracleDuration then
	
		--print("Checking miracle")
		if (_G.Input_Jump()) then
			
			_G.isMiracleJumping = true
			timePassed = 0
			states:SetCurrLogicState("JUMP")
			sprite:SetAnimation("JUMP",states)
			
		end
		
	else
	
		--print("Miracle false")
		_G.isMiracleJumping = false
		
	end

end

function CheckMidairJump()
	
	if (_G.Input_Jump()) and _G.canDoubleJump then
	
		_G.isMiracleJumping = false
		timePassed = 0
		states:SetCurrLogicState("JUMP")
		sprite:SetAnimation("JUMP",states)
		_G.canDoubleJump = false
	
	end

end

-- Conditions to leave the current state.
function LeaveState()

	if myCollision:GetColEndGameBox() == true then
		states:SetCurrLogicState("WIN")
    sprite:SetAnimation("WALK",states)
	end

	if _G.Input_Attack() then
	
		timePassed = 0
		
		--[[if _G.i:KeyDown(CVK_DOWN) and groundPoundUnlocked then
			
			states:SetCurrLogicState("GROUNDPOUNDFALL")
      sprite:SetAnimation("GROUNDPOUNDFALL",states)
			--states:SetCurrLogicState("JUMPATTACK")
			
		else
			states:SetCurrLogicState("JUMPATTACK")
      sprite:SetAnimation("JUMPATTACK",states)
			
		end--]]
		
		_G.isMiracleJumping = false
		--print("FROM FALL TO JUMPATTACK")
		states:SetCurrLogicState("JUMPATTACK")
		sprite:SetAnimation("JUMPATTACK",states)
		
	end

	if myCollision:GetColBottom() == true then
		_G.canJumpMidair = true
		_G.canDash = true
		_G.canDoubleJump = true
		_G.canMiracleJump = true

		timePassed = 0
		ChooseNextAction()
		
	end
	
	if _G.canDash and _G.Input_Dash() then
		_G.canDash = false
		_G.isMiracleJumping = false
		timePassed = 0
		--print("FROM FALL TO DASH")
		states:SetCurrLogicState("DASH")
    sprite:SetAnimation("DASH",states)
	end
	
	if myCollision:GetColCheckpoint() == true then
		_G.isMiracleJumping = false
		timePassed = 0
		states:SetCurrLogicState("END")
    sprite:SetAnimation("END",states)
	end

end


-- Chooses the next action.
function ChooseNextAction()

	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
		_G.isMiracleJumping = false
		timePassed = 0
		--print("FROM FALL TO WALK")
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
    sprite:SetAnimation("ATTACK1",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
    sprite:SetAnimation("BIGPUNCH",states)]]--
	elseif (_G.Input_Jump()) then
		_G.isMiracleJumping = false
		timePassed = 0
		--print("FROM FALL TO JUMP")
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)
	else
		_G.isMiracleJumping = false
		timePassed = 0
		--print("FROM FALL TO IDLE")
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
