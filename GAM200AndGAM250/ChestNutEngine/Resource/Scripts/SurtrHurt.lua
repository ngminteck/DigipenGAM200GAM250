--[[

This script controls the character getting hurt.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- AttackDecide [DONE]
- Jump [DONE]
- Big Punch [DONE]
- Fall [DONE]
- Die

]]--

-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	transComp = AllComponents:GetTransformComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
    sprite = AllComponents:GetSpriteComponent()
	damageObject = sceneactor:GetChild("PlayerDamageFX")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	effectObject = sceneactor:GetChild("PlayerDashFX")
	dustObject = sceneactor:GetChild("PlayerDashDustFX")
	doubleJumpObject = sceneactor:GetChild("PlayerDoubleJumpFX")
	-- Manage the duration of this state
	timePassed = 0
	flinchDuration = 0.25
	hitBoost = 12
	
	-- Camera shake variables
	myCamera = AllComponents:GetCameraComponent():GetCamera()
	camShakeDistance = 1
	camShakeFrequency = 1
	camShakeDuration = 0.5
	camShaking = false -- for managing camera unlock
	camShook = false -- for managing number of times it shakes in this instance of "Hurt"
	timeSinceShake = 0

end


-- Called every frame.
function Update()
--INFLICT DAMAGE

	effectObject:SetActive(false)
	dustObject:SetActive(false)
	doubleJumpObject:SetActive(false)

  if timePassed == 0 then
    myStats:SetHP(myStats:GetHP() - 1)
  end

	damageObject:SetActive(true)
	rb:SetVel(vec2.new(0, 0))
	
	ShakeCamera()
	UnshakeCamera()

	if timePassed < flinchDuration then
		timePassed = timePassed + _G.DT
	else
		timePassed = 0
		damageObject:SetActive(false)
		--transComp:SetPosition(_G.respawnPoint)
		_G.canDoubleJump = true
		_G.canDash = true
		_G.canMiracleJump = true
		rb:SetVel(vec2.new(rb:GetVel().x, hitBoost))
		myStats:ResetCooldown("INVULN")
		myStats:ActivateCooldown("INVULN", true)
		--invulnEffect:SetActive(true)
		
		camShook = false
		ChooseNextState()
	end

--[[
	-- Check whether the player is dead
	if myStats:GetHP() <= 0 then
		states:SetCurrLogicState("DIE")
    sprite:SetAnimation("DIE",states)
	end
	
	-- Stop moving
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	-- Do nothing while waiting for the state to be over
	timePassed = timePassed + _G.DT
	if timePassed >= flinchDuration then
		ChooseNextState()
		timePassed = 0
	end
--]]
end


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


-- Select the natural next state based on conditions.
function ChooseNextState()

	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACKDECIDE")
    sprite:SetAnimation("ATTACKDECIDE",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")--]]
	elseif (_G.Input_Jump()) then
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)
	elseif myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	else
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end
	
	if myStats:GetHP() == 0 then
	    states:SetCurrLogicState("DIE")
		sprite:SetAnimation("DIE", states)
	end
end

-- Camera shake function
function ShakeCamera()

	-- Only shake camera is camera is not already shaking.
	if camShook == false then
	
		camShaking = true
		camShook = true

		-- Locks the camera to shake it.
		--myCamera:SetViewLock()
		myCamera:SetCamShake(camShakeDistance, camShakeFrequency, camShakeDuration)
	
	end
	
end


-- Unlocks camera after camera finish shaking
function UnshakeCamera()
	
	if camShaking == true then
	
		timeSinceShake = timeSinceShake + _G.DT
		if timeSinceShake >= (camShakeDuration * _G.DT) then
		
			--myCamera:SetViewUnLock()
			timeSinceShake = 0
			camShaking = false
			
		end
	
	end

end