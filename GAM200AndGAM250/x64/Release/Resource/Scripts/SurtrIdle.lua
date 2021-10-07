--[[

This script controls the idle of the character.

Possible states this script can transition to:
- Walk [DONE]
- Attack1 [DONE]
- Jump [DONE]
- BigPunch [DONE]
- Hurt [DONE]
- Fall [DONE]

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

end


-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, rb:GetVel().y))
	LeaveState()
	
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
		
	end
	
	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	end

	if _G.i:KeyDown(CVK_SPACE) and myCollision:GetColBottom() == true then
		states:SetCurrLogicState("JUMP")
	end

	if _G.i:KeyDown(CVK_X) then
		states:SetCurrLogicState("ATTACK1")
	end

	if _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	end

	if myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
	end

end
