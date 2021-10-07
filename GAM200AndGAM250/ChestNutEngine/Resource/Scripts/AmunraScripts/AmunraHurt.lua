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
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
    sprite = AllComponents:GetSpriteComponent()
	--damageObject = sceneactor:GetChild("PlayerDamageFX")
	--invulnEffect = sceneactor:GetChild("PlayerInvulnFX")
	
	shockwave = sceneactor:GetChild("Shockwave")
	
	-- Manage the duration of this state
	damageFlag = 0  
	player_rb = player:GetPrefab():GetRigidBodyComponent()
	-- Camera shake variables
	cam = player:GetPrefab():GetCameraComponent():GetCamera()	
	timeSinceShake = 0

end


-- Called every frame.
function Update()
--INFLICT DAMAGE
  if damageFlag == 0 then
    shockwave:SetSelfActive(false)
    myStats:SetHP(myStats:GetHP() - 1)
	_G.audio:Play("Resource/Audio/Boss/BossHurt.wav", vec3.new(0,0,0), 0.0, false)
	print("HURT")
  
	damageFlag = 1
	myStats:ResetCooldown("INVULN")
	myStats:ActivateCooldown("INVULN", true)
  end
  
  ChooseNextState()
end

-- Select the natural next state based on conditions.
function ChooseNextState()

  print(myStats:GetCooldown("INVULN"))
	if myStats:GetCooldown("INVULN") <= 0.0 then
	  damageFlag = 0
	  states:SetCurrLogicState("RESET")
      --sprite:SetSprite("RESET")
	end
	
	if myStats:GetHP() == 0 then
	  states:SetCurrLogicState("DIE")
	  sprite:SetSprite("DIE")
	end
end
