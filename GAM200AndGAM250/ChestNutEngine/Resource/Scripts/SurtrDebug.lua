--[[

This script controls the idle of the character.

Possible states this script can transition to:
- Walk [DONE]
- AttackDecide [DONE]
- Jump [DONE]
- BigPunch [DONE]
- Hurt [DONE]
- Fall [DONE]
- Regen

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	transComp = AllComponents:GetTransformComponent()
	myStats = AllComponents:GetStatsComponent()
    sprite = AllComponents:GetSpriteComponent()
    colObj = myCollision:GetCollidedObject()
	
	savePoint = vec2.new(transComp:GetPosition().x, transComp:GetPosition().y)

	speed = 0.1 -- noclip speed
	speedMult = 4 -- noclip sprint speed
	debugActive = true
	originalGravity = 50
	
end


-- Called every frame.
function Update()

	NoClipMovement()
	DeactiveState()
	
end

function NoClipMovement()
	
	rb:SetVel(vec2.new(0, 0))
	rb:SetAccel(vec2.new(0, 0))
	rb:SetGravity(0)
	rb:SetHasGravity(false)
	
	if(_G.i:KeyDown(CVK_CTRL)) then
	
		if (_G.i:KeyDown(KEY_LEFT)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x - (speed * speedMult), transComp:GetPosition().y))
		end

		if (_G.i:KeyDown(KEY_RIGHT)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x + (speed * speedMult), transComp:GetPosition().y))
		end
		
		if (_G.i:KeyDown(KEY_UP)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x, transComp:GetPosition().y + (speed * speedMult)))
		end
		
		if (_G.i:KeyDown(KEY_DOWN)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x, transComp:GetPosition().y - (speed * speedMult)))
		end

	else

		if (_G.i:KeyDown(KEY_LEFT)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x - speed, transComp:GetPosition().y))
		end

		if (_G.i:KeyDown(KEY_RIGHT)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x + speed, transComp:GetPosition().y))
		end
		
		if (_G.i:KeyDown(KEY_UP)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x, transComp:GetPosition().y + speed))
		end
		
		if (_G.i:KeyDown(KEY_DOWN)) then
			transComp:SetPosition(vec2.new(transComp:GetPosition().x, transComp:GetPosition().y - speed))
		end

	end
	
	
	if _G.i:KeyDown(CVK_J) then
		savePoint = vec2.new(transComp:GetPosition().x, transComp:GetPosition().y)
	end
	
	if _G.i:KeyDown(CVK_K) then
		transComp:SetPosition(savePoint)
	end


	if (_G.i:KeyTrig(CVK_6)) and (debugActive == false) then
	
		debugActive = true
		rb:SetGravity(originalGravity)
		rb:SetHasGravity(true)
    rb:SetObjType("PLAYER")
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
	
	end
	
end


function RespawnChecker()
	
	for i = 1, #colObj do
		if colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "RESPAWNBOX" then
			_G.respawnPoint = vec2.new(transComp:GetPosition().x, transComp:GetPosition().y)
		elseif colObj[i]:GetPrefab():GetRigidBodyComponent():GetObjType() == "PITTRAPS" then
			states:SetCurrLogicState("HURT")
			sprite:SetAnimation("HURT",states)
			--transComp:SetPosition(_G.respawnPoint)
			break
		end
	end
end

-- Conditions to leave the current state.
function LeaveState()
	
	--if myCollision:GetColEnemyHitbox() == true
	--or myCollision:GetColEnemyDashHitbox() == true
	--or myCollision:GetColEnemyCounterHitbox() == true then
	
	if myCollision:GetColEnemyHitbox() == true
    or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
						
		states:SetCurrLogicState("HURT")
		sprite:SetAnimation("HURT",states)
	end
	
	if _G.i:KeyDown(KEY_LEFT) or _G.i:KeyDown(KEY_RIGHT) then
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)

	end

	if _G.i:KeyDown(KEY_JUMP) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("JUMP")
    sprite:SetAnimation("JUMP",states)

	end

	--[[if _G.i:KeyDown(CVK_X) then
		--states:SetCurrLogicState("ATTACK1")
		states:SetCurrLogicState("ATTACKDECIDE")
    sprite:SetAnimation("ATTACKDECIDE",states)
	end--]]
	
	--[[if _G.i:KeyDown(CVK_C) then
		--if SUFFICIENT ENERGY then
		states:SetCurrLogicState("REGENSTARTUP")
    sprite:SetAnimation("REGENSTARTUP",states)
	end]]--
	
	if _G.canDash and _G.i:KeyDown(KEY_DASH) then
		states:SetCurrLogicState("DASH")
    sprite:SetAnimation("DASH",states)
	end
	
	--[[if _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	end--]]

	if myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	end

end

function DeactiveState()
	if(debugActive == true) and (_G.i:KeyUp(CVK_6)) then
  
    rb:SetObjType("NONEXIST")
		debugActive = false
	end
end

