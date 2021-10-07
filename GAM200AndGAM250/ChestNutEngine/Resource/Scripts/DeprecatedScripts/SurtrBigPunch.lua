--[[

This script controls the combat ability of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
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
	myStats = AllComponents:GetStatsComponent()
	attackHitbox = sceneactor:GetChild("BigPunchHitbox")
	punchAnim = sceneactor:GetChild("SurtrSummonPunch")
  sprite = AllComponents:GetSpriteComponent()
	-- Variables for managing attack
	timePassed = 0
	attackDuration = 1.5 -- set proper number when applicable

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.7
	
	-- Variables for flipping
	xValue = 8
	
	-- Variables for mana-consumption
	manaConsumed = false

end


-- Called every frame.
function Update()

	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, rb:GetVel().y))
	
	if myStats:GetMP() > 1 then
		ExecuteAttack()
	end
	
	CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()

end


-- Executes the attack.
function ExecuteAttack()

	-- Mana
	ConsumeMana()

	-- Animated sprite
	CheckDirection()
	AttackAnimation()

	-- Hitbox
	timePassedForHitbox = timePassedForHitbox + _G.DT
	if timePassedForHitbox >= hitboxDelay then
		attackHitbox:SetActive(true)
		timePassedForHitbox = 0
	end	

end


-- Consumes mana when using the ability
function ConsumeMana()

	if manaConsumed == false then
		myStats:SetMP(myStats:GetMP() - 1)
		manaConsumed = true
	end

end


-- Decide on which direction to spawn the animation in.
function CheckDirection()

	if states:GetCurrDirection() == 1 then
		punchAnim:GetPrefab():GetTransformComponent():SetScale(vec3.new(-8, 4, 0))
	end
	
	if states:GetCurrDirection() == 2 then
		punchAnim:GetPrefab():GetTransformComponent():SetScale(vec3.new(8, 4, 0))
	end

end


-- Set the animation active.
function AttackAnimation()

	punchAnim:SetActive(true)

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	if attackHitbox:GetPrefab():GetCollisionComponent():GetIsCol() == true then
		punchAnim:SetActive(false)
		attackHitbox:SetActive(false)
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	if timePassed >= attackDuration then

		timePassed = 0
		punchAnim:SetActive(false)
		attackHitbox:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	print("before invulnerability")

	if myCollision:GetColBottom() == false then
		timePassed = 0
		attackHitbox:SetActive(false)
		manaConsumed = true
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	end
	
	if myCollision:GetColEnemyHitbox() == true
	or myCollision:GetColEnemyDashHitbox() == true
	or myCollision:GetColEnemyCounterHitbox() == true
	or myCollision:GetColPitfall() == true then
				
		manaConsumed = true
		states:SetCurrLogicState("HURT")
		sprite:SetAnimation("HURT",states)
	end
	
	print("after invulnerability")

end


-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

			print("before choosing state")

	if _G.i:KeyDown(CVK_LEFT) or _G.i:KeyDown(CVK_RIGHT) then
		if _G.i:KeyDown(CVK_LEFT) then
			facingRight = false
			print("LEFT")
		else
			facingRight = true
			print("RIGHT")
		end
		
		manaConsumed = true
		states:SetCurrLogicState("WALK")
    sprite:SetAnimation("WALK",states)
	else
		manaConsumed = true
		states:SetCurrLogicState("IDLE")
    sprite:SetAnimation("IDLE",states)
	end
	
			print("after choosing state")


end
