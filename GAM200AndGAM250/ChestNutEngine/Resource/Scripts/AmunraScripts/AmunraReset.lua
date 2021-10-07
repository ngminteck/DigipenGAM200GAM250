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
function Start(sceneactor, player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	transComp = AllComponents:GetTransformComponent()
	colComp = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	--damageObject = sceneactor:GetChild("PlayerDamageFX")
	--invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	-- Manage the duration of this state
  resetFlag = 0
	
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
end


-- Called every frame.
function Update()
  if resetFlag == 0 then
    print("RESET")
    rb:SetHasGravity(false)
    resetFlag = 1
  end 
  
  rb:SetVel(vec2.new(0.0, rb:GetVel().y + 0.5))
  
  if colComp:GetColTop() == true then
    rb:SetObjType("NONEXIST")
    rb:SetVel(vec2.new(0.0, 0.0))
    _G.audio:Play("Resource/Audio/Boss/BossSlam.wav", vec3.new(0,0,0), 20, false)
	  cam:SetCamShake(0.1,5,0.4)
    ChooseNextState()
  end
  
end

-- Select the natural next state based on conditions.
function ChooseNextState()
  resetFlag = 0
  states:SetCurrLogicState("IDLE")
  sprite:SetSprite("IDLE",states)

	
end
