

-- Called at first frame.
function Start(sceneactor,player)

  -- Get components/Children
  AllComponents = sceneactor:GetPrefab()
  states = AllComponents:GetMovementComponent()
  rb = AllComponents:GetRigidBodyComponent()
  myCollision = AllComponents:GetCollisionComponent()
  myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
  
  cam = player:GetPrefab():GetCameraComponent():GetCamera()
  camShakeDistance = 0
  camShakeFrequency = 0
  camShakeDuration = 0
  
  spikeSet1 = { sceneactor:GetChild("Spike1"), sceneactor:GetChild("Spike2"), 
	         sceneactor:GetChild("Spike3"), sceneactor:GetChild("Spike4"), sceneactor:GetChild("Spike5") }
  
  spikeSet2 = { sceneactor:GetChild("Spike6"), sceneactor:GetChild("Spike7"), 
	         sceneactor:GetChild("Spike8"), sceneactor:GetChild("Spike9"), sceneactor:GetChild("Spike10"),
			 sceneactor:GetChild("Spike11"), sceneactor:GetChild("Spike12"), sceneactor:GetChild("Spike13") }
			 
  spikeSet3 = { sceneactor:GetChild("Spike14"), sceneactor:GetChild("Spike15"), 
	         sceneactor:GetChild("Spike16"), sceneactor:GetChild("Spike17"), sceneactor:GetChild("Spike18") }
			 
  for i = 1, #spikeSet1 do
    spikeSet1[i]:SetSelfActive(false)
  end
  for i = 1, #spikeSet2 do
    spikeSet2[i]:SetSelfActive(false)
  end
  for i = 1, #spikeSet3 do
    spikeSet3[i]:SetSelfActive(false)
  end
  attackFlag = 0
  spikeOutFlag = 0
  spikePeekFlag = 0
  peekAnim = 0
  outAnim = 0
  
  local spikeSet
end


-- Called every frame.
function Update()
  if attackFlag == 0 then
    print("ATTACK2")
    attackFlag = 1
	rb:SetVel(vec2.new(0.0,0.0))
	myStats:ResetCooldown("SPIKEPEEK")
	myStats:ActivateCooldown("SPIKEPEEK", true)
	myStats:ResetCooldown("SPIKEOUT")
	myStats:ActivateCooldown("SPIKEOUT", true)
  spikePeekFlag = 1
  camShakeDistance = 0.05
  camShakeFrequency = 5
  camShakeDuration = 0.1
	rand = math.random(3)
	if rand == 1 then
	  spikeSet = spikeSet1
	elseif rand == 2 then
	  spikeSet = spikeSet2
	else
	  spikeSet = spikeSet3
	end
	
	SpikePeek()
  end
	
  --if spikePeekFlag == 1 then
  --  SpikePeek()
  --end
  
  if myStats:GetCooldown("SPIKEPEEK") <= 0.0 then
    --print("SPIKEOUT")
    spikePeekFlag = 0
    peekAnim = 0
    spikeOutFlag = 1
    myStats:ResetCooldown("SPIKEPEEK")
    myStats:ActivateCooldown("SPIKEPEEK", false)
    camShakeDistance = 0.2
    camShakeFrequency = 5
    camShakeDuration = 0.1
    _G.audio:Play("Resource/Audio/Boss/BossBurst.wav", vec3.new(0,0,0), 0.0, false)
  end
  
  if spikeOutFlag == 1 then    
    SpikeOut()
  end
  

  if myStats:GetCooldown("SPIKEOUT") <= 0.0 then
  --print("LEAVE")
  for i = 1, #spikeSet do
  
    spikeSet[i]:GetPrefab():GetSpriteComponent():SetFrameCount(13)
    spike_trans = spikeSet[i]:GetPrefab():GetTransformComponent()
    spike_trans:SetScale(vec3.new(spike_trans:GetScale().x, 1, spike_trans:GetScale().z))
    spikeSet[i]:SetSelfActive(false)
  end
    spikeOutFlag = 0
    outAnim = 0
	  myStats:ResetCooldown("SPIKEOUT")
	  myStats:ActivateCooldown("SPIKEOUT", false)
    LeaveState()
  end
  
	cam:SetCamShake(camShakeDistance, camShakeFrequency, camShakeDuration)
end

function SpikePeek()

  _G.audio:Play("Resource/Audio/Boss/BossBurstWindup.wav", vec3.new(0,0,0), 0.0, false)
  spikePeekFlag = 0
  print("PEEK")
  for i = 1, #spikeSet do
    --spike_rb = spikeSet[i]:GetPrefab():GetRigidBodyComponent()
 	  spike_trans = spikeSet[i]:GetPrefab():GetTransformComponent()
    spike_rb = spikeSet[i]:GetPrefab():GetRigidBodyComponent()
    
    
    spike_rb:SetObjType("NONEXISTBOSSSPIKES")
    --spike_trans:SetPosition(vec2.new(spike_trans.Pos.x, spike_trans.Pos.y + 0.7))
    spikeSet[i]:SetSelfActive(true)
  end
end

function SpikeOut()
--print("OUT")
  for i = 1, #spikeSet do
    spike_rb = spikeSet[i]:GetPrefab():GetRigidBodyComponent()
    spike_trans = spikeSet[i]:GetPrefab():GetTransformComponent()
    spike_rb:SetObjType("BOSSSPIKES")
    if outAnim >= 60 then
      --spike_trans:SetScale(vec3.new(spike_trans:GetScale().x, spike_trans:GetScale().y - 2.8, spike_trans:GetScale().z))
      --spike_rb:SetVel(vec2.new(0.0, -0.7))
    else
      spike_trans:SetScale(vec3.new(spike_trans:GetScale().x, spike_trans:GetScale().y + 1, spike_trans:GetScale().z))
      --spike_rb:SetVel(vec2.new(0.0, 0.5))
    end
  end
  outAnim = outAnim + 1
end

-- Called when attack ends: Choose what to do NextAttack
function LeaveState()
  attackFlag = 0
	states:SetCurrLogicState("WALK")
  sprite:SetSprite("WALK")

end
