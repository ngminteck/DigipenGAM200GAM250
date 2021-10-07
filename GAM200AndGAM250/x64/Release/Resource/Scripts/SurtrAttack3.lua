--[[

This script controls the second attack of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Big Punch [DONE]
- Fall [DONE]

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("PlayerHitbox")

	-- Variables for managing attack
	timePassed = 0
	attackDuration = 1

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.2

end


-- Called every frame.
function Update()

	ExecuteAttack()
	CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()

end


-- Executes the third attack.
function ExecuteAttack()

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay then

		attackHitbox:SetActive(true)
		timePassedForHitbox = 0

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	if attackHitbox:GetPrefab():GetCollisionComponent():GetColEnemy() == true then
		print("i is colliding")
		attackHitbox:SetActive(false)
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	if timePassed >= attackDuration then

		timePassed = 0
		attackHitbox:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then
	
		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("FALL")
		
	end
	
	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
	
		timePassed = 0
		
		states:SetCurrLogicState("HURT")
		
	end

end



-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		states:SetCurrLogicState("WALK")
	elseif _G.i:KeyDown(CVK_CTRL) then
		states:SetCurrLogicState("BIGPUNCH")
	else
		states:SetCurrLogicState("IDLE")
	end

end
