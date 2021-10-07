
local transComp
local moveComp

local counter = 0

function Start(AllComponents)
transComp = AllComponents:GetTransformComponent()
moveComp = AllComponents:GetMovementComponent()
--print(TransComp.scale.x)
end

function Update()
	counter = counter + _G.DT
  --print(counter)
--print("In Idle State")
--print(transComp:getscale())
--print("HIAAA")
if _G.i:KeyDown(CVK_LEFT)
then
print("walking")
moveComp:SetCurrLogicState("WALK")
end
--transComp:SetPosition(transComp:GetPosition().x)
--temp = vec2.new(transComp:GetPosition().x+0.1,transComp:GetPosition().y)
--
--transComp.Pos.x = transComp.Pos.x+0.5
end
