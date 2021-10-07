--[[

This script controls the landing of the character.

It merely plays the landing animation and decides what state
to transit into.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- AttackDecide [DONE]
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
	sprite = AllComponents:GetSpriteComponent()
  
end


-- Called every frame.
function Update()
	
	ChooseNextAction()

end

-- Chooses the next action
function ChooseNextAction()

	--if myCollision:GetColEndGameBox() == true then
	--	states:SetCurrLogicState("WIN")
    --sprite:SetAnimation("WALK",states)
	--end
	
	-- If movement keys pressed, go to Walk State.
	if _G.i:KeyDown(KEY_LEFT) or _G.i:KeyDown(KEY_RIGHT) then
			timePassed = 0
		effectTimer = effectDuration
		effectObject:SetActive(false)
		states:SetCurrLogicState("WALK")
		sprite:SetAnimation("WALK",states)

	--[[elseif _G.i:KeyDown(CVK_X) then
		effectTimer = effectDuration
		effectObject:SetActive(false)
		states:SetCurrLogicState("ATTACK1")
		sprite:SetAnimation("ATTACK1",states)--]]
	--[[elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
    sprite:SetAnimation("BIGPUNCH",states)]]--

    -- If no input, go to Idle State.
	elseif rb:GetVel().y <= 0 then
		effectTimer = effectDuration
		effectObject:SetActive(false)
		print("Idle transition")
			timePassed = 0
		states:SetCurrLogicState("IDLE")
		sprite:SetAnimation("IDLE",states)
	end
end
