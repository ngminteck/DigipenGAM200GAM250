

-- Called at first frame.
function Start(sceneactor)

	-- Get components/Children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	
	saws = { sceneactor:GetChild("Saw1"), sceneactor:GetChild("Saw2"), 
	         sceneactor:GetChild("Saw3") }
  
  saw1_aabb = saws[1]:GetPrefab():GetAABBComponent()
  saw2_aabb = saws[2]:GetPrefab():GetAABBComponent()
  saw3_aabb = saws[3]:GetPrefab():GetAABBComponent()
  
  sawsend = { sceneactor:GetChild("End1"), sceneactor:GetChild("End2"), sceneactor:GetChild("End3") }
  
  for i = 1, #saws do
    saws[i]:SetSelfActive(false)
  end
  attackFlag = 0
  fireFlag = 0
  reloadDur = 3
  attackChoices = 1
  attackCounter = 0
  soundFlag = 0
  projendCounter = 0
end


-- Called every frame.
function Update()
  if attackFlag == 0 then
    if attackCounter == 1 then
      LeaveState()
      return
    end
    --print("Resource\Audio\Boss")
    _G.audio:Play("Resource/Audio/Boss/BossFireballWindup.wav", vec3.new(0,0,0), 0.0, false)
    attackFlag = 1
	  rb:SetVel(vec2.new(0.0,0.0))
    saws[1]:SetSelfActive(true)
    saws[2]:SetSelfActive(true)
    saws[3]:SetSelfActive(true)
    saws[1]:GetPrefab():GetSpriteComponent():SetFrameCount(20)
    saws[2]:GetPrefab():GetSpriteComponent():SetFrameCount(20)
    saws[3]:GetPrefab():GetSpriteComponent():SetFrameCount(20)
	  myStats:ResetCooldown("RELOADDUR")
	  myStats:ActivateCooldown("RELOADDUR", true)
    attackChoices = math.random(2)
    attackCounter = attackCounter + 1
    soundFlag = 1
  end
	
  --end
  if myStats:GetCooldown("RELOADDUR") <= 2.0 then
    if soundFlag == 1 then
      _G.audio:Play("Resource/Audio/Boss/BossFireballShot.wav", vec3.new(0,0,0), 0.0, false)
      soundFlag = 0
    end
    if attackChoices == 1 then
      FireTriProj()
    else
      FireTriProj()
    end
  end
  ProjCollisionHandler()
  
  
  if myStats:GetCooldown("RELOADDUR") <= 0.0 then
    attackFlag = 0
  end

end

function FireTriProj()
  m = 0.05
  speedx = 0.1
  speedy = 0.1
  
  if saws[1]:GetActive() == true then
    saw1_aabb:SetOffset(vec2.new(saw1_aabb:GetOffset().x + speedx - m, saw1_aabb:GetOffset().y - speedy))
  end
  if saws[2]:GetActive() == true then
    saw2_aabb:SetOffset(vec2.new(saw2_aabb:GetOffset().x + speedx - 2 * m, saw2_aabb:GetOffset().y - speedy * 1.2))
  end
  if saws[3]:GetActive() == true then
    saw3_aabb:SetOffset(vec2.new(saw3_aabb:GetOffset().x + speedx - 3 * m, saw3_aabb:GetOffset().y - speedy))
  end
end

function FireQuadProj()
  m = 0.04
  speedx = 0.1
  speedy = 0.1
  if saws[1]:GetActive() == true then
    saw1_aabb:SetOffset(vec2.new(saw1_aabb:GetOffset().x + speedx - m, saw1_aabb:GetOffset().y - speedy))
  end
  if saws[2]:GetActive() == true then
    saw2_aabb:SetOffset(vec2.new(saw2_aabb:GetOffset().x + speedx - 2 * m, saw2_aabb:GetOffset().y - speedy * 1.2))
  end
  if saws[3]:GetActive() == true then
    saw3_aabb:SetOffset(vec2.new(saw3_aabb:GetOffset().x + speedx - 3 * m, saw3_aabb:GetOffset().y - speedy * 1.2))
  end
  if saws[4]:GetActive() == true then
    saw4_aabb:SetOffset(vec2.new(saw4_aabb:GetOffset().x + speedx - 4 * m, saw4_aabb:GetOffset().y - speedy))
  end
  
end

function ProjCollisionHandler()

  for i = 1, #saws do
    saw_col = saws[i]:GetPrefab():GetCollisionComponent()
	sawend_sprite = sawsend[i]:GetPrefab():GetSpriteComponent()
	--print(sawend_sprite:GetFrameCount())
	if sawend_sprite:GetFrameCount() == 6 then
	  sawsend[i]:SetSelfActive(false)
	end
    if saw_col:GetColCollider() == true 
	or saw_col:GetColPitfall() == true then
      saws[i]:SetSelfActive(false)
	  sawsend[i]:SetSelfActive(true)
	  
      saw_aabb = saws[i]:GetPrefab():GetAABBComponent()
	  sawend_aabb = sawsend[i]:GetPrefab():GetAABBComponent()
	  sawend_aabb:SetOffset(vec2.new(saw_aabb:GetOffset().x, saw_aabb:GetOffset().y - 0.2))
	  
      saw_aabb:SetOffset(vec2.new(0.0,0.0))
	  sawend_sprite:SetFrameCount(0)
    end
	
  end
  
end

-- Called when attack ends: Choose what to do NextAttack
function LeaveState()
  attackFlag = 0
  attackCounter = 0
  for i = 1, #saws do
    saws[i]:SetSelfActive(false)
  end
  states:SetCurrLogicState("WALK")
  sprite:SetSprite("WALK")

end
