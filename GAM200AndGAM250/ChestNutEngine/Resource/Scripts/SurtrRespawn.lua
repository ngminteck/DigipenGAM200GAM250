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
	self = sceneactor
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	transComp = AllComponents:GetTransformComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	effectObject = sceneactor:GetChild("PlayerDamageFX")
	invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	dashObject = sceneactor:GetChild("PlayerDashFX")
	dustObject = sceneactor:GetChild("PlayerDashDustFX")
	doubleJumpObject = sceneactor:GetChild("PlayerDoubleJumpFX")
	-- Manage the duration of this state
	timePassed = 0
	flinchDuration = 0.2
	
	-- Variables for saw/spike collision feedback
	stateDuration = 0.5
	fadeDuration = 0.2
	myCamera = AllComponents:GetCameraComponent():GetCamera()
	defaultZoom = myCamera:GetZoom()
	currentZoom = myCamera:GetZoom()
	camInitialised = false
	lerpSpeed = 0.03
			
end


-- Called every frame.
function Update()

	dashObject:SetActive(false)
	dustObject:SetActive(false)
	doubleJumpObject:SetActive(false)
	sprite:SetAnimation("HURT",states)

--INFLICT DAMAGE
  if timePassed == 0 then
    myStats:SetHP(myStats:GetHP() - 1)
  end
  	
	effectObject:SetActive(true)
	rb:SetVel(vec2.new(0, 0))

	if timePassed < stateDuration then
	
		timePassed = timePassed + _G.DT
		InitCameraAtInstance()
		ZoomCamera()
		
	elseif timePassed <= (stateDuration + fadeDuration) then
	
		timePassed = timePassed + _G.DT
		-- FADE TO BLACK HERE
	
	else
		if _G.bossStage == false then
      RevertCamera()
    end
		timePassed = 0
		effectObject:SetActive(false)
		transComp:SetPosition(vec2.new((_G.respawnPoint.x), (_G.respawnPoint.y + 2)))
		_G.canDoubleJump = true
		_G.canDash = true
		_G.canMiracleJump = true
		
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


-- Select the natural next state based on conditions.
function ChooseNextState()
	sprite:SetAnimation("IDLE",states)
	if _G.Input_MoveLeft() or _G.Input_MoveRight() then
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACKDECIDE")
    sprite:SetAnimation("ATTACKDECIDE",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")--]]
	elseif _G.Input_Jump() then
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)
	elseif myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	else
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end

end


-- Initialise the camera at the instance right before it gets manipulated (to cater for dynamic camera)
function InitCameraAtInstance()

	if camInitialised == false then
	
		defaultZoom = myCamera:GetZoom()
		currentZoom = myCamera:GetZoom()
		camInitialised = true
		print(currentZoom)
		
	end
	
end


-- Zooms the camera in according to lerp speed
function ZoomCamera()

	if currentZoom > (defaultZoom - 2) then
		currentZoom = currentZoom - lerpSpeed
	end
	
	myCamera:SetViewLocation((transComp:GetPosition().x), (transComp:GetPosition().y), currentZoom, false, 1)
	--myCamera:SetZoom(currentZoom)

end


-- Returns the camera to normal
function RevertCamera()

	-- Not really "revert", but moves the camera back to respawn position.
	--myCamera:SetViewLocation((_G.respawnPoint.x), (_G.respawnPoint.y), 0, true, 0.5)

	--Revert zoom.
	camInitialised = false
	currentZoom = defaultZoom
	myCamera:SetZoom(currentZoom, 1)

end