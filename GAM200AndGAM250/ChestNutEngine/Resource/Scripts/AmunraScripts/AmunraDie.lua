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
  
  explosions = { sceneactor:GetChild("Explosion1"), sceneactor:GetChild("Explosion2"),
	        sceneactor:GetChild("Explosion3"), sceneactor:GetChild("Explosion4")}
  shield = sceneactor:GetChild("Shield")
  shockwave = sceneactor:GetChild("Shockwave")
  
  whitescreen = sceneactor:GetChild("TransitionEffect")
  whitescreen:SetSelfActive(false)
  
  for i = 1, #explosions do
    explosions[i]:SetSelfActive(false)
  end
  
  bossFire = sceneactor:GetChild("Spike19")
  bossFire1 = sceneactor:GetChild("Spike20")
  
  actor = sceneactor
  pla = player
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
  
  dieFlag = 0
  diedieFlag = false
end


-- Called every frame.
function Update()
  if dieFlag == 0 then
    whitescreen:SetSelfActive(true)
    print("DIE")
    _G.audio:StopAllChannel()
	shield:SetSelfActive(false)
	shockwave:SetSelfActive(false)
	  _G.audio:Play("Resource/Audio/Boss/BossDeath.wav", vec3.new(0,0,0), 0.0, false)
    rb:SetHasGravity(false)
    rb:SetObjType("NONEXIST")
    dieFlag = 1
    
    myStats:ResetCooldown("EXPLOSIONCOUNTER")
    myStats:ActivateCooldown("EXPLOSIONCOUNTER", true)
    
    myStats:ResetCooldown("EXPLOSIONDUR")
    myStats:ActivateCooldown("EXPLOSIONDUR", true)
    
    for i = 1, #explosions do
      explosions[i]:SetSelfActive(true)
      explosions[i]:GetPrefab():GetSpriteComponent():SetFrameCount(math.random(9))
    end
  end
  
  if myStats:GetCooldown("EXPLOSIONCOUNTER") > 3.0 then
    cam:SetCamShake(0.1,5,1)
    if myStats:GetCooldown("EXPLOSIONDUR") <= 0.0 then
      for i = 1, #explosions do
        explosions[math.random(4)]:GetPrefab():GetAABBComponent():SetOffset(vec2.new((math.random(100)-50)/50, (math.random(100)-50)/50))
      end
      myStats:ResetCooldown("EXPLOSIONDUR")
    end
  end
  if myStats:GetCooldown("EXPLOSIONCOUNTER") > 0.0 then
    if myStats:GetCooldown("EXPLOSIONCOUNTER") <= 3.0 then
      whitescreen:SetSelfActive(false)
      cam:SetCamShake(0.2,5,1)
      if myStats:GetCooldown("EXPLOSIONDUR") <= 0.0 then
        for i = 1, #explosions do
          explosions[math.random(4)]:GetPrefab():GetAABBComponent():SetOffset(vec2.new((math.random(100)-50)/25, (math.random(100)-50)/25))
        end
        myStats:ResetCooldown("EXPLOSIONDUR")
      end
    end
  elseif myStats:GetCooldown("EXPLOSIONCOUNTER") <= 0.0 then
    if diedieFlag == false then
      for i = 1, #explosions do
        explosions[i]:SetSelfActive(false)
      end
	    myStats:ResetCooldown("DEATHANIM")
	    myStats:ResetCooldown("END")
	    myStats:ActivateCooldown("DEATHANIM", true)
      diedieFlag = true
    end
  end
    
--  if diedieFlag == true then
--    sprite:SetFrameCount(9)
--  end
  
  print(myStats:GetCooldown("DEATHANIM"))
  
  if myStats:IsActive("DEATHANIM") == true
  and myStats:GetCooldown("DEATHANIM") <= 0.0 then
    --diedieFlag = false 
    bossFire:SetSelfActive(false)
    bossFire1:SetSelfActive(false)
    cam:SetViewLocation(6,5,8,false, 1,0.5,0.5)
	  myStats:ActivateCooldown("END", true)
    
  end
  
  if myStats:IsActive("END") == true
  and myStats:GetCooldown("END") <= 0.0 then
    cam:ViewBackItself(0.5,0.5)
    myStats:ActivateCooldown("END", false)
  end  
end

-- Conditions to leave the current state.
function LeaveState()
    dieFlag = 0
    
end
