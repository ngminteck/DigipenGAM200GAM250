--[[

This script controls the first attack of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Fall [DONE]
- Regen

TO DO: ENERGY COST, HEALTH INCREMENT

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
	waitForHealth = 1 -- set proper number when applicable
	done = false
	
end


-- Called every frame.
function Update()

	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	if (timePassed < waitForHealth) then
		
		timePassed = timePassed + _G.DT
		
		if _G.i:KeyUp(CVK_C) then
			timePassed = 0
			ChooseNextState()
		end
		
	else
	
		print("Healthup! RegenStartup")
		timePassed = 0
		--INCREMENT HEALTH
		--DECREMENT ENERGY
		--if INSUFFICIENT ENERGY then
		--ChooseNextState()
		--else states:SetCurrLogicState("REGEN") end
		states:SetCurrLogicState("REGEN")
    sprite:SetAnimation("REGEN",states)
	end

	CheckInterruptions()

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


-- Called when attack ends
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
