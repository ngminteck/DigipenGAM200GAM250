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
function Start(sceneactor,player)

--	-- Get components
  AllComponents = sceneactor:GetPrefab()
  states = AllComponents:GetMovementComponent()
  transComp = AllComponents:GetTransformComponent()
  rb = AllComponents:GetRigidBodyComponent()
  myCollision = AllComponents:GetCollisionComponent()
  colObj = myCollision:GetCollidedObject()
  myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
  playerTrans = player:GetPrefab():GetTransformComponent()
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
  
  bossFire = sceneactor:GetChild("Spike19")
  bossFire1 = sceneactor:GetChild("Spike20")
  bossFire:SetSelfActive(false)
  bossFire1:SetSelfActive(false)
  
  movespeed = 3
  moveFlag = 0
  attackType1Counter = 0
  attackType2Counter = 0
end


-- Called every frame.
function Update()
  if moveFlag == 0 then
    bossFire:SetSelfActive(true)
    bossFire1:SetSelfActive(true)
    print("WALK")
    moveFlag = 1
    myStats:SetCooldown("MOVEDUR", math.random(2))
    myStats:ResetCooldown("MOVEDUR")
    myStats:ActivateCooldown("MOVEDUR", true)
	  _G.audio:Play("Resource/Audio/Boss/BossMove.wav", vec3.new(0,0,0), 0.0, false)
    
    if playerTrans.Pos.x - transComp.Pos.x <= 0 then
        rb:SetVel(vec2.new(-movespeed, 0.0))
      else
        rb:SetVel(vec2.new(movespeed, 0.0))
      end
    end
    
    if transComp.Pos.x < -10 then
      rb:SetVel(vec2.new(movespeed, 0.0))
    elseif transComp.Pos.x > 10 then
      rb:SetVel(vec2.new(-movespeed, 0.0))
    end
  
  --cam:SetCamShake(1,1,1)
  
  if myStats:GetCooldown("MOVEDUR") <= 0.0 then
    LeaveState()
  end

end

-- Conditions to leave the current state.
function LeaveState()
    moveFlag = 0
    --attackType1Counter = 3
    --attackType2Counter = 1
	if attackType1Counter == 3 
	and attackType2Counter == 1 then
	  states:SetCurrLogicState("ATTACK3")
      
    attackType1Counter = 0
	  attackType2Counter = 0
    elseif attackType1Counter == 3 then
	    attackType2Counter = 1
      states:SetCurrLogicState("ATTACK2")  
	  sprite:SetSprite("ATTACK2")
    else
	  states:SetCurrLogicState("ATTACK1")
      sprite:SetSprite("ATTACK1")
      attackType1Counter = attackType1Counter + 1
    end
end
