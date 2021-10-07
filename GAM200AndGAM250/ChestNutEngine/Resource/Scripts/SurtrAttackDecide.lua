--[[

This script detects how long the player has held the attack button
and decides whether to perform a charged attack or a normal one.

Possible states this script can transition to:
- Attack1			[DONE]
- ChargedAttack

]]--

function Start(sceneactor)
	
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	transComp = AllComponents:GetTransformComponent()
	myStats = AllComponents:GetStatsComponent()
  sprite = AllComponents:GetSpriteComponent()
	timer = 0
	
	chargedAttackUnlocked = true

end

-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, rb:GetVel().y))
	
	if chargedAttackUnlocked == false then
	
		states:SetCurrLogicState("ATTACK1")
		sprite:SetAnimation("ATTACK1",states)
	end
	
	if (timer < 1) then
	
		timer = timer + _G.DT
		
		if _G.i:KeyUp(KEY_ATTACK) then
		
			states:SetCurrLogicState("ATTACK1")
      sprite:SetAnimation("ATTACK1",states)
			timer = 0
			
		end
	
	else
	
		states:SetCurrLogicState("CHARGEDATTACK")
    sprite:SetAnimation("CHARGEDATTACK",states)
		timer = 0
		
	end
	
		LeaveState()
	
end

-- Chooses the next state if not attacking
function LeaveState()

	if myCollision:GetColEnemyHitbox() == true
    or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true 
  or myCollision:GetColTrigger() == true then
						
		states:SetCurrLogicState("HURT")
    sprite:SetAnimation("HURT",states)
		
	end
	
	if myCollision:GetColBottom() == false then
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	end

end