
-- Called at first frame.
function Start(sceneactor, player)

  -- Get components
  AllComponents = sceneactor:GetPrefab()
  states = AllComponents:GetMovementComponent()
  rb = AllComponents:GetRigidBodyComponent()
  myCollision = AllComponents:GetCollisionComponent()
  transComp = AllComponents:GetTransformComponent()
  myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
  colObj = myCollision:GetCollidedObject()
  
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
  _G.bossStage = true
  sceneactor:SetSelfActive(true)
  
  rb:SetObjType("NONEXIST")
  playerCol = player:GetPrefab():GetCollisionComponent()
  
  myStats:RegisterCooldown("INVULN", 1.0, 0.0, false)
  myStats:RegisterCooldown("IDLEDUR", 3.0, 0.0, false)
  myStats:RegisterCooldown("MOVEDUR", math.random(2)+1, 0.0, false)
  myStats:RegisterCooldown("RELOADDUR", 3, 0.0, false)
  myStats:RegisterCooldown("SPIKEPEEK", 3, 0.0, false)
  myStats:RegisterCooldown("SPIKEOUT", 6, 0.0, false)
  myStats:RegisterCooldown("RESTARTSEQUENCE", 4, 0.0, false)
  myStats:RegisterCooldown("DEATHANIM", 1.5, 0.0, false)
  myStats:RegisterCooldown("END", 2, 0.0, false)
  myStats:RegisterCooldown("EXPLOSIONCOUNTER", 5 , 0.0, false)
  myStats:RegisterCooldown("EXPLOSIONDUR", 0.2 , 0.0, false)
  
  
  
  idleFlag = 0
  move = false
  bgmFlag = false
end

-- Called every frame.
function Update()
  if playerCol:GetColDIn() == true then
    _G.respawnPoint = vec2.new(-2,3)
    move = true
  end


  if move == true then
	  if idleFlag == 0 then
	  if bgmFlag == false then
	    G.audio:Play("Resource/Audio/Boss/BGM_Boss.wav", vec3.new(0,0,0), 0.0, true)
		bgmFlag = true
	  end
      print("IDLE")
	    idleFlag = 1
      rb:SetVel(vec2.new(0.0, 0.0))
	    myStats:ResetCooldown("IDLEDUR")
	    myStats:ActivateCooldown("IDLEDUR", true)
	  end
	    
	  --CheckHurt()
	  if myStats:GetCooldown("IDLEDUR") <= 0.0 then
	    LeaveState()
	  end
  end
  --cam:SetCamShake(0.2,1,1)
end

function CheckHurt()
	if myStats:GetCooldown("INVULN") <= 0.0 then
      if myCollision:GetColPlayerHitbox() == true then
	  	states:SetCurrLogicState("HURT")
	  	sprite:SetSprite("HURT")
		idleFlag = 0
	  end
	end

end

-- Conditions to leave the current state.
function LeaveState()
	idleFlag = 0
  states:SetCurrLogicState("WALK")
  sprite:SetSprite("WALK")
end
