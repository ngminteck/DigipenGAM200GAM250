

-- Called at first frame.
function Start(sceneactor, player)

	-- Get components/Children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	transComp = AllComponents:GetTransformComponent()
	
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
  
	saws = { sceneactor:GetChild("Saw4"), sceneactor:GetChild("Saw5"), 
	         sceneactor:GetChild("Saw6"), sceneactor:GetChild("Saw7"), sceneactor:GetChild("Saw8"),
			     sceneactor:GetChild("Saw9") }
           
	shockwave = sceneactor:GetChild("Shockwave")
	shield = sceneactor:GetChild("Shield")
	shield:SetSelfActive(true)
    shockwave:SetSelfActive(false)
    saw4_aabb = saws[1]:GetPrefab():GetAABBComponent()
    saw5_aabb = saws[2]:GetPrefab():GetAABBComponent()
    saw6_aabb = saws[3]:GetPrefab():GetAABBComponent()
    saw7_aabb = saws[4]:GetPrefab():GetAABBComponent()
    saw8_aabb = saws[5]:GetPrefab():GetAABBComponent()
    saw9_aabb = saws[6]:GetPrefab():GetAABBComponent()
	
    for i = 1, #saws do
      saws[i]:SetSelfActive(false)
    end
    
  shockwave_sprite = shockwave:GetPrefab():GetSpriteComponent()
	
  attackFlag = 0
	inPosition = 0
	drop = 0
	speedx = 0
	restart = 0
  soundWindupFlag = 0
  soundShootFlag = 0
  shockwaveCounter = 27
  windupFlag = false
end

-- Called every frame.
function Update()
  if attackFlag == 0 then
    print("ATTACK3")
    attackFlag = 1
	  myStats:ResetCooldown("RESTARTSEQUENCE")
	  if transComp.Pos.x <= 0 then
        xvec = -8 - transComp.Pos.x
	    rb:SetVel(vec2.new(xvec/3.0, 0.0))
        speedx = 0.1
      else
        xvec = 8 - transComp.Pos.x
	    rb:SetVel(vec2.new(xvec/3.0, 0.0))
	    speedx = -0.1
      end
  end
  
  
  if inPosition == 0 then
    MoveToLeftOrRightPlatform()
  end
  
  if inPosition == 1 then
    --print(sprite:GetTextureName())
    if sprite:GetFrameCount() == 8 then
      sprite:SetSprite("SLAM_DOWN")
	  sprite:SetFrameCount(0)
      rb:SetHasGravity(true)
    end
	
	if windupFlag == false then
	  sprite:SetSprite("SLAM_WINDUP")
	  sprite:SetFrameCount(0)
	  windupFlag = true
	end
	
    rb:SetObjType("ENEMY")
	if myCollision:GetColBottom() == true then
	  shield:SetSelfActive(false)
	  drop = 1
	  inPosition = 2
	  cam:SetCamShake(0.1,5,0.4)
	  _G.audio:Play("Resource/Audio/Boss/BossSlam.wav", vec3.new(0,0,0), 0.0, false)
      shockwave:SetSelfActive(true)
      shockwave_sprite:SetFrameCount(0)
	  print("LANDED")
		
      sprite:SetSprite("SLAM_RECOVER")
	  sprite:SetFrameCount(0)
	    myStats:ActivateCooldown("RESTARTSEQUENCE", true)
      myStats:ResetCooldown("RESTARTSEQUENCE")
	  if speedx < 0 then  
	    for i = 4, 6 do
          saws[i]:SetSelfActive(true)
        end
	  else
		for i = 1, 3 do
          saws[i]:SetSelfActive(true)
        end
	  end
    
      
	  saw4_aabb:SetOffset(vec2.new(0, 3))
      saw5_aabb:SetOffset(vec2.new(0, 2))
	  saw6_aabb:SetOffset(vec2.new(0, 1))
	  saw7_aabb:SetOffset(vec2.new(0, 3))
      saw8_aabb:SetOffset(vec2.new(0, 2))
	  saw9_aabb:SetOffset(vec2.new(0, 1))
		
      soundWindupFlag = 1
    --_G.audio:Play("Resource/Audio/Boss/BossFireballShot.wav", vec3.new(0,0,0), 0.0, false)
	end
  end
  --print(shockwave_sprite:GetFrameCount())
  if shockwave_sprite:GetFrameCount() == 8 then
    shockwave:SetSelfActive(false)
  end
  if drop == 1 then
    if soundWindupFlag == 1 then
      soundWindupFlag = 0
		  _G.audio:Play("Resource/Audio/Boss/BossFireballWindup.wav", vec3.new(0,0,0), 0.0, false)
      soundShootFlag = 1
	  end
    if sprite:GetFrameCount() == 7 then
      sprite:SetSprite("ATTACK1")
	  sprite:SetFrameCount(0)
	end
    CheckHurt()
    
	  if myStats:GetCooldown("RESTARTSEQUENCE") <= 0.0 then
	    restart = 1
      myStats:ActivateCooldown("RESTARTSEQUENCE", false)
	  end
    
	  if myStats:GetCooldown("RESTARTSEQUENCE") <= 3.0 then
      if speedx < 0 then
	      FireProjLeft()
	    else
	      FireProjRight()
	    end
      
      if soundShootFlag == 1 then
        _G.audio:Play("Resource/Audio/Boss/BossFireballShot.wav", vec3.new(0,0,0), 0.0, false)
        soundShootFlag = 0
      end
	  end
  end
  
  if restart == 1 then
    drop = 0
    rb:SetHasGravity(false)
	shield:SetSelfActive(true)
	  rb:SetVel(vec2.new(0.0, rb:GetVel().y + 0.5))
	  if myCollision:GetColTop() == true then
      rb:SetObjType("NONEXIST")
	    _G.audio:Play("Resource/Audio/Boss/BossSlam.wav", vec3.new(0,0,0), 20, false)
	    restart = 0
	    cam:SetCamShake(0.1,5,0.4)
	    LeaveState()
	  end
  end
  
  ProjCollisionHandler()
  
