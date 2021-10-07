--[[

This script controls the first attack of the character.

Possible states this script can transition to:
- Idle [DONE]
- Walk [DONE]
- Hurt [DONE]
- Fall [DONE]

TO DO: Add energy cost

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components/Children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox1 = sceneactor:GetChild("PlayerGroundPoundHitbox1")
	attackHitbox2 = sceneactor:GetChild("PlayerGroundPoundHitbox2")
  sprite = AllComponents:GetSpriteComponent()
	-- Variables for managing attack
	timePassed = 0
	attackDuration = 0.9 -- set proper number when applicable
	--proceed = false
	done = false

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	--hitboxDelay = 0.2
	
end


-- Called every frame.
function Update()



	-- Disallows momentum from SurtrWalk
	rb:SetVel(vec2.new(0, 0))

	ExecuteAttack()
	--CheckHitboxCollision()
	AttackCountdown()
	CheckInterruptions()

end


-- Executes the first attack.
function ExecuteAttack()

	--[[timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay and done == false then

		attackHitbox2:SetActive(true)
		
		done = true
		timePassedForHitbox = -0.5

	end--]]
	
	attackHitbox2:SetActive(true)

end


-- Check for hitbox collisions
--[[function CheckHitboxCollision()
		
	if attackHitbox2:GetPrefab():GetCollisionComponent():GetColPlayerHitbox() == true then
		attackHitbox2:SetActive(false)
	end

end--]]


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	if timePassed > attackDuration then
		
		attackHitbox1:SetActive(false)
		attackHitbox2:SetActive(false)
		ChooseNextState()

	end

end


-- Check if anything interrupts the attack.
function CheckInterruptions()

	if myCollision:GetColBottom() == false then

		timePassed = 0
		done = false
		attackHitbox1:SetActive(false)
		attackHitbox2:SetActive(false)
		states:SetCurrLogicState("FALL")
    sprite:SetAnimation("FALL",states)
	end

end


-- Called when attack ends: Choose what to do NextAttack
function ChooseNextState()

	if _G.i:KeyDown(KEY_LEFT) or _G.i:KeyDown(KEY_RIGHT) then
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
