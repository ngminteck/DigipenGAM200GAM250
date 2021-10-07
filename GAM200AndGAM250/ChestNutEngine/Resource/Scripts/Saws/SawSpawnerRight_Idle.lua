
-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	sprite = AllComponents:GetSpriteComponent()
	rb = AllComponents:GetRigidBodyComponent()

	-- Get timing variables
	interval = rb:GetModA()
	timePassed = 0
	
  Saw1 = sceneactor:GetChild("Saw1")
  Saw1_rb = Saw1:GetPrefab():GetRigidBodyComponent()
  Saw1_aabb = Saw1:GetPrefab():GetAABBComponent()
  Saw1_col = Saw1:GetPrefab():GetCollisionComponent()
  Saw1_sprite = Saw1:GetPrefab():GetSpriteComponent()
  Saw1:SetSelfActive(false)
    
  Saw2 = sceneactor:GetChild("Saw2")  
  Saw2_rb = Saw2:GetPrefab():GetRigidBodyComponent()
  Saw2_aabb = Saw2:GetPrefab():GetAABBComponent()
  Saw2_col = Saw2:GetPrefab():GetCollisionComponent()
  Saw2_sprite = Saw2:GetPrefab():GetSpriteComponent()
  Saw2:SetSelfActive(false)
  speed = rb:GetModB()
  
  player_mov = player:GetPrefab():GetMovementComponent()
  playerhitbox =  player:GetChild("PlayerHitbox")
  
end


-- Called every frame.
function Update()

	sprite:SetSprite("SAW_SPAWNER_FACERIGHT", states)
	timePassed = timePassed + _G.DT
	if timePassed >= interval then
	
		timePassed = 0
		states:SetCurrLogicState("SHOOT")
  
	end
  
    if Saw1_col:GetColCollider() == true then
      Saw1:SetSelfActive(false)
      _G.mod1 = 1
    end
    if Saw1_col:GetColPlayerHitbox() == true then
      playerhitbox:SetSelfActive(false)
      if player_mov:GetCurrDirection() == 1 then
        _G.mod1 = -1
      else
        _G.mod1 = 1.5
      end
    end
    
    if Saw1:GetActive() == true then 
      Saw1_aabb:SetOffset(vec2.new(_G.mod1*-speed+Saw1_aabb:GetOffset().x,Saw1_aabb:GetOffset().y))
    end
    
    if Saw2_col:GetColCollider() == true then
      Saw2:SetSelfActive(false)
      _G.mod2 = 1
    end
    
    if Saw2_col:GetColPlayerHitbox() == true then
      playerhitbox:SetSelfActive(false)
      if player_mov:GetCurrDirection() == 1 then
        _G.mod2 = -1
      else
        _G.mod2 = 1.5
      end
    end
    
    if Saw2:GetActive() == true then 
      Saw2_aabb:SetOffset(vec2.new(_G.mod2*-speed+Saw2_aabb:GetOffset().x,Saw2_aabb:GetOffset().y))
    end
end