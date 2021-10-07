--[[

This script controls this enemy's dash-attack.

Possible states this script can transition to:
- Walk [DONE]

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components and children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	attackHitbox = sceneactor:GetChild("EnemyDashHitbox")

	-- Variables for attack
	timePassedForHitbox = 0
	durationForActive = 0.1
	durationForInactive = 1
	canAttack = true

end


-- Called every frame.
function Update()

	rb:SetVel(vec2.new(0, rb:GetVel().y))
	timePassedForHitbox = timePassedForHitbox + _G.DT

	SpawnHitbox()
	CheckHitboxCollision()
	DespawnHitbox()

end


-- Spawns the hitbox.
function SpawnHitbox()

	if timePassedForHitbox >= durationForActive and canAttack == true then
		attackHitbox:SetActive(true)
	end

end


-- Check for hitbox collisions
function CheckHitboxCollision()

	if attackHitbox:GetPrefab():GetCollisionComponent():GetIsCol() == true then
	
		print("accessed")
		attackHitbox:SetActive(false)
		canAttack = false
		
	end

end


-- Counts down to the end of attack.
function DespawnHitbox()

	if timePassedForHitbox >= durationForInactive then

		timePassedForHitbox = 0
		canAttack = true
		attackHitbox:SetActive(false)
		states:SetCurrLogicState("WALK")

	end

end