end

function FireProjRight()
  if saws[1]:GetActive() == true then
    saw4_aabb:SetOffset(vec2.new(saw4_aabb:GetOffset().x + speedx * 2.5, saw4_aabb:GetOffset().y))
  end
  if saws[2]:GetActive() == true then
    saw5_aabb:SetOffset(vec2.new(saw5_aabb:GetOffset().x + speedx * 2.5, saw5_aabb:GetOffset().y))
  end
  if saws[3]:GetActive() == true then
    saw6_aabb:SetOffset(vec2.new(saw6_aabb:GetOffset().x + speedx * 2.5, saw6_aabb:GetOffset().y))
  end
end

function FireProjLeft()
  if saws[4]:GetActive() == true then
    saw7_aabb:SetOffset(vec2.new(saw7_aabb:GetOffset().x + speedx * 2.5, saw7_aabb:GetOffset().y))
  end
  if saws[5]:GetActive() == true then
    saw8_aabb:SetOffset(vec2.new(saw8_aabb:GetOffset().x + speedx * 2.5, saw8_aabb:GetOffset().y))
  end
  if saws[6]:GetActive() == true then
    saw9_aabb:SetOffset(vec2.new(saw9_aabb:GetOffset().x + speedx * 2.5, saw9_aabb:GetOffset().y))
  end
end

function MoveToLeftOrRightPlatform()
  
  if transComp.Pos.x < -7 
  or transComp.Pos.x > 7 then
    inPosition = 1
	rb:SetVel(vec2.new(0.0, 0.0))
	print("INPOS")
  end
  
end

function ProjCollisionHandler()

  for i = 1, #saws do
    saw_col = saws[i]:GetPrefab():GetCollisionComponent()
    if saw_col:GetColCollider() == true 
	or saw_col:GetColEndGameBox() == true then
      saws[i]:SetSelfActive(false)
      saw_aabb = saws[i]:GetPrefab():GetAABBComponent()
      saw_aabb:SetOffset(vec2.new(0.0,0.0))
    end
  end
  
end

function CheckHurt()
	if myStats:GetCooldown("INVULN") <= 0.0 then
      if myCollision:GetColPlayerHitbox() == true then
        for i = 1, #saws do
          saws[i]:SetSelfActive(false)
        end
		shield:SetSelfActive(true)
	  	  states:SetCurrLogicState("HURT")
	  	  sprite:SetSprite("HURT")
        attackFlag = 0
        inPosition = 0
        drop = 0
        speedx = 0
        restart = 0
		windupFlag = false
	  end
	end
end

-- Called when attack ends: Choose what to do NextAttack
function LeaveState()
  attackFlag = 0
  inPosition = 0
  drop = 0
  speedx = 0
  restart = 0
  windupFlag = false
  myStats:ActivateCooldown("RESTARTSEQUENCE", false)
  states:SetCurrLogicState("WALK")
  sprite:SetSprite("WALK")

end
