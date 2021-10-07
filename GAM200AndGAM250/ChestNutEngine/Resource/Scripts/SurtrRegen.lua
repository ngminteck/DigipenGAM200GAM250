--[[

This script controls the first attack of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components/Children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("PlayerHitbox")
  sprite = AllComponents:GetSpriteComponent()
	-- Variables for managing attack
	timePassed = 0
	waitForHealth = 0.5 -- set proper number when applicable
	done = false
	
end


-- Called every frame.
function Update()

	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if (timePassed < waitForHealth) then
		
		timePassed = timePassed + _G.DT
		
	else
	
		print("Healthup! Regen")
		timePassed = 0
		--INCREMENT HEALTH
		--DECREMENT ENERGY
		--if INSUFFICIENT ENERGY then
		--ChooseNextState() end
		if _G.i:KeyDown(CVK_C) == false then
			ChooseNextState()
		end
	
	end

	CheckInterruptions()
	
	if _G.i:KeyUp(CVK_C) then
		timePassed = 0
		ChooseNextState()
	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then
	
		timePassed = 0
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	end
	
	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
			
		timePassed = 0
		done = false
		states:SetCurrLogicState("HURT")
		sprite:SetAnimation("HURT",states)
	end

end


function ChooseNextState()

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		timePassed = 0
		done = false
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")--]]
	else
		timePassed = 0
		done = false
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end

end
