--[[

This script controls the falling of the character.

Possible states this script can transition to:
- Hurt [DONE]
- Jump-attack [DONE]
- Land [DONE]
- Jump

]]--


-- Called at first frame.
function Start(sceneactor)

	-- Get components
	AllComponents = sceneactor:GetPrefab()
	states = AllComponents:GetMovementComponent()
	transComp = AllComponents:GetTransformComponent()
	rb = AllComponents:GetRigidBodyComponent()
	myCollision = AllComponents:GetCollisionComponent()
    colObj = myCollision:GetCollidedObject()
	myStats = AllComponents:GetStatsComponent()
  	sprite = AllComponents:GetSpriteComponent()
	
end


-- Called every frame.
function Update()
   --states:SetCurrDirection("Right")
   rb:SetVel(vec2.new(1,1))
end

