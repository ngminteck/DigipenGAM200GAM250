--[[

This script controls the landing of the character.

It merely plays the landing animation and decides what state
to transit into.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Attack1 [DONE]
- Jump [DONE]
- BigPunch [DONE]
- Hurt [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()

end


-- Called every frame.
function Update()
	
	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
	
		states:SetCurrLogicState("HURT")
		
	elseif _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	elseif _G.i:KeyDown(CVK_SPACE) then
		states:SetCurrLogicState("JUMP")
	else
		states:SetCurrLogicState("IDLE")
	end

end
