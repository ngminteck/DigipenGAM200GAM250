--[[

This script controls this enemy's attack.

Possible states this script can transition to:
- Walk [DONE]

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	attackHitbox = sceneactor:GetChild("EnemyCounterHitbox")

	-- Variables for managing attack
	timePassed = 0
	attackDuration = 1

	-- Variables for managing hitbox spawning
	timePassedForHitbox = 0
	hitboxDelay = 0.6

end


-- Called every frame.
function Update()

	ExecuteAttack()
	CheckHitboxCollision()
	AttackCountdown()

end


-- Executes the attack.
function ExecuteAttack()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	timePassedForHitbox = timePassedForHitbox + _G.DT

	if timePassedForHitbox >= hitboxDelay then

		attackHitbox:SetActive(true)
		timePassedForHitbox = -0.4

	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	if attackHitbox:GetPrefab():GetCollisionComponent():GetIsCol() == true then
	
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("WALK")
		
	end

end


-- Counts down to the end of attack.
function AttackCountdown()

	timePassed = timePassed + _G.DT

	if timePassed >= attackDuration then

		timePassed = 0
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("WALK")

	end

end
