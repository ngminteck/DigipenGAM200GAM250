--[[

This script controls the falling of the character.

Possible states this script can transition to:
- GroundPound [DONE]

]]--

function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
	myStats = AllComponents:GetStatsComponent()
	attackHitbox1 = sceneactor:GetChild("PlayerGroundPoundHitbox1")
  sprite = AllComponents:GetSpriteComponent()	
	-- Falling speed
	fallSpeed = 25

end


-- Called every frame.
function Update()
	
	attackHitbox1:SetActive(true)
	
	rb:SetVel(vec2.new(0, -fallSpeed))
	
	if myCollision:GetColBottom() == true then

		states:SetCurrLogicState("GROUNDPOUND")
		sprite:SetAnimation("GROUNDPOUND",states)
	end

end