
local moveVec = vec2.new(0.2, 0)
local originalPos
local moveDuration = 3
local beforeMovementPos
local targetPos
local currentMoveVec
local movingToPos = true

local transComp
local rb

local counter

function Start(sceneactor,player)
  AllComponents = sceneactor:GetPrefab()
	rb = AllComponents:GetRigidBodyComponent()
	transComp = AllComponents:GetTransformComponent()
	counter = moveDuration
	ChangeDirection(movingToPos)
	
end

function Update()
	if counter > 0 then
		counter = counter - _G.DT
		rb:SetVel(currentMoveVec)
		transComp:SetPosition(transComp:GetPosition() + rb:GetVel())
	else
		counter = moveDuration
		
		if movingToPos then movingToPos = false
		else movingToPos = true end
		
		ChangeDirection(movingToPos)
	end
  
end

function ChangeDirection(movingAwayFromHome)

	currentMoveVec = movingAwayFromHome and moveVec or -moveVec

end