local ScaleX = 30.8
local PosX = 15.4

local timer = 0.0

local StartScaleX = 0.0
local EndScaleX
local MaxScaleX   = 154.0
local StartPosX   = 200.0
local EndPosX
local MaxPosX     = 277.0

local HP = 0

local count = 0.0

local timer = 0
local flag = 0
-- Called at first frame.
function Start(sceneactor,Player)
  PlayerComponents = Player:GetPrefab()
  PlayerStats = PlayerComponents:GetStatsComponent()
  HP = PlayerStats:GetHP()

  object = sceneactor:GetPrefab()
  
  --ManaBarUI = sceneactor:GetChild("UIBarFill"):GetPrefab()
  --ManaBarUITransform = ManaBarUI:GetTransformComponent()
  
  UIHPFire1 = sceneactor:GetChild("UIHPFire1")
  UIHPFire1_sprite = UIHPFire1:GetPrefab():GetSpriteComponent()
  
  UIHPFire2 = sceneactor:GetChild("UIHPFire2")
  UIHPFire2_sprite = UIHPFire2:GetPrefab():GetSpriteComponent()
  
  UIHPFire3 = sceneactor:GetChild("UIHPFire3")
  UIHPFire3_sprite = UIHPFire3:GetPrefab():GetSpriteComponent()
  
  UIHPFire4 = sceneactor:GetChild("UIHPFire4")
  UIHPFire4_sprite = UIHPFire4:GetPrefab():GetSpriteComponent()
  
  UIHPFire5 = sceneactor:GetChild("UIHPFire5")
  UIHPFire5_sprite = UIHPFire5:GetPrefab():GetSpriteComponent()
  
  resetFireFlag = false
end

-- Called every frame.
function Update()
    
  if PlayerStats:GetHP() < HP then
    resetFireFlag = false
	  flag = 1
  end
  
  HP = PlayerStats:GetHP()
  if _G.i:KeyDown(CVK_M) 
    then PlayerStats:SetHP(HP-1)
  end 
  
  --print(HP)
  --GOD MODE ON
  if _G.i:KeyDown(CVK_G) then
    PlayerStats:SetHP(9000)
  end
    
  if flag == 1 then
    CalculateHP()
  end
  

end

function CalculateHP()
  if HP == 0 then
	  UIHPFire1_sprite:SetSprite("Health_Fade_Out")
    --UIHPFire1_sprite:SetFrameCount(11)
	if timer == 22 then
		UIHPFire1:SetSelfActive(false)
    UIHPFire2:SetSelfActive(false)
    UIHPFire3:SetSelfActive(false)
    UIHPFire4:SetSelfActive(false)
    UIHPFire5:SetSelfActive(false)
		timer = 0
		flag = 0
	end
	timer = timer + 1
  elseif HP == 1 then
	  UIHPFire2_sprite:SetSprite("Health_Fade_Out")
    --UIHPFire2_sprite:SetFrameCount(11)
	if timer == 22 then
		UIHPFire1:SetSelfActive(true)
    UIHPFire2:SetSelfActive(false)
    UIHPFire3:SetSelfActive(false)
    UIHPFire4:SetSelfActive(false)
    UIHPFire5:SetSelfActive(false)
		timer = 0
		flag = 0
	end
	timer = timer + 1
  elseif HP == 2 then
	  UIHPFire3_sprite:SetSprite("Health_Fade_Out")
    --UIHPFire3_sprite:SetFrameCount(11)
	if timer == 22 then
    UIHPFire1:SetSelfActive(true)
    UIHPFire2:SetSelfActive(true)
    UIHPFire3:SetSelfActive(false)
    UIHPFire4:SetSelfActive(false)
		UIHPFire5:SetSelfActive(false)
		timer = 0
		flag = 0
	end
	timer = timer + 1
  elseif HP == 3 then
	  UIHPFire4_sprite:SetSprite("Health_Fade_Out")
    --UIHPFire4_sprite:SetFrameCount(11)
	if timer == 22 then
    UIHPFire1:SetSelfActive(true)
    UIHPFire2:SetSelfActive(true)
    UIHPFire3:SetSelfActive(true)
    UIHPFire4:SetSelfActive(false)
		UIHPFire5:SetSelfActive(false)
		timer = 0
		flag = 0
	end
	timer = timer + 1
  elseif HP == 4 then
	  UIHPFire5_sprite:SetSprite("Health_Fade_Out")
    --UIHPFire5_sprite:SetFrameCount(11)
	if timer == 22 then
    UIHPFire1:SetSelfActive(true)
    UIHPFire2:SetSelfActive(true)
    UIHPFire3:SetSelfActive(true)
    UIHPFire4:SetSelfActive(true)
		UIHPFire5:SetSelfActive(false)
		timer = 0
		flag = 0
	end
	timer = timer + 1
  else
    if resetFireFlag == false then
      resetFireFlag = true
	    UIHPFire1_sprite:SetSprite("Health_Idle")
      UIHPFire2_sprite:SetSprite("Health_Idle")
      UIHPFire3_sprite:SetSprite("Health_Idle")
      UIHPFire4_sprite:SetSprite("Health_Idle")
      UIHPFire5_sprite:SetSprite("Health_Idle")
      UIHPFire1_sprite:SetFrameCount(11)
      UIHPFire2_sprite:SetFrameCount(11)
      UIHPFire3_sprite:SetFrameCount(11)
      UIHPFire4_sprite:SetFrameCount(11)
      UIHPFire5_sprite:SetFrameCount(11)
      UIHPFire1:SetSelfActive(true)
      UIHPFire2:SetSelfActive(true)
      UIHPFire3:SetSelfActive(true)
      UIHPFire4:SetSelfActive(true)
		  UIHPFire5:SetSelfActive(true)  
    end
    timer = 0
    flag = 0
  end
end
