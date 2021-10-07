--[[

This script controls the first attack of the character.

Possible states this script can transition to:
- Attack2 [DONE]
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Big Punch [DONE]
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

	-- Variables for managing attack
	timePassed = 0
	inputAvailTime = 0.2
	attackDuration = 0.8 -- set proper number when applicable
	proceed = false
	done = false

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.2
	
end


-- Called every frame.
function Update()

	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, rb:GetVel().y))

	ExecuteAttack()
	CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()
	NextAttack()

end


-- Executes the first attack.
function ExecuteAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay and done == false then

		attackHitbox:SetActive(true)
		
		done = true
		timePassedForHitbox = -0.5

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()
		
	if attackHitbox:GetPrefab():GetCollisionComponent():GetColPlayerHitbox() == true then
		attackHitbox:SetActive(false)
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	-- Press attack again before attack ends
	if timePassed >= inputAvailTime and timePassed < attackDuration then

		if _G.i:KeyDown(CVK_X) then
			proceed = true
			attackHitbox:SetActive(false)
		end

	-- Attack ends
	elseif timePassed > attackDuration and proceed == false then

		attackHitbox:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then

		timePassed = 0
		done = false
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("FALL")

	end
	
	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
			
		timePassed = 0
		done = false
		states:SetCurrLogicState("HURT")
		
	end

end


-- Moves on to the second attack.
function NextAttack()

	if proceed == true and timePassed > attackDuration then

		timePassed = 0
		done = false
		proceed = false
		states:SetCurrLogicState("ATTACK2")

	end

end


-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		timePassed = 0
		done = false
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	else
		timePassed = 0
		done = false
		states:SetCurrLogicState("IDLE")
	end

end
