
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
	mySaw = sceneactor
	
	isActive = false
	
	sawSpeed = rb:GetModA()
	sawTimer = 0
	startTime = rb:GetStartTime() -- for now startTime is direction of saw spawn
	maxDuration = rb:GetDuration() -- distance = sawSpeed * 0.9828 * duration
	readyToRespawnDuration = maxDuration + 1
	startPos = myTransform:GetPosition()
	
	initialOffset = rb:GetModB()
	delayTimer = 0
	
	isInit = false
	
	testDistance = 10
	distanceTimer = 0
	tracked = false
end


-- Called every frame.
function Update()
	sawTimer = sawTimer + _G.DT
	distanceTimer = distanceTimer + _G.DT
	delayTimer = delayTimer + _G.DT
	--SawCalculator()
	Initialize()
	SpawnSaw()
	DespawnSaw()

end

-- Show saw in scene when in editor mode
function Initialize()
	if(isInit == false) then
		--rb:SetVel(vec2.new(startTime * sawSpeed,0))
		isInit = true
		isActive = false
		mySaw:SetActive(false)
	end
end

function SpawnSaw()
	-- Spawn a saw once timer exceed
	--if(sawTimer > startTime) and (isActive == false) then 
	if(sawTimer > 0) and (isActive == false) and (delayTimer >= initialOffset) then 
		isActive = true
		mySaw:SetActive(true)
		myTransform:SetPosition(startPos)
		rb:SetVel(vec2.new(0,startTime * sawSpeed))
	end
end


function DespawnSaw()

	if(sawTimer > maxDuration) then
		isActive = false
		mySaw:SetActive(false)
		
		if (sawTimer > readyToRespawnDuration) then
			sawTimer = 0
			myTransform:SetPosition(startPos)
			rb:SetVel(vec2.new(0,0))
		end
		
	end
end

function SawCalculator()
	local pos = myTransform:GetPosition()
	local mag = (startPos - pos):Length()
	print(mag)
	if(mag >= testDistance) and (tracked == false) then
		tracked = true
		print ("Speed: ", sawSpeed, " Time: ", distanceTimer)
	end
	
end