
-- Called at first frame.
function Start(sceneactor,player)
	
	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myTransform = AllComponents:GetTransformComponent()

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	myActor = sceneactor
	
	inReverse = false
	
	sawSpeed = 5
	sawTimer = 0
	sawDirection = rb:GetStartTime() -- rename startTime to sawDirection
	maxDuration = rb:GetDuration()
	startPos = myTransform:GetPosition()
	
end


-- Called every frame.
function Update()
	
	sawTimer = sawTimer + _G.DT
	MoveSaw()
	--print(inReverse)
end

function MoveSaw()
	-- Set speed to 0
	rb:SetVel(vec2.new(0,0))
	
	-- Move saw in direction till timer expires
	if(inReverse == false ) then
		rb:SetVel(vec2.new(sawDirection * sawSpeed, 0))
	else
		rb:SetVel(vec2.new(-sawDirection * sawSpeed, 0))
	end
	
	if (sawTimer > maxDuration) then
		sawTimer = 0
		inReverse = not inReverse
		print(inReverse)
	end
end

