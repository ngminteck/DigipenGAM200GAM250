local ScaleX = 30.8
local PosX = 15.4

local timer = 0.0

local StartScaleX = 0.0
local EndScaleX
local MaxScaleX   = 154.0
local StartPosX   = 200.0
local EndPosX
local MaxPosX     = 277.0

local ManaBallCount = 0

local count = 0.0;
-- Called at first frame.
function Start(sceneactor,Player)
  PlayerComponents = Player:GetPrefab()
  PlayerStats = PlayerComponents:GetStatsComponent()

  object = sceneactor:GetPrefab()
  
  ManaBarUI = sceneactor:GetChild("UIBarFill"):GetPrefab()
  ManaBarUITransform = ManaBarUI:GetTransformComponent()
  
  UIBall1 = sceneactor:GetChild("UIBall1")
  UIBall2 = sceneactor:GetChild("UIBall2")
  UIBall3 = sceneactor:GetChild("UIBall3")
  UIBall4 = sceneactor:GetChild("UIBall4")
  UIBall5 = sceneactor:GetChild("UIBall5")
  UIBall6 = sceneactor:GetChild("UIBall6")
  
  UIHPLarge1 = sceneactor:GetChild("UIHPLarge1")
  UIHPLarge2 = sceneactor:GetChild("UIHPLarge2")
  UIHPLarge3 = sceneactor:GetChild("UIHPLarge3")
  UIHPLarge4 = sceneactor:GetChild("UIHPLarge4")
  UIHPLarge5 = sceneactor:GetChild("UIHPLarge5")
end

-- Called every frame.
function Update()
  HP = PlayerStats:GetHP()
  MP = PlayerStats:GetMP()
  isDead = PlayerStats:GetDead()
  print(_G.TotalHits)
  if ManaBarUITransform.Scale.x >=MaxScaleX and _G.TotalHits>=5.0 then
    _G.TotalHits = _G.TotalHits -5.0
    if ManaBallCount ~= 6 then
      ManaBallCount = ManaBallCount +1
        CalculateManaBall()
    end
  end
  if ManaBallCount ~= 6 then
  CalculateManaBar()
  end
  CalculateHP()

end

function CalculateManaBall()
  if ManaBallCount == 0 then
  print(UIBall1)
    UIBall1:SetSelfActive(false)
    UIBall2:SetSelfActive(false)
    UIBall3:SetSelfActive(false)
    UIBall4:SetSelfActive(false)
    UIBall5:SetSelfActive(false)
    UIBall6:SetSelfActive(false)
  elseif ManaBallCount== 1 then
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(false)
    UIBall3:SetSelfActive(false)
    UIBall4:SetSelfActive(false)
    UIBall5:SetSelfActive(false)
    UIBall6:SetSelfActive(false)
  elseif ManaBallCount == 2 then
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(true)
    UIBall3:SetSelfActive(false)
    UIBall4:SetSelfActive(false)
    UIBall5:SetSelfActive(false)
    UIBall6:SetSelfActive(false)
  elseif ManaBallCount == 3 then
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(true)
    UIBall3:SetSelfActive(true)
    UIBall4:SetSelfActive(false)
    UIBall5:SetSelfActive(false)
    UIBall6:SetSelfActive(false)
  elseif ManaBallCount == 4 then
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(true)
    UIBall3:SetSelfActive(true)
    UIBall4:SetSelfActive(true)
    UIBall5:SetSelfActive(false)
    UIBall6:SetSelfActive(false)
  elseif ManaBallCount == 5 then
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(true)
    UIBall3:SetSelfActive(true)
    UIBall4:SetSelfActive(true)
    UIBall5:SetSelfActive(true)
    UIBall6:SetSelfActive(false)
  else
    UIBall1:SetSelfActive(true)
    UIBall2:SetSelfActive(true)
    UIBall3:SetSelfActive(true)
    UIBall4:SetSelfActive(true)
    UIBall5:SetSelfActive(true)
    UIBall6:SetSelfActive(true)
  end
end

function CalculateManaBar()
  EndScaleX = ScaleX*_G.TotalHits
  EndPosX   = _G.TotalHits*PosX + StartPosX
  DiffScale = EndScaleX - ManaBarUITransform.Scale.x
  DiffPos   = EndPosX   - ManaBarUITransform.Pos.x
  ManaBarUITransform.Scale.x = ManaBarUITransform.Scale.x + DiffScale * 0.5
  ManaBarUITransform.Pos.x   = ManaBarUITransform.Pos.x + DiffPos*0.5
end

function CalculateHP()
	if HP ==10 then
	UIHPLarge1:SetSelfActive(true)
	UIHPLarge2:SetSelfActive(true)
	UIHPLarge3:SetSelfActive(true)
	UIHPLarge4:SetSelfActive(true)
	UIHPLarge5:SetSelfActive(true)
	elseif HP == 8 then
	UIHPLarge1:SetSelfActive(true)
	UIHPLarge2:SetSelfActive(true)
	UIHPLarge3:SetSelfActive(true)
	UIHPLarge4:SetSelfActive(true)
	UIHPLarge5:SetSelfActive(false)
	elseif HP == 6 then
	UIHPLarge1:SetSelfActive(true)
	UIHPLarge2:SetSelfActive(true)
	UIHPLarge3:SetSelfActive(true)
	UIHPLarge4:SetSelfActive(false)
	UIHPLarge5:SetSelfActive(false)
	elseif HP == 4 then
	UIHPLarge1:SetSelfActive(true)
	UIHPLarge2:SetSelfActive(true)
	UIHPLarge3:SetSelfActive(false)
	UIHPLarge4:SetSelfActive(false)
	UIHPLarge5:SetSelfActive(false)
	
	elseif HP == 2 then
	UIHPLarge1:SetSelfActive(true)
	UIHPLarge2:SetSelfActive(false)
	UIHPLarge3:SetSelfActive(false)
	UIHPLarge4:SetSelfActive(false)
	UIHPLarge5:SetSelfActive(false)
	
	elseif HP == 0 then
	UIHPLarge1:SetSelfActive(false)
	UIHPLarge2:SetSelfActive(false)
	UIHPLarge3:SetSelfActive(false)
	UIHPLarge4:SetSelfActive(false)
	UIHPLarge5:SetSelfActive(false)
	end
end