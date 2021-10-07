--[[

This script controls this enemy's windup for dash.

Possible states this script can transition to:
- Dash

]]--


-- Called at first frame.
function Start(sceneactor,player)

	-- Get components and children
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myTransform = AllComponents:GetTransformComponent()
	myPos = myTransform:GetPosition().x

	-- Get the player
	myPlayer = player:GetPrefab()
	playerTransform = myPlayer:GetTransformComponent()
	playerPos = playerTransform:GetPosition().x

	-- Variables for windup
	timePassedForWindup = 0
	windupDuration = 2

end


-- Called every frame.
function Update()
	
	UpdatePosition()
	FacePlayerDirection()
	DashWindup()
	CheckGetHurt()

end


-- Update the player's own position
function UpdatePosition()

	myPos = myTransform:GetPosition().x
	playerPos = playerTransform:GetPosition().x

end


-- Face the direction where player is.
function FacePlayerDirection()

	if myPos < (playerPos - 5) then
		states:SetCurrDirection(Direction.Right)
	end

	if myPos > (playerPos + 5) then
		states:SetCurrDirection(Direction.Left)
	end

end


-- Does nothing (or spawn particles) while preparing to dash.
function DashWindup()

	rb:SetVel(vec2.new(0, rb:GetVel().y))

	timePassedForWindup = timePassedForWindup + _G.DT

	if timePassedForWindup >= windupDuration then

		states:SetCurrLogicState("DASH")
		timePassedForWindup = 0

	end

end


-- Check whether this enemy got hurt.
function CheckGetHurt()

	if myCollision:GetColPlayerHitbox() == true
	or myCollision:GetColPlayerDashHitbox() == true
	or myCollision:GetColBigPunchHitbox() == true then
		_G.TotalHits = _G.TotalHits + 1
		states:SetCurrLogicState("HURT")
	end

end
