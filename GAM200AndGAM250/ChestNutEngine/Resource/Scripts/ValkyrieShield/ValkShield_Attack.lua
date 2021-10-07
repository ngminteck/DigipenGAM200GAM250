
-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myTransform = AllComponents:GetTransformComponent()
	
	shieldObj = sceneactor:GetChild("Shield_Obj")
	shieldAABB = shieldObj:GetPrefab():GetAABBComponent()
	--shieldRB = shieldObj:GetPrefab():GetRigidBodyComponent()
	shieldObj:SetActive(false)
	
	shieldCol = sceneactor:GetChild("ShieldCollider")
	sColAABB = shieldCol:GetPrefab():GetAABBComponent()
	shieldCol:SetActive(false)
	
	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	
	-- Shield Variables
	
	-- Chase Variables
	facingRight = false
	
	-- Detection Variables
	detectRange = 9
	attackRange = 6
	inAttackRange = true
	inDetectRange = true
	--playerDetected = false
	
	-- Attack Variables
	shieldSpeed = 6
	shieldLocDelta = 0
	startLocation = vec2.new(0,0)
	targetLocation = vec2.new(0,0)
	shieldThrown = false
	shieldReturn = false
	myGravity = 0
	
	-- Hover Variables
	hoverDuration = 2.5
	hoverTimer = 0
	isHovering = false
	
end


-- Called every frame.
function Update()

	ThrowShield()
		
		if(isHovering) then
			HoverShield()
		else
			MoveShield()
		end
	--[[if(inAttackRange) then
		
	end
	
	if(isAttacking == false) then
		CheckForPlayer()
		ChooseNextState()
	end]]--
	
end

-- Check if the player is in range of enemy
function CheckForPlayer()

	--print("Checking PlayerPos")
	-- Get my position
	myPos = myTransform:GetPosition()
	-- Get player position
	playerPos = playerTransform:GetPosition()
	
	-- Check distance between you and player
	distance = (myPos - playerPos):Length()
	
	if(distance < attackRange) then
		inAttackRange = true
		inDetectRange = true
		
	elseif (distance < detectRange ) then
	
		inAttackRange = false
		inDetectRange = true
		
	else
	
		inAttackRange = false
		inDetectRange = false
		
	end
	
end

-- Checks if shield has been thrown
function ThrowShield()
	
	if(shieldThrown == false) then
		-- Spawn shield at valkyrie location
		shieldObj:SetActive(true)
		shieldAABB:SetOffset(vec2.new(0,0))
		targetLocation = myTransform:GetPosition() - playerTransform:GetPosition()
		targetLocation = vec2.new(targetLocation.x, -targetLocation.y)
		--rb:SetGravity(0)
		isAttacking = true
		shieldThrown = true
	end
	
end

-- Handles the movement of the shield
function MoveShield()
	
	if(shieldThrown) then
		if(shieldReturn == false) then
			--print("Shield Throwing")
			-- Increment position delta
			local currentPos = shieldAABB:GetOffset()
			local offset = MoveTowards(currentPos, targetLocation, shieldSpeed / 100)
			shieldAABB:SetOffset(offset)
			sColAABB:SetOffset(offset)
			local posDiff = (currentPos - targetLocation)
			local posMag = posDiff:Length()
			--print(posMag)
			-- If shield reach target loc, enable hovering
			if (posMag < 0.9)then
				shieldCol:SetActive(true)
			end
			
			
			if(posMag < 0.3) then
				isHovering = true
			end
		else
			--print("Shield Returning")
			local currentPos = shieldAABB:GetOffset()
			local offset = MoveTowards(currentPos, vec2.new(0,0), shieldSpeed / 100)
			shieldAABB:SetOffset(offset)
			sColAABB:SetOffset(offset)
			--shieldTransform:SetPosition(MoveTowards(pos, startLocation, shieldSpeed / 100))
			
			local posDiff = (currentPos - startLocation)
			local posMag = posDiff:Length()
			
			--print(posMag)
			
			if (posMag < 0.4)then
				shieldCol:SetActive(false)
			end
			-- If shield reach target loc, enable hovering
			if(posMag < 0.3) then
				shieldReturn = false
				shieldThrown = false
				isAttacking = false
				shieldObj:SetActive(false)
			end
		
		end
	end
end

-- Make shield hover at position for duration
function HoverShield()

	--print("Hovering Shield")
	hoverTimer = hoverTimer + _G.DT
	
	if(hoverTimer > hoverDuration) then
		hoverTimer = 0
		shieldReturn = true
		isHovering = false
	end
	
	return (current - target) * t  
end

-- Clamped Lerping for shield movement.
function MoveTowards(current, target, maxDistanceDelta)

	local a = target - current
	local magnitude = a:Length()
	if(magnitude <= maxDistanceDelta) or (magnitude == 0) then
		return target
	end
	
	return current + (a / magnitude * maxDistanceDelta)
end

function ChooseNextState()
	
	rb:SetGravity(myGravity)
	
	if (inDetectRange) then
		states:SetCurrLogicState("CHASE")
	elseif (inDetectRange == false) and (inAttackRange == false) then
		states:SetCurrLogicState("IDLE")
	end
	
end

-- Helper function for lerping vectors
--** Not in use now
function LerpVector(current, target, t)

	local posX = LerpFloat(current.x, target.x, t)
	local posY = LerpFloat(current.y, target.y, t)
	return vec2.new(posX,posY)
	
end

-- Helper function for lerping floats
--** Not in use now
function LerpFloat(current, target, t)
	return (target - current) * t
end
